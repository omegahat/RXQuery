#include <simplestore/simplestore.h>
#include <sstream>
#include "marshal.h"
#include "extFun.hpp"
#include "RModule.hpp"

#include <Rinternals.h>



extern "C"
SEXP
R_registerZorbaModule(SEXP s_ctx, SEXP r_module)
{
    zorba::StaticContext *ctx;
    zorba::ExternalModule *mod;

    ctx = R_GET_REF(s_ctx, zorba::StaticContext);
    mod = R_GET_REF(r_module, zorba::ExternalModule);    

    ctx->registerModule(mod);
    return(ScalarLogical(TRUE));
}

extern "C"
SEXP
R_createRModule(SEXP uri, SEXP env, SEXP ifactory/* will set this as a default factory in the module. */, SEXP ans)
{
  RExternalModule * mod = new RExternalModule(CHAR(STRING_ELT(uri, 0)), env);
  SEXP ref;

  PROTECT(ref = R_MakeExternalPtr(mod, Rf_install("RExternalModule"), R_NilValue));
  // XXX finalizer.  Decremenet the reference count.
  SET_SLOT(ans, Rf_install("ref"), ref);
  UNPROTECT(1);
  return(ans);
}

extern "C"
SEXP
R_RExternalModule_getEnvironment(SEXP r_module)
{
    RExternalModule *mod;
    mod = R_GET_REF(r_module, RExternalModule);    
    return(mod->getEnvironment());
}


SEXP
makeExternalRObject(const void *val, const char * const className)
{
    SEXP k, ans, ref;
    PROTECT(k = MAKE_CLASS(className));
    PROTECT(ans = NEW(k));
    PROTECT(ref = R_MakeExternalPtr((void *) val, Rf_install(className), R_NilValue));
    SET_SLOT(ans, Rf_install("ref"), ref);
    UNPROTECT(3);
    return(ans);
}
