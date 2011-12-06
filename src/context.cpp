#include <simplestore/simplestore.h>
#include <zorba/xmldatamanager.h>

#include "marshal.h"





extern "C"
SEXP
R_initZorbaEngine(SEXP r_obj)
{
  SEXP ref;
  zorba::simplestore::SimpleStore* lStore = zorba::simplestore::SimpleStoreManager::getStore();
  zorba::Zorba *xqEngine = zorba::Zorba::getInstance(lStore);

  PROTECT(ref = R_MakeExternalPtr(xqEngine, Rf_install("zorba::Zorba"), R_NilValue));
  // XXX finalizer.  Decremenet the reference count.
  SET_SLOT(r_obj, Rf_install("ref"), ref);
  UNPROTECT(1);
  return(r_obj);
}

extern "C"
SEXP
R_createStaticContext(SEXP engine, SEXP r_obj, SEXP dynamic)
{

  zorba::Zorba *eng = (zorba::Zorba *) R_ExternalPtrAddr(GET_SLOT(engine, Rf_install("ref")));

  SEXP ref;
  zorba::StaticContext_t tmp;
  if(0 && LOGICAL(dynamic)[0]) {
    // need to get the dynamic context from the XQuery_t object.
    // tmp = eng->createDynamicContext();
  } else
     tmp = eng->createStaticContext();
  zorba::StaticContext *sctx = tmp.get();
  sctx->addReference();

  PROTECT(ref = R_MakeExternalPtr(sctx, Rf_install("zorba::StaticContext"), R_NilValue));
  // XXX finalizer.  Decrement the reference count.
  SET_SLOT(r_obj, Rf_install("ref"), ref);
  UNPROTECT(1);

  return(r_obj);
}

#if 0
extern "C"
SEXP
R_createStaticContext(SEXP r_engine)
{
  zorba::Zorba *xqEngine;

  xqEngine = R_GET_REF(r_engine, zorba::Zorba);

  zorba::StaticContext * lContext = xqEngine->createStaticContext();
  return(makeExternalRObject((void*) lContext,  "zorba::StaticContext"));
}
#endif


extern "C"
SEXP
R_get_xmlDataManager(SEXP engine, SEXP r_obj)
{
  SEXP ref;
  zorba::Zorba *eng = (zorba::Zorba *) R_ExternalPtrAddr(GET_SLOT(engine, Rf_install("ref")));
  zorba::XmlDataManager *xml = eng->getXmlDataManager();
  PROTECT(ref = R_MakeExternalPtr(xml, Rf_install("zorba::XmlDataManager"), R_NilValue));
  // XXX finalizer.  Decremenet the reference count.
  SET_SLOT(r_obj, Rf_install("ref"), ref);
  UNPROTECT(1);
  return(r_obj);
}

extern "C"
SEXP
R_context_setVariable(SEXP r_ctxt, SEXP r_id, SEXP r_value)
{
  zorba::DynamicContext *ctxt;
  ctxt = R_GET_REF(r_ctxt, zorba::DynamicContext);
  zorba::Zorba *eng;
  eng = R_GET_REF(GET_SLOT(r_ctxt, Rf_install("engine")), zorba::Zorba);
  zorba::Item val;
  val = convertRScalarToItem(r_value, eng->getItemFactory());
  bool ans = ctxt->setVariable(CHAR(STRING_ELT(r_id, 0)), val);
  
  return(ScalarLogical(ans));
}


extern "C"
SEXP
R_context_setDefaultCollection(SEXP r_ctxt, SEXP r_value)
{
  zorba::DynamicContext *ctxt;
  ctxt = R_GET_REF(r_ctxt, zorba::DynamicContext);
  zorba::Zorba *eng;
  eng = R_GET_REF(GET_SLOT(r_ctxt, Rf_install("engine")), zorba::Zorba);
  if(!eng) {
     zorba::simplestore::SimpleStore* lStore = zorba::simplestore::SimpleStoreManager::getStore();
     eng = zorba::Zorba::getInstance(lStore);
  }
  zorba::Item val;
  val = convertRScalarToItem(r_value, eng->getItemFactory());
  bool ans = ctxt->setDefaultCollection(val);
  return(ScalarLogical(ans));
}




extern "C"
SEXP
R_context_compileQuery(SEXP r_ctxt, SEXP r_query)
{
  zorba::Zorba *eng;
  eng = R_GET_REF(GET_SLOT(r_ctxt, Rf_install("engine")), zorba::Zorba);

  zorba::StaticContext *ctxt;
  ctxt = R_GET_REF(r_ctxt, zorba::StaticContext);
  zorba::XQuery_t q;
  q = eng->compileQuery(CHAR(STRING_ELT(r_query, 0)), ctxt); 

//  SEXP ans = R_getQueryResults(q);
//  return(ans);

  return(R_createRXQueryObject(q));
}

void
R_free_XQuery(SEXP obj)
{
    zorba::XQuery *c;
    c = (zorba::XQuery *) EXTPTR_PTR(obj);
//    fprintf(stderr, "R_free_XQuery: %p %d\n", (void *) c, c->getRefCount());
    c->removeReference();
    R_ClearExternalPtr(obj);
}


SEXP
R_createRXQueryObject(zorba::XQuery_t q)
{
  SEXP ref, klass, r_obj;
  PROTECT(klass = MAKE_CLASS("zorba::XQuery"));
  PROTECT(r_obj = NEW(klass));
  //  zorba::Zorba *eng = (zorba::Zorba *) R_ExternalPtrAddr(GET_SLOT(engine, Rf_install("ref")));
  zorba::XQuery *tmp;
  tmp = q.get();
  tmp->addReference();
  PROTECT(ref = R_MakeExternalPtr(tmp, Rf_install("zorba::XQuery"), R_NilValue));
  R_RegisterCFinalizer(ref, R_free_XQuery);
  SET_SLOT(r_obj, Rf_install("ref"), ref);
  UNPROTECT(3);
  return(r_obj);
}


SEXP
R_createRStaticContextObject(zorba::StaticContext_t sctx)
{
  SEXP ref, klass, r_obj;
  PROTECT(klass = MAKE_CLASS("StaticContext"));
  PROTECT(r_obj = NEW(klass));
  //  zorba::Zorba *eng = (zorba::Zorba *) R_ExternalPtrAddr(GET_SLOT(engine, Rf_install("ref")));
  PROTECT(ref = R_MakeExternalPtr(sctx, Rf_install("zorba::StaticContext"), R_NilValue));
  // XXX finalizer.  Decremenet the reference count.
  SET_SLOT(r_obj, Rf_install("ref"), ref);

  UNPROTECT(3);
  return(r_obj);
}
