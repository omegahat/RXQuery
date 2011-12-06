#include "marshal.h"

#include "extFun.hpp"
#include "RError.hpp"

SEXP RXQueryFunction::callR(const zorba::StatelessExternalFunction::Arguments_t &args,
                            const zorba::StaticContext *sctxt, const zorba::DynamicContext *dynctxt) const
{
  int numArgs;

  SEXP e, ptr, rval;
  int errorOccurred = 0, i;

//  zorba::XQuery *q = query.get();
//  zorba::StaticContext *q = context.get();

  numArgs = args.size();

  PROTECT(e = ptr = allocVector(LANGSXP, numArgs + 1 + (sctxt != NULL) + (dynctxt != NULL)));
  SETCAR(ptr, getRFunctionSEXP()); ptr = CDR(ptr);

  for(i = 0; i < numArgs; i++) {
    zorba::ItemSequence* iseq = args[i];
    zorba::Item lItem;
    iseq->next(lItem); //XXX need to do the entire sequence.

    SETCAR(ptr, convertItemToR(lItem)); ptr = CDR(ptr);
  }

  if(sctxt) {
      SETCAR(ptr, makeExternalRObject(sctxt, "zorba::StaticContext")); ptr = CDR(ptr);
  }
  if(dynctxt)
     SETCAR(ptr, makeExternalRObject(dynctxt, "zorba::DynamicContext"));

  rval = R_tryEval(e, R_GlobalEnv, &errorOccurred);

  // if an error has occurred, then raise an R error.

  if(errorOccurred) {
      throw std::exception(); //RXQueryError();
  }

  UNPROTECT(1);

  return(rval);
}

SEXP RXQueryFunction::callR(const zorba::StatelessExternalFunction::Arguments_t &args) const
{
    return(callR(args, (zorba::StaticContext *) NULL, (zorba::DynamicContext *)NULL));
}


extern "C"
SEXP
R_getItemFactory(SEXP r_engine)
{
  zorba::Zorba *xqEngine;

  xqEngine = R_GET_REF(r_engine, zorba::Zorba);

  zorba::ItemFactory *itemFactory = xqEngine->getItemFactory();
  return(makeExternalRObject((void*) itemFactory,  "zorba::ItemFactory"));
}



