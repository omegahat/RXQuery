#include <zorba/zorba.h>
#include <simplestore/simplestore.h>

using namespace zorba;


int
main(int argc, char *argv[])
{

  simplestore::SimpleStore* lStore = simplestore::SimpleStoreManager::getStore();
  Zorba *xqEngine = Zorba::getInstance(lStore);

  XQuery_t q = xqEngine->compileQuery("1+2");

  return(0);
}
