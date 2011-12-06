#include <zorba/zorba.h>

#include <Rdefines.h>

SEXP makeExternalRObject(const void *val, const char * const className);

#define R_GET_REF(x, type) \
  (type *) R_ExternalPtrAddr(GET_SLOT(x, Rf_install("ref")))

// Not certain if this is useful, works, ...
#define R_GET_REF_T(x, type) \
  (type) R_ExternalPtrAddr(GET_SLOT(x, Rf_install("ref")))

SEXP convertItemToR(zorba::Item item, bool asXMLSubNode = false);
SEXP R_getQueryResults(zorba::XQuery_t lQuery, double intSize = 10., double factor = 1.5);


SEXP R_createRXQueryObject(zorba::XQuery_t q);
SEXP R_createRStaticContextObject(zorba::StaticContext_t sctx);


zorba::Item convertRScalarToItem(SEXP ans, zorba::ItemFactory* lFactory);
zorba::ItemSequence_t convertRToXQuery(SEXP ans, zorba::ItemFactory *factory);


typedef std::vector<zorba::Item> iv_t;
typedef iv_t::iterator ii_t;

class IteratorBackedItemSequence : public zorba::ItemSequence {
 public:
  IteratorBackedItemSequence(iv_t& vec)
    : m_vec(vec),
    m_i(m_vec.begin()),
    m_end(m_vec.end()) { }

  bool next(zorba::Item& val)
  {
    if (m_i == m_end) {
      return false;
    }
    val = *m_i;
    ++m_i;
    return true;
  }

 private:
  iv_t m_vec;
  ii_t m_i;
  ii_t m_end;
};


void raiseZorbaError(zorba::QueryException &e);
void raiseZorbaError(zorba::ZorbaException &e);
void raiseZorbaWarning(zorba::ZorbaException &e);


SEXP makeXMLNode(zorba::Item it, bool ref = true);
SEXP makeXMLNodeRef(zorba::Item &it);

