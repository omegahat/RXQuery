#include <zorba/xmldatamanager.h>
#include <sstream>
#include "marshal.h"


extern "C"
SEXP
R_xmlManager_loadDocuments(SEXP r_mgr, SEXP r_doc, SEXP isURI, SEXP asText)
{
    zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);
    int numDocs = Rf_length(r_doc), i;

    zorba::Item *it, item;
    SEXP ans;
    PROTECT(ans = NEW_LIST(numDocs));

    for(i = 0; i < numDocs; i++) {
	try {
        if(LOGICAL(isURI)[i]) 
            item = mgr->loadDocumentFromUri(CHAR(STRING_ELT(r_doc, i)));
        else {
               if(LOGICAL(asText)[i]) {
                  std::istringstream stream(CHAR(STRING_ELT(r_doc, i)));
                  item = mgr->parseDocument(stream);
            } else
                  item = mgr->loadDocument(CHAR(STRING_ELT(r_doc, i)));
        }        
        it = new zorba::Item(item);
        if(it->isNode())
           SET_VECTOR_ELT(ans, i, makeXMLNodeRef(*it));
        else
           SET_VECTOR_ELT(ans, i, makeExternalRObject(it, "zorba::Item"));
      } catch(zorba::ZorbaException &e) {
           // We could keep going and just issue a warning.
         raiseZorbaError(e);
      }
    }
    UNPROTECT(1);
    return(ans);
}

/* Probably not used anymore. See above version that is vectorized. */
extern "C"
SEXP
R_xmlManager_loadDocument(SEXP r_mgr, SEXP r_doc, SEXP isURI, SEXP asText)
{
  zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);

  if(LOGICAL(isURI)[0]) 
      mgr->loadDocumentFromUri(CHAR(STRING_ELT(r_doc, 0)));
  else {
    if(LOGICAL(asText)[0]) {
      std::istringstream stream(CHAR(STRING_ELT(r_doc, 0)));
      mgr->parseDocument(stream);
    } else
      mgr->loadDocument(CHAR(STRING_ELT(r_doc, 0)));
  }

  // Should be returning the item

  return(ScalarLogical(TRUE));
}

void
R_free_Collection(SEXP obj)
{
    zorba::Collection *c;
    c = (zorba::Collection *) EXTPTR_PTR(obj);
    c->removeReference();
    R_ClearExternalPtr(obj);
}

extern "C"
SEXP
R_xmlManager_createCollection(SEXP r_mgr, SEXP r_name)
{
  zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);

  int i = 0, n;
  SEXP ans, tmp;
  n = Rf_length(r_name);
  PROTECT(ans = NEW_LIST(n));
  zorba::Collection_t el;
  zorba::Collection *c;
  for(i = 0; i < n; i++) {
      try {
          el =  mgr->createCollection( zorba::String(CHAR(STRING_ELT(r_name, i))));
          c = el.get();
          c->addReference();
          SET_VECTOR_ELT(ans, i, tmp = makeExternalRObject(c, "zorba::Collection"));
          R_RegisterCFinalizer(GET_SLOT(tmp, Rf_install("ref")), R_free_Collection);
      } catch(zorba::ZorbaException &e) {
          raiseZorbaError(e);
      }
  }
  UNPROTECT(1);
  return(ans);
}



extern "C"
SEXP
R_xmlManager_getDocument(SEXP r_mgr, SEXP r_doc)
{
     zorba::Item *it, item;
     zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);
     int i, n;
     SEXP ans;

     n = Rf_length(r_doc);
     PROTECT(ans = NEW_LIST(n));

     for(i = 0 ; i < n ; i++) {
       try {
          item = mgr->getDocument(zorba::String(CHAR(STRING_ELT(r_doc, i))));
          it = new zorba::Item(item);
	  if(it->isNode())
    	     SET_VECTOR_ELT(ans, i, makeXMLNodeRef(*it));
          else
             SET_VECTOR_ELT(ans, i, makeExternalRObject(it, "zorba::Item"));
       } catch(zorba::ZorbaException &e) {
         raiseZorbaWarning(e);
       }
     }

     UNPROTECT(1);
     return(ans);
}



extern "C"
SEXP
R_Manager_getCollections(SEXP r_mgr, SEXP r_collectionNames)
{
    zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);
    int i, n;

    n = Rf_length(r_collectionNames);
    SEXP ans;
    PROTECT(ans = NEW_LIST(n));
    for(i = 0; i < n; i++) {
      try {
        zorba::Collection_t col;
        col = mgr->getCollection(zorba::String(CHAR(STRING_ELT(r_collectionNames, i))));
        SET_VECTOR_ELT(ans, i, makeExternalRObject(col.get(), "zorba::Collection"));
      } catch(zorba::ZorbaException &e) {
         raiseZorbaWarning(e);
      }
    }
    SET_NAMES(ans, r_collectionNames);
    UNPROTECT(1);

    return(ans);
}


extern "C"
SEXP
R_xmlManager_deleteCollections(SEXP r_mgr, SEXP r_collectionNames)
{
    zorba::XmlDataManager *mgr = R_GET_REF(r_mgr, zorba::XmlDataManager);
    int i, n;
    
    n = Rf_length(r_collectionNames);
    SEXP ans;
    PROTECT(ans = NEW_LOGICAL(n));
    for(i = 0; i < n; i++) {
      try {
          LOGICAL(ans)[i] = mgr->deleteCollection(zorba::String(CHAR(STRING_ELT(r_collectionNames, i))));
      } catch(zorba::ZorbaException &e) {
         raiseZorbaWarning(e);
      }
    }
    UNPROTECT(1);
    return(ans);
}
