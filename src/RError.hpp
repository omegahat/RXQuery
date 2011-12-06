
#include <zorba/exception.h>

class RXQueryError : public zorba::DynamicException
{
  public:
    RXQueryError() : DynamicException(XPDY0021,  zorba::String(""), zorba::String(""),  0, zorba::String(""), 0, 0){};
      
    RXQueryError(const char *msg) :
         DynamicException(XPDY0021,  zorba::String(msg), zorba::String(""),  0, zorba::String(""), 0, 0)
        {}
};


