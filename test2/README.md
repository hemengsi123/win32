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


unsigned int threadIDIn = 0;
unsigned int threadIDOut = 0;

//hThreadIn = (HANDLE)_beginthreadex( NULL, 0, data_swapin_thread, (void*)&node, 0, &threadIDIn );
//hThreadOut = (HANDLE)_beginthreadex( NULL, 0, data_swapout_thread, (void*)&node, 0, &threadIDOut );

CloseHandle((HANDLE)_beginthreadex( NULL, 0, data_swapin_thread, (void*)param, 0, &threadIDIn ));
CloseHandle((HANDLE)_beginthreadex( NULL, 0, (unsigned int (__stdcall *)( void * ))data_swapout_thread, (void*)param, 0, &threadIDOut ));
```