#include <zorba/collection.h>
#include "marshal.h"

extern "C"
SEXP
R_Collection_addNodes(SEXP r_collection, SEXP r_items)
{
    zorba::Collection *coll = R_GET_REF(r_collection, zorba::Collection);
    int n, i;
    SEXP ans;
    n = Rf_length(r_items);
    PROTECT(ans = allocVector(LGLSXP, n));
    for(i = 0; i < n; i++) {
        zorba::Item *it;
        it = R_GET_REF(VECTOR_ELT(r_items, i), zorba::Item);
//        fprintf(stderr, "type = %s  %s\n", it->getType().getStringValue().c_str(), it->getStringValue().c_str());
        LOGICAL(ans)[i] = coll->addNode(*it);
    }
    UNPROTECT(1);
    return(ans);
}

extern "C"
SEXP
R_Collection_getName(SEXP r_collection)
{
    zorba::Collection *coll = R_GET_REF(r_collection, zorba::Collection);
    return(convertItemToR(coll->getName()));
}

extern "C"
SEXP
R_Collection_size(SEXP r_collection)
{
    zorba::Collection *coll = R_GET_REF(r_collection, zorba::Collection);
    return(ScalarInteger(coll->size()));
}

extern "C"
SEXP
R_Collection_deleteNode(SEXP r_collection, SEXP r_item)
{
    zorba::Collection *coll = R_GET_REF(r_collection, zorba::Collection);
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    bool val = coll->deleteNode(*it);
    return(ScalarLogical(val));
}


