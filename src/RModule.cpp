#include <simplestore/simplestore.h>
#include "RModule.hpp"
#include "marshal.h"
#include "extFun.hpp"

zorba::StatelessExternalFunction *
RExternalModule::getExternalFunction(zorba::String name) const
{
   
    SEXP val;
    zorba::StatelessExternalFunction *ans = NULL;
    const char * const str = name.c_str();

    val = findVar(Rf_install(str), env); /* findVarInFrame3(env, Rf_install(str), (Rboolean) TRUE) for just this frame. */

    if(val == R_UnboundValue) {
        // raise an exception with our own class.
        // throw ExternalFunctionData::createZorbaException
        fprintf(stderr, "Can't find %s in module\n", str);
        return(NULL);
//        throw zorba::DynamicException();
    }

    if(TYPEOF(val) == PROMSXP)
        val = Rf_eval(val, R_GlobalEnv);
    if(TYPEOF(val) == CLOSXP) {
        zorba::ItemFactory *itemFactory;
        RXQueryFunction *func;
        SEXP rval;

        zorba::simplestore::SimpleStore* lStore = zorba::simplestore::SimpleStoreManager::getStore();
        zorba::Zorba *zorba = zorba::Zorba::getInstance(lStore);
        itemFactory = zorba->getItemFactory();

        func = new RXQueryClosureFunction(str, itemFactory, val, true);
        PROTECT(rval = makeExternalRObject(func, "RClosureExternalFunction"));
        defineVar(Rf_install(str), rval, getEnvironment());
        UNPROTECT(1);
        ans = dynamic_cast<zorba::PureStatelessExternalFunction*>(func);
    } else {
       ans = R_GET_REF(val, zorba::StatelessExternalFunction);
    }

    return(ans);
}
