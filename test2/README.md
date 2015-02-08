```
#ifdef __LINUX_CODE__
#include <sys/syscall.h>  // __NR_gettid
#define gettid() syscall(__NR_gettid)
#else
#define gettid() ::GetCurrentThreadId()
#endif
```