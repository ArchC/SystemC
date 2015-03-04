#if defined(sparc)
#include <qt/md/sparc.h>
#elif defined(__hppa)
#include <qt/md/hppa.h>
#elif defined(i386)
#include <qt/md/i386.h>
#else
#error "Don't have support for your architecture (yet)"
#endif
