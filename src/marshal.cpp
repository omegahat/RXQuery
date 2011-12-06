#include "marshal.h"

#include <vector>
#include <zorba/external_function.h>
#include <zorba/store_consts.h>


#include <Rinternals.h>

#ifdef isNull
#undef isNull
#endif


#define compareString(src, to) \
  strcmp(src.c_str(), to) == 0

SEXP convertItemToPOSIXct(zorba::Item value, const char *fmt = "dateTime");
SEXP makeTextNode(SEXP val);
SEXP makeXMLTextNode(zorba::Item child);


  //  src.byteEqual(to, sizeof(to))

SEXP
convertItemToR(zorba::Item item, bool asXMLSubNode)
{
  SEXP ans = R_NilValue;
  int np = 0;
 
  zorba::Item type;
  type = item.getType();
  zorba::String typeName;
  typeName = type.getStringValue();
  char *endp;
  //  return(mkString(typeName.c_str()));

#ifdef R_DEBUG_XQUERY
  fprintf(stderr, "Type: %s\n", typeName.c_str());
#endif

  try {
    if(item.isNull()) {
      ans = R_NilValue;
    //    else if(item.isNaN())
    //      ans = R_NilValue;
    }else if(item.isNode() && (( compareString(typeName, "untyped") ) || compareString(typeName, "anyType"))) {  // !item.isAtomic()
      //      fprintf(stderr, "node: %s %s\n", typeName.c_str(), item.getStringValue().c_str());      
       ans = makeXMLNode(item);
    } else if(compareString(typeName, "integer") || compareString(typeName, "int") || compareString(typeName, "short") || compareString(typeName, "unsignedShort"))
       ans = ScalarInteger(R_strtod(item.getStringValue().c_str(), &endp));
     else if(compareString(typeName, "string") || compareString(typeName, "normalizedString") || compareString(typeName, "untypedAtomic"))
       ans = mkString(item.getStringValue().c_str());
     else if(compareString(typeName, "decimal") || compareString(typeName, "double") || compareString(typeName, "float"))
       ans = ScalarReal(R_strtod(item.getStringValue().c_str(), &endp));
     else if(compareString(typeName, "boolean"))
       ans = ScalarLogical(item.getBooleanValue());
     else if(compareString(typeName, "dateTime"))
       ans = convertItemToPOSIXct(item);
     else if(compareString(typeName, "date"))
       ans = convertItemToPOSIXct(item, "date");
     else if(compareString(typeName, "time"))
       ans = convertItemToPOSIXct(item, "time");
     else if(compareString(typeName, "anyURI"))
       ans = mkString(item.getStringValue().c_str());
     else  {
       if(asXMLSubNode) {
	 ans = makeXMLTextNode(item);
       } else {
         fprintf(stderr, "unhandled type: %s.   Processed as strings for now!\n", typeName.c_str());
         ans = mkString(item.getStringValue().c_str());
       }
       // ans = R_NilValue;
     }
  } catch(zorba::SystemException &e) {
    ans = mkString(item.getStringValue().c_str());
  }


  if(np) 
     UNPROTECT(np);
  return(ans);
}


zorba::Item
convertRScalarToItem(SEXP ans, zorba::ItemFactory* lFactory)
{

  zorba::Item it;

	switch(TYPEOF(ans)) {

  	    case  REALSXP:
	      it = lFactory->createDouble(REAL(ans)[0]);
	      break;
  	    case  INTSXP:
	      it = lFactory->createInt(INTEGER(ans)[0]);
	      break;
  	    case  LGLSXP:
	      it = lFactory->createBoolean(LOGICAL(ans)[0]);
	      break;
  	    case  STRSXP:
	      it = lFactory->createString(zorba::String(CHAR(STRING_ELT(ans, 0))));
	      break;
            default:
	       break;

	}
	return(it);
}

//   zorba->lFactory = engine->getItemFactory();
zorba::ItemSequence_t 
convertRToXQuery(SEXP ans, zorba::ItemFactory* lFactory)
{
      iv_t vec;
      zorba::Item it;
      
      unsigned int len = Rf_length(ans);

      if(len == 1)  {

	it = convertRScalarToItem(ans, lFactory);

	vec.push_back(it);


      } else {


      }


      // transfer ownership of the IteratorBackedItemSequence to Zorba (using an auto_ptr)
      return zorba::ItemSequence_t(new IteratorBackedItemSequence(vec));
}


/* Map an zorba::Item to a POSIXt object within an optional format. */
SEXP
convertItemToPOSIXct(zorba::Item value, const char *fmt)
{
  const char *val;
  val = value.getStringValue().c_str();

  SEXP e, ans;
  PROTECT(e = allocVector(LANGSXP, 3));

    SETCAR(e, Rf_install("zorbaPOSIXct"));
    SETCAR(CDR(e), mkString(val));
    SETCAR(CDR(CDR(e)), mkString(fmt));

     ans = Rf_eval(e, R_GlobalEnv);

  UNPROTECT(1);
  return(ans);
}

/* Set the names on an R object whose size is already determined. */
void
RSetNames(SEXP obj, const char * const vals[])
{
  int n = Rf_length(obj);
  SEXP names;
  PROTECT(names = NEW_CHARACTER(n));
  for(int i = 0; i < n; i++) 
    SET_STRING_ELT(names, i, mkChar(vals[i]));

  SET_NAMES(obj, names);

  UNPROTECT(1);
}


SEXP
makeRNames(const char * const vals[], int n)
{
  SEXP names;
  PROTECT(names = NEW_CHARACTER(n));
  for(int i = 0; i < n; i++) 
    SET_STRING_ELT(names, i, mkChar(vals[i]));
  UNPROTECT(1);
  return(names);
}



const char * const nodeNames[] = {"name", "attributes", "children", "namespace", "namespaceDefinitions"};

const char * const nodeClassNames[] = {"XMLNode", "RXMLAbstractNode", "XMLAbstractNode"};


SEXP
makeXMLClassVector(const char *name)
{
  SEXP ans;
  int n = sizeof(nodeClassNames)/sizeof(nodeClassNames[0]), i;
  PROTECT(ans = NEW_CHARACTER(n+1));
  SET_STRING_ELT(ans, 0, mkChar(name));
  for(i = 0; i < n; i++)
    SET_STRING_ELT(ans, i + 1, mkChar(nodeClassNames[i]));
  UNPROTECT(1);
  return(ans);
}


SEXP
makeXMLTextNode(zorba::Item child)
{
  SEXP ans, classes, names;
  const char * txt;

  txt = child.getStringValue().c_str();
  PROTECT(ans = NEW_LIST(5));
  SET_VECTOR_ELT(ans, 2, mkString(txt));
  PROTECT(names = makeRNames(nodeNames, sizeof(nodeNames)/sizeof(nodeNames[0])));
  SET_STRING_ELT(names, 2, mkChar("value"));
  SET_NAMES(ans, names);
  SET_VECTOR_ELT(ans, 0, mkString("text"));

  SET_CLASS(ans, makeXMLClassVector("XMLTextNode"));
  UNPROTECT(2);
  return(ans);
}


SEXP
makeXMLNodeRef(zorba::Item &it)
{
  const char *className;
  switch(it.getNodeKind()) {
      case zorba::store::StoreConsts::documentNode:
         className = "ZorbaXMLDocument";
         break;
      case zorba::store::StoreConsts::elementNode:
         className = "ZorbaXMLElementNode";
         break;
      case zorba::store::StoreConsts::textNode:
         className = "ZorbaXMLTextNode";
         break;
      case zorba::store::StoreConsts::piNode:
         className = "ZorbaXMLPINode";
         break;
      case zorba::store::StoreConsts::commentNode:
         className = "ZorbaXMLCommentNode";
         break;
      case zorba::store::StoreConsts::attributeNode:
         className = "ZorbaXMLAttributeNode";
         break;
      case zorba::store::StoreConsts::anyNode:
      default:
         className = "zorba::Item";
  }
  zorba::Item *el = new zorba::Item(it);
  return(makeExternalRObject(el, className));
}


SEXP
makeXMLNode(zorba::Item it, bool ref)
{
  SEXP ans;

  if(ref) {
      return(makeXMLNodeRef(it));
  }


  PROTECT(ans = NEW_LIST(sizeof(nodeNames)/sizeof(nodeNames[0])));

     zorba::Iterator_t kids, vals;
     unsigned int n = 0, i, numAttrs = 0;
     zorba::Item child, val;

     SEXP names, tmp;

     it.getNodeName(val);
     SET_VECTOR_ELT(ans, 0, mkString(val.getStringValue().c_str()));

     kids = it.getAttributes();
     kids->open();  while(kids->next(child)) n++;  kids->close();
     
     PROTECT(tmp = NEW_CHARACTER(n));
     PROTECT(names = NEW_CHARACTER(n));

        for(i = 0, kids->open(); i < n ; i++)  {
          kids->next(child);
	  child.getNodeName(val);
          SET_STRING_ELT(tmp, i, mkChar(child.getStringValue().c_str()));
          SET_STRING_ELT(names, i, mkChar(val.getStringValue().c_str()));
        }  
        numAttrs = n;
        SET_NAMES(tmp, names);
        SET_VECTOR_ELT(ans, 1, tmp);
    UNPROTECT(2);
      
  /* child nodes.*/

     n = 0;
     kids = it.getChildren();
     kids->open();  while(kids->next(child)) n++;  kids->close();

 
     SEXP r_kids;
     PROTECT(r_kids = NEW_LIST(n));
        kids->open();
        for(i = 0; i < n ; i++)  {
	  kids->next(child);

	  SEXP tmp = convertItemToR(child);
          PROTECT(tmp);
          if(TYPEOF(tmp) == STRSXP) {
              tmp = makeTextNode(tmp);
          }
	  SET_VECTOR_ELT(r_kids, i, tmp);
          UNPROTECT(1);
	}
	kids->close();
     
        SET_VECTOR_ELT(ans, 2, r_kids);
     UNPROTECT(1);

     RSetNames(ans, nodeNames);

  SET_CLASS(ans, makeRNames(nodeClassNames, sizeof(nodeClassNames)/sizeof(nodeClassNames[0])));
  // SET_CLASS(ans, mkString("XMLNode"));
  UNPROTECT(1);
  return(ans);
}

SEXP 
makeTextNode(SEXP val)
{
    SEXP e, ans;
    PROTECT(e = lang2(Rf_install("xmlTextNode"), val));
    ans = Rf_eval(e, R_GlobalEnv);
    UNPROTECT(1);
    return(ans);
}

extern "C"
SEXP
R_zorbaXMLNode_toXMLNode(SEXP r_item)
{
    zorba::Item *it;
    it = R_GET_REF(r_item, zorba::Item);
    return(makeXMLNode(*it, false));
}

