
# win32 学习

readme.md 文件 [编写规则](https://help.github.com/articles/markdown-basics/)
# 1. char wchar_t TCHAR 介绍
#### // add 2015.01.08

由于字符编码的不同，在C++中有三种对于字符类型：char, wchar_t , TCHAR。
TCHAR不能算作一种类型，他紧紧是一个宏, 在使用多字节编码时被定义成char，在Unicode编码时定义成wchar_t
```
type                Meaning in MBCS builds          Meaning in Unicode builds
WCHAR                 wchar_t                             wchar_t
LPSTR                 char*                               char*
LPCSTR                const char*                         const char*
LPWSTR                wchar_t*                            wchar_t*
LPCWSTR               const wchar_t*                      const wchar_t* 
TCHAR                 char                                wchar_t
LPTSTR                TCHAR*                              TCHAR*
LPCTSTR               const TCHAR*                        const TCHAR* 

常用的字符串处理函数和宏:
1、str 开头的 函数 处理SBCS字符串
2、wcs 开头的 函数 处理宽字符串，wcs是宽字符串的英文缩写 \#include <wchar.h>
   所有的unicode函数均以wcs开头。若要调用Unicode函数，只需用前缀wcs来取代ANSI字符串函数的前缀str即可。
   对于每一个标准的ANSI C字符串函数，基本都有等价的unicode函数.
3、_mbs 开头的 函数 处理DBCS（双字节字符集）字符串
4、_tcs 开头的 宏 配合TCHAR使用 #include <tchar.h>
   根据预定义分别扩展为str wcs _mbs， 见后面的描述。
5、l开头的windows自带的宽字符处理函数
6、大小写兼有的 windows自带的宽字符处理函数

###### L 与 _T 区别
L"" 转换成unicode的字符串
_T 根据是否定义了_UNICODE，选择ANCI或UNICODE字符集(_TEXT同其一样)

###### 在C语言中，如何输出百分号呢？
只要在格式控制符里输入2个 %% 就可以了: printf("%d%%\n",10); \\10%

###### INT 与 DWORD 区别
INT 代表int，int会随着机器位数的不同而发生变化，比如在16位机上为16为，在32位机上为32位，在64位机上为64位。
DWORD 代表 unsigned long
```