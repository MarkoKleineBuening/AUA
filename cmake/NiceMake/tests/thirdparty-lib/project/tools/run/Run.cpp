#include <iostream>
#include <cstdlib>

#include <testsharedlib/Shared.h>
#include <testsharedlib/objsharedlib/ObjShared.h>
#include <teststaticlib/Static.h>
#include <teststaticlib/objlib/Obj.h>

#include <ModernThirdParty.h>
#include <TraditionalThirdParty.h>


#if !defined(HAVE_MODERN_TPL)
  #error "Missing imported compiler flags of modern-thirdparty-src"
#endif

#if !defined(HAVE_TRADITIONAL_TPL)
  #error "Missing imported compiler flags of traditional-thirdparty-src"
#endif


int main(int argc, char **argv) {
  std::cout << (getMagicNumberShared() + getMagicNumberObjShared()
    + getMagicNumberStatic() + getMagicNumberObj()) << getMagicNumTraditional()
    << getMagicNumModern();
  return EXIT_SUCCESS;
}
