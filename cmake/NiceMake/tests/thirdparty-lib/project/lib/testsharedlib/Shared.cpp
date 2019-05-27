#include <testsharedlib/Shared.h>

#include <ModernThirdParty.h>
#include <TraditionalThirdParty.h>

#if !defined(HAVE_MODERN_TPL)
  #error "Missing imported compiler flags of modern-thirdparty-src"
#endif

#if !defined(HAVE_TRADITIONAL_TPL)
  #error "Missing imported compiler flags of traditional-thirdparty-src"
#endif

int getMagicNumberShared() {
  return 1 * (getMagicNumTraditional() + getMagicNumModern());
}
