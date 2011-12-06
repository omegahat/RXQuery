#include <zorba/version.h>
#include <Rdefines.h>

extern "C"
SEXP
R_zorba_version()
{
  SEXP ans;
  PROTECT(ans = NEW_INTEGER(3));
  INTEGER(ans)[0] = zorba::Version::getMajorVersion();
  INTEGER(ans)[1] = zorba::Version::getMinorVersion();
  INTEGER(ans)[2] = zorba::Version::getPatchVersion();
  UNPROTECT(1);
  return(ans);
}
