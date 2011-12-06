#include <zorba/item.h>
#include "marshal.h"

#ifdef isNull
#undef isNull
#endif

extern "C"
SEXP
R_Item_getNodeKind(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(ScalarInteger(it->getNodeKind()));
}


extern "C"
SEXP
R_Item_convertItemToR(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(convertItemToR(*it));
}

extern "C"
SEXP
R_Item_getStringValue(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(mkString(it->getStringValue().c_str()));
}

extern "C"
SEXP
R_Item_isAtomic(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(ScalarLogical(it->isAtomic()));
}

extern "C"
SEXP
R_Item_isNaN(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(ScalarLogical(it->isNaN()));
}


extern "C"
SEXP
R_Item_isNull(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(ScalarLogical(it->isNull()));
}


extern "C"
SEXP
R_Item_isNode(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(ScalarLogical(it->isNode()));
}


extern "C"
SEXP
R_Item_getNodeName(SEXP r_item)
{
    zorba::Item *it, tmp;
    it = R_GET_REF(r_item, zorba::Item);
    if(!it->isNode()) {
        PROBLEM "getNodeName is only available for zorba::Item objects that are nodes"
         ERROR;
    }

    if(!it->getNodeName(tmp)) {
        PROBLEM "can't get the node name"
            ERROR;
    }
    return(mkString(tmp.getStringValue().c_str()));
}

extern "C"
SEXP
R_Item_getPrefix(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    try {
       return(mkString(it->getPrefix().c_str()));
    } catch(zorba::ZorbaException &e) {
        raiseZorbaError(e);
    }
}



extern "C"
SEXP
R_Item_getNamespace(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    try {
       return(mkString(it->getNamespace().c_str()));
    } catch(zorba::ZorbaException &e) {
        raiseZorbaError(e);
    }
}

extern "C"
SEXP
R_Item_getLocalName(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    try {
        return(mkString(it->getLocalName().c_str()));
    } catch(zorba::ZorbaException &e) {
        raiseZorbaError(e);
    }
}



extern "C"
SEXP
R_Item_getChildren(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    int n = 0, i;
    zorba::Iterator_t kids = it->getChildren();
    zorba::Item el;
    kids->open();
    while(kids->next(el))
        n++;
    kids->close();

    SEXP ans;
    PROTECT(ans = NEW_LIST(n));
    kids->open();
    while(kids->next(el)) {
        zorba::Item *tmp = new zorba::Item(el);
//        SET_VECTOR_ELT(ans, i++, makeExternalRObject(tmp, "zorba::Item"));
        SET_VECTOR_ELT(ans, i++, makeXMLNodeRef(*tmp));
    }
    kids->close();
    UNPROTECT(1);

    return(ans);
}



extern "C"
SEXP
R_Item_getAttributes(SEXP r_item, SEXP r_withNS)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    int n = 0, i;
    zorba::Iterator_t kids = it->getAttributes();
    zorba::Item el;
    kids->open();
    while(kids->next(el))
        n++;
    kids->close();

    SEXP ans, names, prefixes, urls;
    PROTECT(ans = NEW_CHARACTER(n));
    PROTECT(names = NEW_CHARACTER(n));
    if(LOGICAL(r_withNS)[0]) {
       PROTECT(prefixes = NEW_CHARACTER(n));
       PROTECT(urls = NEW_CHARACTER(n));
    }
    kids->open();
    while(kids->next(el)) {
//        zorba::Item *tmp = new zorba::Item(el);
//        SET_VECTOR_ELT(ans, i++, makeExternalRObject(tmp, "zorba::Item"));
//  
        SET_STRING_ELT(ans, i, mkChar(el.getStringValue().c_str()));
        zorba::Item tmp;
        el.getNodeName(tmp);
        SET_STRING_ELT(names, i, mkChar(tmp.getStringValue().c_str()));

        if(LOGICAL(r_withNS)[0]) {
            SET_STRING_ELT(prefixes, i, mkChar(el.getPrefix().c_str()));
            SET_STRING_ELT(urls, i, mkChar(el.getNamespace().c_str()));
        }
        i++;
    }
    SET_NAMES(ans, names);
    if(LOGICAL(r_withNS)[0]) {
       SET_ATTR(ans, Rf_install("prefixes"), prefixes);
       SET_ATTR(ans, Rf_install("ns"), urls);
    }
    kids->close();
    UNPROTECT(2 + LOGICAL(r_withNS)[0] ? 2 : 0);

    return(ans);
}


extern "C"
SEXP
R_Item_getType(SEXP r_item)
{
    zorba::Item *it, *el;
    it = R_GET_REF(r_item, zorba::Item);
    el = new zorba::Item(it->getType());
    return(makeExternalRObject(el, "zorba::Item"));
}

