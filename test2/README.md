```
#ifdef __LINUX_CODE__
#include <sys/syscall.h>  // __NR_gettid
#define gettid() syscall(__NR_gettid)
#else
#define gettid() ::GetCurrentThreadId()
#endif

#if defined( _MSC_VER ) & defined( _MT )
# include <windows.h>
#endif
```