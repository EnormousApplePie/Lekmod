#pragma once
// The Mac game exports native implementations of selected Win32/CRT functions.
// Keep these declarations separate from the Windows SDK used by the PC build.
#include "Windows.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cwchar>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <malloc/malloc.h>
#include <cwctype>
#include <cctype>
#include <memory>
#include <unordered_map>
#include <unordered_set>
namespace std { namespace tr1 { using std::shared_ptr; using std::weak_ptr; using std::unordered_map; using std::unordered_set; } }
// Select the SDK PC paths; LEKMOD_MACOS handles differences from the Windows ABI.
#define _WINPC 1
#define _WIN64 1
#define WIN32 1
#define _CRTIMP
#define _NORMAL_BLOCK 1
#define _DEBUG_LT(a,b) ((a)<(b))
#define __forceinline inline __attribute__((always_inline))
#define __analysis_assume(x) ((void)0)
#define __assume(x) ((void)0)
#define _TRUNCATE static_cast<size_t>(-1)
#define _msize malloc_size
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#define _wcsicmp wcscasecmp
#define _wcsnicmp wcsncasecmp
#define _finite std::isfinite
#define _isnan std::isnan
#define _vsnprintf vsnprintf
#define fprintf_s fprintf
#define _stricoll strcasecmp
#define _wcsicoll wcscasecmp
inline char* _strnset(char* p,int c,size_t n) { for(size_t i=0;i<n&&p[i];++i)p[i]=c; return p; }
inline wchar_t* _wcsnset(wchar_t* p,wchar_t c,size_t n) { for(size_t i=0;i<n&&p[i];++i)p[i]=c; return p; }
inline char* _strupr(char* p) { for(char* q=p;*q;++q)*q=std::toupper(static_cast<unsigned char>(*q)); return p; }
inline char* _strlwr(char* p) { for(char* q=p;*q;++q)*q=std::tolower(static_cast<unsigned char>(*q)); return p; }
inline wchar_t* _wcsupr(wchar_t* p) { for(wchar_t* q=p;*q;++q)*q=std::towupper(*q); return p; }
inline wchar_t* _wcslwr(wchar_t* p) { for(wchar_t* q=p;*q;++q)*q=std::towlower(*q); return p; }
inline char* _strrev(char* p) { std::reverse(p,p+strlen(p)); return p; }
inline wchar_t* _wcsrev(wchar_t* p) { std::reverse(p,p+wcslen(p)); return p; }
#include <FireWorks/FMemHooks.h>
inline int vsprintf_s(char* out, size_t n, const char* fmt, va_list args) { return vsnprintf(out,n,fmt,args); }
inline int sprintf_s(char* out, size_t n, const char* fmt, ...) { va_list a; va_start(a,fmt); int r=vsnprintf(out,n,fmt,a); va_end(a); return r; }
template<size_t N> int sprintf_s(char (&out)[N], const char* fmt, ...) { va_list a; va_start(a,fmt); int r=vsnprintf(out,N,fmt,a); va_end(a); return r; }
inline int strcpy_s(char* out, size_t n, const char* in) { if(strlen(in)>=n) { if(n)out[0]=0; return ERANGE; } strcpy(out,in); return 0; }
template<size_t N> int strcpy_s(char (&out)[N], const char* in) { return strcpy_s(out,N,in); }
inline int strcat_s(char* out, size_t n, const char* in) { return strcpy_s(out+strlen(out),n-strlen(out),in); }
template<size_t N> int strcat_s(char (&out)[N], const char* in) { return strcat_s(out,N,in); }
inline int fopen_s(FILE** f,const char* name,const char* mode) { *f=fopen(name,mode); return *f?0:errno; }
inline int memcpy_s(void* dst,size_t n,const void* src,size_t count) { if(count>n)return ERANGE; memcpy(dst,src,count); return 0; }
#include <array>
namespace std { namespace tr1 { using std::array; } }
#define UNREFERENCED_PARAMETER(x) ((void)(x))
#define _malloca malloc
#define _freea free
#define _strdup strdup
inline int strncpy_s(char* dst,size_t n,const char* src,size_t count) {
    if(!n) return EINVAL;
    size_t len=strnlen(src,count==_TRUNCATE?n-1:count);
    if(len>=n) {dst[0]=0;return ERANGE;}
    memcpy(dst,src,len);dst[len]=0;return 0;
}
inline int wcscpy_s(wchar_t* dst,size_t n,const wchar_t* src) {
    if(wcslen(src)>=n){if(n)dst[0]=0;return ERANGE;}
    wcscpy(dst,src);return 0;
}
template<size_t N> int wcscpy_s(wchar_t (&dst)[N],const wchar_t* src) {return wcscpy_s(dst,N,src);}
inline int _itoa_s(int value,char* dst,size_t n,int radix) {
    if(radix<2 || radix>36 || !n) return EINVAL;
    char buf[34];size_t count=0;
    bool neg=value<0 && radix==10;
    unsigned int mag=neg?0u-static_cast<unsigned int>(value):static_cast<unsigned int>(value);
    do {buf[count++]="0123456789abcdefghijklmnopqrstuvwxyz"[mag%radix];mag/=radix;}while(mag);
    if(neg)buf[count++]='-';
    if(count>=n){dst[0]=0;return ERANGE;}
    for(size_t i=0;i<count;++i)dst[i]=buf[count-1-i];
    dst[count]=0;return 0;
}
template<size_t N> int _itoa_s(int value,char (&dst)[N],int radix) {return _itoa_s(value,dst,N,radix);}
template<size_t N> int vsprintf_s(char (&dst)[N],const char* fmt,va_list args) {return vsnprintf(dst,N,fmt,args);}
#define MAXINT INT_MAX
