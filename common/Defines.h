#pragma once
#ifndef _DEFINES_H_
#define _DEFINES_H_

#ifdef WIN32
#ifdef NPP_WIN32_DLL_IMPORT

#define NPP_DLL_DECL __declspec(dllimport)
#else
#ifdef  NPP_WIND_DLL_EXPORT
#define NPP_DLL_DECL __declspec(dllexport)
#else
#define NPP_DLL_DECL
#endif
#endif
#endif

#if defined( _MSC_VER )
typedef __int64            int64;
typedef long               int32;
typedef short              int16;
typedef signed char        int8;
typedef unsigned __int64   uint64;
typedef unsigned long      uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
#else
//typedef long long int64;
typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;
typedef uint16      WORD;
typedef uint32      DWORD;
#endif

#endif // _DEFINES_H_

