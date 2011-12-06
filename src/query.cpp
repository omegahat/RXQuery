#include <simplestore/simplestore.h>
#include <sstream>
#include <zorba/exception.h>

#include "marshal.h"
#include "extFun.hpp"

#include <vector>
#include <strstream>

#include <Rinternals.h>




void registerFunctions(SEXP r_funs, zorba::StaticContext_t sctx, zorba::ItemFactory *itemFactory, SEXP r_deterministic);

//zorba::StaticException &e
void
raiseZorbaError(zorba::QueryException &e)
{
     PROBLEM "%s in %s (line = %d, column = %d)", e.getDescription().c_str(),
                                           e.getQueryURI().c_str(),
                                               e.getLineBegin(),
                                               e.getColumnBegin()
	ERROR;
}

void
raiseZorbaError(zorba::ZorbaException &e)
{
    PROBLEM "error in Zorba query %s in %s at %d",  e.getDescription().c_str(),
        e.getFileName().c_str(),
        (int) e.getFileLineNumber()
	ERROR;
}


void
raiseZorbaWarning(zorba::ZorbaException &e)
{
    PROBLEM "error in Zorba query %s in %s at %d",  e.getDescription().c_str(),
        e.getFileName().c_str(),
        (int) e.getFileLineNumber()
	WARN;
}





extern "C"
SEXP
R_executeXQuery(SEXP r_query, SEXP r_ctxt, SEXP r_baseURI, SEXP r_guessSize) //, SEXP r_funs, SEXP r_deterministic)
{
 SEXP ans = R_NilValue;

 try {
        /* Create a new instance of the Zorba engine */
    zorba::simplestore::SimpleStore* lStore = zorba::simplestore::SimpleStoreManager::getStore();
    zorba::Zorba *xqEngine = zorba::Zorba::getInstance(lStore);

    zorba::StaticContext_t sctx = NULL;
    zorba::XQuery_t q;

     /* If we are given functions in r_funs, then create a StaticContext
        and register these functions using the names on the list and the values
        in that list.  
        Then compile the query and that evaluates it also.
      */

    if(Rf_length(r_ctxt))
       sctx = R_GET_REF(r_ctxt, zorba::StaticContext);
    else
       sctx = xqEngine->createStaticContext();

    const char *baseURI;
    if(Rf_length(r_baseURI) && (baseURI = CHAR(STRING_ELT(r_baseURI, 0)))[0]) {
       sctx->setBaseURI(zorba::String(baseURI));
    }

    try {
          if(sctx == NULL)
             q = xqEngine->compileQuery(CHAR(STRING_ELT(r_query, 0)));
          else 
             q = xqEngine->compileQuery(CHAR(STRING_ELT(r_query, 0)), sctx);
    } catch (zorba::ZorbaException &e) {
        raiseZorbaError(e);
          PROBLEM  "error compiling zorba query: %s",  e.getDescription().c_str() // e.theDescription
              ERROR;
    }



#if 0
    /* Here we try to create a query separately, get the context
       compile the query, register the functions with the context using the query.
       But the getStaticContext() returns a constant context so we can't register functions,
       etc.
     */
    q = xqEngine->createQuery();
    sctx = q->getStaticContext();

    if(Rf_length(r_baseURI) && (baseURI = CHAR(STRING_ELT(r_baseURI, 0)))[0]) {
       sctx->setBaseURI(zorba::String(baseURI));
    }

    if(Rf_length(r_funs))
      registerFunctions(r_funs, sctx, itemFactory, r_addQuery, sctx);

    try {
      q = q->compileQuery(String(CHAR(STRING_ELT(r_query, 0))), sctx); 
    } catch(zorba::StaticException &e) {
        PROBLEM "%s (line = %d, column = %d)", e.getQueryURI().c_str(),
                                               e.getLineBegin(),
                                               e.getColumnBegin()
	ERROR;
    } 
#endif

#if 0
/* Just testing here when the query is know to be protected. */
    std::ostrstream str;
    q->printPlan(str);
    fprintf(stderr, "query plan:\n %s\n", str.str());
#endif

    ans = R_getQueryResults(q, REAL(r_guessSize)[0],  REAL(r_guessSize)[1]);

    /*
       xqEngine->shutdown();
       zorba::simplestore::SimpleStoreManager::shutdownStore(lStore);
     */

   } catch(zorba::ZorbaException &e) {
       raiseZorbaError(e);
   }

  return(ans);
}

#define MAX(a, b) ((a) < (b) ? (b) : (a))

SEXP
R_getQueryResults(zorba::XQuery_t lQuery, double initSize, double factor)
{

  zorba::Iterator_t lIterator = lQuery->iterator();
  unsigned int n = 10, i = 0;
  zorba::Item lItem;
  SEXP ans;
  int numProtects = 0;

  n = MAX(10, initSize);
  if(factor < 1)
    factor = 1.5;

#if 0
  try {
    lIterator->open();
    while(lIterator->next(lItem)) n++;
    lIterator->close();
  } catch(zorba::DynamicException &e) {
     PROBLEM "%s (line = %d, column = %d)", e.getQueryURI().c_str(), e.getLineBegin(), e.getColumnBegin()
	ERROR;

  } catch(zorba::ZorbaException &e) {
      raiseZorbaError(e);
  }
#endif

#ifdef R_DEBUG_XQUERY
  fprintf(stderr, "Number of result items: %d\n", n);
#endif

 try {
  try {
     lIterator->open();
     PROTECT(ans = NEW_LIST(n)); numProtects++;
     while ( lIterator->next(lItem) ) {
#ifdef R_DEBUG_XQUERY
         fprintf(stderr, "type %s", lItem.getType().getStringValue().c_str());
#endif
       if(Rf_length(ans) <= i) {
          SET_LENGTH(ans, Rf_length(ans) * factor);
          PROTECT(ans); numProtects++;
       }

       SET_VECTOR_ELT(ans, i++, convertItemToR(lItem));
     }

     SET_LENGTH(ans, i);

     UNPROTECT(numProtects);
         //XXX use a finally to ensure it is done but just say it once.
     lIterator->close();

  } catch(zorba::DynamicException &e) {
      lIterator->close();
      raiseZorbaError(e);
  }
 } catch(std::exception &e) {
         PROBLEM "general exception"
             ERROR;
 }


  return(ans);
}



    /* Register the functions in r_funs with the StaticContext so that they can be used
       within XQuery commands within that context. The r_funs object is expected to have 
       names that are used as the XQuery names for the function. 
       The elements can be names of R functions or function objects.
       We don't R_Preserve these objects as we expect that they will be protected from garbage
       collection while they are being used.
    */
void registerFunctions(SEXP r_funs, zorba::StaticContext_t sctx, zorba::ItemFactory *itemFactory, SEXP r_deterministic)
{
    int i;
//    std::vector<RXQueryFunction *> funs;

    for(int i = 0; i < Rf_length(r_funs) ; i++) {
      RXQueryFunction *func;
      bool deterministic;
      SEXP el = VECTOR_ELT(r_funs, i);

      const char *id = CHAR(STRING_ELT(GET_NAMES(r_funs), i));
      deterministic = LOGICAL(r_deterministic)[i];

      if(TYPEOF(el) == STRSXP) {
          /* Compare the name on the list with the value of the list. If they are the same,
             it is a regular RXQueryFunction object. Otherwise, a different name in XQuery and R
             so use an RXQueryAltNamedFunction. */
        if(strcmp(id, CHAR(STRING_ELT(el, 0))) == 0)
         func = new RXQueryFunction(CHAR(STRING_ELT(el, 0)), itemFactory, deterministic);
        else
         func = new RXQueryAltNamedFunction(id, itemFactory, CHAR(STRING_ELT(el, 0)), deterministic);
      } else if(TYPEOF(el) == CLOSXP)
  	    /* Got a function object, so register that as an RXQueryClosureFunction() object.*/
         func = new RXQueryClosureFunction(id, itemFactory, el, deterministic);

#if 0
      if(LOGICAL(r_addQuery)[i]) 
	func->setContext(ctxt);
#endif


#if 0
      sctx->registerStatelessExternalFunction(func);
#endif

//      funs.push_back(func);
    }

//    return(funs);
}


extern "C"
SEXP
R_createExternalFunction(SEXP name, SEXP el, SEXP r_deterministic, SEXP r_itemFactory, SEXP ans)
{
    const char *id = CHAR(STRING_ELT(name, 0));
    bool deterministic = LOGICAL(r_deterministic)[0];
    const char *alt = NULL;
    zorba::ItemFactory *itemFactory;
    RXQueryFunction *func;
    SEXP ref;

    itemFactory = R_GET_REF(r_itemFactory, zorba::ItemFactory);

    
    if(TYPEOF(el) == STRSXP) {
          /* Compare the name on the list with the value of the list. If they are the same,
             it is a regular RXQueryFunction object. Otherwise, a different name in XQuery and R
             so use an RXQueryAltNamedFunction. */
        if(Rf_length(GET_NAMES(name)))
            alt = CHAR(STRING_ELT(GET_NAMES(name), 0));

        if(!alt || strcmp(id, alt) == 0)
         func = new RXQueryFunction(CHAR(STRING_ELT(el, 0)), itemFactory, deterministic);
        else
         func = new RXQueryAltNamedFunction(id, itemFactory, alt, deterministic);
    } else if(TYPEOF(el) == CLOSXP) {
  	    /* Got a function object, so register that as an RXQueryClosureFunction() object.*/
         func = new RXQueryClosureFunction(id, itemFactory, el, deterministic);
    }


    PROTECT(ref = R_MakeExternalPtr(func, Rf_install("RExternalFunction"), R_NilValue));
    SET_SLOT(ans, Rf_install("ref"), ref);
    UNPROTECT(1);
    
    return(ans);
}

#if 1

#include <strstream>
void
R_debug_printPlan(zorba::XQuery_t qt)
{
  for(int i = 0; i < 100; i++) {
    std::ostrstream str;
    qt->printPlan(str);  
    fprintf(stderr, "query plan:\n <plan>%s</plan>\n", str.str());
  }
}
#endif

extern "C"
SEXP
R_compileQuery(SEXP r_query, SEXP r_engine, SEXP r_obj)
{

  zorba::Zorba *eng = (zorba::Zorba *) R_ExternalPtrAddr(GET_SLOT(r_engine, Rf_install("ref")));
  zorba::XQuery_t qt = eng->compileQuery(CHAR(STRING_ELT(r_query, 0)));
//R_debug_printPlan(qt);
  return(R_createRXQueryObject(qt));

#if 0
  zorba::XQuery *q = qt.get();
  q->addReference();

  SEXP tmp;

  SET_SLOT(r_obj, Rf_install("ref"), tmp = R_MakeExternalPtr(q, Rf_install("zorba::XQuery"), R_NilValue));
  // XXX Finalizer

  return(r_obj);
#endif
}

extern "C"
SEXP
R_createDynamicContext(SEXP r_query, SEXP r_obj)
{
  zorba::DynamicContext *ctxt;
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  SEXP tmp;

  ctxt = query->getDynamicContext();
  SET_SLOT(r_obj, Rf_install("ref"), tmp = R_MakeExternalPtr(ctxt, Rf_install("zorba::DynamicContext"), R_NilValue));  
  return(r_obj);
}


extern "C"
SEXP
R_setVariableAsDocument(SEXP r_ctxt, SEXP r_var, SEXP r_uri, SEXP r_value)
{
  zorba::DynamicContext *ctxt;
  bool ans;

  std::auto_ptr<std::istream> stream(new std::stringstream(CHAR(STRING_ELT(r_value, 0))));

  ctxt = R_GET_REF(r_ctxt, zorba::DynamicContext); 
  ans = ctxt->setVariableAsDocument(CHAR(STRING_ELT(r_var, 0)), CHAR(STRING_ELT(r_uri, 0)), stream);
  return(ScalarLogical(ans));
}



extern "C"
SEXP
R_getQueryResults(SEXP r_query, SEXP size, SEXP factor)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  return(R_getQueryResults(query, REAL(size)[0], REAL(factor)[0]));
}



#include <iostream>
using namespace zorba;
extern "C"
SEXP
R_foo(SEXP r_eng)
{

  zorba::Zorba *eng;
  eng = R_GET_REF(r_eng, zorba::Zorba);
  XQuery_t lQuery = eng->compileQuery("<a><b attr='1'/><b attr='2'/></a>");

  Iterator_t lIterator = lQuery->iterator();
  lIterator->open();

  Item lItem;
  while ( lIterator->next(lItem) ) {
    Iterator_t lChildIter = lItem.getChildren();

    lChildIter->open();
    Item lChild;
    while (lChildIter->next(lChild)) {

      Item lNodeName;
      lChild.getNodeName(lNodeName);
      std::cout << "node name " << lNodeName.getStringValue() << std::endl;
      Iterator_t lAttrIter = lChild.getAttributes();
      
      lAttrIter->open();

      Item lAttr;
      while (lAttrIter->next(lAttr)) {
	std::cout << "  attribute value " << lAttr.getStringValue() << std::endl;
      }
      lAttrIter->close();
    }
    lChildIter->close();
  }

  lIterator->close();


}




extern "C"
SEXP
R_XQuery_printPlan(SEXP r_query)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  std::ostrstream str;
  query->printPlan(str);  

  const char *val = str.str();
//  fprintf(stderr, "query plan:<plan>\n'%s'</plan>", val);
  return(ScalarString(mkChar(val)));
}

extern "C"
SEXP
R_XQuery_execute(SEXP r_query)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  query->execute();
  return(R_NilValue);
}


extern "C"
SEXP
R_XQuery_getDocLoadingTime(SEXP r_query)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  SEXP ans;
  PROTECT(ans = NEW_NUMERIC(2));
  REAL(ans)[0] = query->getDocLoadingTime();
  REAL(ans)[1] = query->getDocLoadingUserTime();
  UNPROTECT(1);
  return(ans);
}

extern "C"
SEXP
R_XQuery_status(SEXP r_query)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  SEXP ans;	
  PROTECT(ans = NEW_LOGICAL(3));
  LOGICAL(ans)[0] = query->isClosed();
  LOGICAL(ans)[1] = query->isDebugMode();
  LOGICAL(ans)[2] = query->isUpdating();
  UNPROTECT(1);
  return(ans);
}

extern "C"
SEXP
R_XQuery_setTimeout(SEXP r_query, SEXP dur)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  query->setTimeout(REAL(dur)[0]);
  return(R_NilValue);
}

extern "C"
SEXP
R_XQuery_setDebugMode(SEXP r_query, SEXP dur)
{
  zorba::XQuery *query = R_GET_REF(r_query, zorba::XQuery);
  query->setTimeout(LOGICAL(dur)[0]);
  return(R_NilValue);
}
