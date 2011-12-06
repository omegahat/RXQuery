#include <zorba/external_module.h>
#include <zorba/external_function.h>

#include "Rdefines.h"

class RExternalModule : public zorba::ExternalModule 
{
protected:
    SEXP env;
    zorba::String *URI;

public:
    RExternalModule(const char *uri) {
        URI = new zorba::String(uri);
        
    }
    RExternalModule(const char *uri, SEXP e) {
        URI = new zorba::String(uri);
        setEnv(e);
    };

    zorba::String getURI() const {
        return(*URI);
    }

    zorba::StatelessExternalFunction *getExternalFunction(zorba::String name) const;

    SEXP getEnvironment() const { return(env);}
    void setEnv(SEXP e) {
        env = e;
        R_PreserveObject(env);
    }
};
