#include <zorba/external_function.h>

/*

 We define are 3 classes of R external functions

 */

class RXQueryFunction : public zorba::NonePureStatelessExternalFunction, public zorba::PureStatelessExternalFunction
{

protected:
  zorba::String _name;
  zorba::ItemFactory *factory;
  bool deterministic;

#if 0
  zorba::StaticContext_t context;
  zorba::XQuery_t query;
#endif

public:

  RXQueryFunction(const char *name, zorba::ItemFactory *ifactory) {
      _name = zorba::String(name);
      factory = ifactory;
      deterministic = true;
    };

  RXQueryFunction(const char *name, zorba::ItemFactory *ifactory, bool _deterministic) {
      _name = zorba::String(name);
      factory = ifactory;
      deterministic = _deterministic;
    };

  virtual zorba::String
    getURI() const { return "urn:R"; }

  virtual zorba::String
    getLocalName() const { return _name; }

//  typedef std::vector<zorba::ItemSequence*> Arguments_t;
  virtual zorba::ItemSequence_t 
  evaluate(const zorba::StatelessExternalFunction::Arguments_t& args) const 
  {
      SEXP ans = callR(args);
      return(convertRToXQuery(ans, factory));
  }

  virtual zorba::ItemSequence_t
  evaluate(const zorba::StatelessExternalFunction::Arguments_t &args,
           const zorba::StaticContext *sctxt,
           const zorba::DynamicContext *dynctxt) const
  {
        SEXP ans = callR(args, sctxt, dynctxt);
        return(convertRToXQuery(ans, factory));
  }


   bool isDeterministic() const {
        return(deterministic);
    }


#if 0
    /* Don't need these with the Zorba 1.0 API. */
  void setContext(zorba::StaticContext_t val) {
    context = val;
  }

  zorba::StaticContext_t getContext() {
    return(context);
  }


  void setQuery(zorba::XQuery_t q) {
    query = q;
  }

  zorba::XQuery_t getQuery() {
    return(query);
  }
#endif

   protected:
      virtual SEXP callR(const zorba::StatelessExternalFunction::Arguments_t &args) const;
      virtual SEXP callR(const zorba::StatelessExternalFunction::Arguments_t &args,
                         const zorba::StaticContext *sctxt, const zorba::DynamicContext *dynctxt) const;
      virtual SEXP getRFunctionSEXP() const {
            return(Rf_install(_name.c_str()));
      } 
};


class RXQueryAltNamedFunction : public RXQueryFunction {

 protected:
  const char *_rname;

 public:
    RXQueryAltNamedFunction(const char *qname, zorba::ItemFactory *ifactory, const char *rname, bool deterministic) 
        :  RXQueryFunction(qname, ifactory, deterministic),
           _rname(rname)
       { }

protected:
  virtual SEXP getRFunctionSEXP() const {return(Rf_install(_rname));}
};



class RXQueryClosureFunction : public  RXQueryFunction {
protected:
  SEXP _rfunc;

public:
    RXQueryClosureFunction(const char *qname, zorba::ItemFactory *ifactory, SEXP func, bool deterministic)  
      :  RXQueryFunction(qname, ifactory, deterministic), _rfunc(func)
       { }

protected:
   virtual SEXP getRFunctionSEXP() const {return(_rfunc);}
};

