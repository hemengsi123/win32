
# win32 学习

readme.md 文件 [编写规则](https://help.github.com/articles/markdown-basics/)
# 1. char wchar_t TCHAR 介绍
#### // add 2015.01.08

<<<<<<< HEAD
由于字符编码的不同，在C++中有三种对于字符类型：char, wchar_t , TCHAR；
=======
由于字符编码的不同，在C++中有三种对于字符类型：char, wchar_t , TCHAR。
>>>>>>> tmp
TCHAR不能算作一种类型，他紧紧是一个宏, 在使用多字节编码时被定义成char，在Unicode编码时定义成wchar_t
```
type                Meaning in MBCS builds          Meaning in Unicode builds
WCHAR                 wchar_t                             wchar_t
<<<<<<< HEAD
LPSTR                 char*                                char*
LPCSTR                const char*                        const char*
LPWSTR                wchar_t*                           wchar_t*
LPCWSTR               const wchar_t*                    const wchar_t* 
TCHAR                 char                                  wchar_t
LPTSTR                TCHAR*                            TCHAR*
LPCTSTR               const TCHAR*                     const TCHAR* 
=======
LPSTR                 char*                               char*
LPCSTR                const char*                         const char*
LPWSTR                wchar_t*                            wchar_t*
LPCWSTR               const wchar_t*                      const wchar_t* 
TCHAR                 char                                wchar_t
LPTSTR                TCHAR*                              TCHAR*
LPCTSTR               const TCHAR*                        const TCHAR* 
>>>>>>> tmp

常用的字符串处理函数和宏:
1、str 开头的 函数 处理SBCS字符串
2、wcs 开头的 函数 处理宽字符串，wcs是宽字符串的英文缩写 \#include <wchar.h>
   所有的unicode函数均以wcs开头。若要调用Unicode函数，只需用前缀wcs来取代ANSI字符串函数的前缀str即可。
   对于每一个标准的ANSI C字符串函数，基本都有等价的unicode函数.
<<<<<<< HEAD
3、_mbs 开头的 函数 处理DBCS字符串
=======
3、_mbs 开头的 函数 处理DBCS（双字节字符集）字符串
>>>>>>> tmp
4、_tcs 开头的 宏 配合TCHAR使用 #include <tchar.h>
   根据预定义分别扩展为str wcs _mbs， 见后面的描述。
5、l开头的windows自带的宽字符处理函数
6、大小写兼有的 windows自带的宽字符处理函数

<<<<<<< HEAD
=======
###### L 与 _T 区别
L"" 转换成unicode的字符串
_T 根据是否定义了_UNICODE，选择ANCI或UNICODE字符集(_TEXT同其一样)

###### 在C语言中，如何输出百分号呢？
只要在格式控制符里输入2个 %% 就可以了: printf("%d%%\n",10); \\10%

###### INT 与 DWORD 区别
INT 代表int，int会随着机器位数的不同而发生变化，比如在16位机上为16为，在32位机上为32位，在64位机上为64位。
DWORD 代表 unsigned long
```
#### add 2015.01.09
###### [代码签名](http://blog.itnmg.net/free-code-signing-certificates/) | [创建pfx数字证书](http://stackoverflow.com/questions/16082333/why-i-get-the-specified-pfx-password-is-not-correct-when-trying-to-sign-applic)
```
第一步，生成一个自签名的根证书(issuer,签发者)(在弹出的的对话框中填入密码123456，第二个弹出框填入相同密码123456)
	makecert.exe -n "CN=Simon test" -sv ..\simonTestKey.pvk ..\simonTestKey.cer -r 
第二步 创建发行者证书 （.spc 意思是 软件发布者证书（Software Pulisher Cerificate））
	Cert2Spc.exe ..\simonTestKey.cer ..\simonTestKey.spc
第三步，将公钥证书和私钥合并成一个PFX格式的证书文件。pvk2pfx.exe （在弹出框中输入上面设置的密码123456）
	pvk2pfx.exe -pvk ..\simonTestKey.pvk -spc ..\simonTestKey.spc -pfx ..\simonTestKey.pfx -po "123456"
也可以跳过第二步，直接操作第三步，生成pfx文件
	pvk2pfx.exe -pvk ..\simonTestKey.pvk -spc ..\simonTestKey.cer -pfx ..\simonTestKey.pfx -po "123456"
第四步，给二进制文件数字签名。signtool.exe
	signtool.exe sign /d "code sign test" /f "..\simonTestKey.pfx" /p "123456" /t "http://timestamp.verisign.com/scripts/timstamp.dll" /ph "E:\Simon\projects\win32\test1\bin\test1.exe"
signtool.exe sign /d "本次签名的描述" /du "为已签名文档的详细说明指定统一资源定位器 (URL)" /f 证书全名 /p 证书密码 /t 时间戳服务器 /ph 要签名的文件全名
例子：
signtool.exe sign /d "ExtLibrary by www.itnmg.net" /du "http://www.itmg.net/extlibrary" /f ..\simonTestKey.pfx /p ****** /t http://timestamp.verisign.com/scripts/timstamp.dll /ph extlibrary.dll
	
>>>>>>> tmp
```