#pragma once
#if !defined(LEKMOD_MACOS) || !defined(__APPLE__)
#error "The macOS Windows.h shim must not be used by a Windows build"
#endif
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cerrno>
#include <climits>
#include <strings.h>
using BYTE=uint8_t; using WORD=uint16_t; using DWORD=uint32_t; using UINT=uint32_t;
using LONG=int32_t; using ULONG=uint32_t; using BOOL=int32_t; using CHAR=char; using WCHAR=wchar_t;
using SHORT=int16_t; using USHORT=uint16_t; using INT=int; using UCHAR=unsigned char;
using INT8=int8_t; using INT16=int16_t; using INT32=int32_t; using INT64=long long;
using UINT8=uint8_t; using UINT16=uint16_t; using UINT32=uint32_t; using UINT64=unsigned long long;
using LONGLONG=long long; using ULONGLONG=unsigned long long; using ULONG_PTR=uintptr_t; using DWORD_PTR=uintptr_t;
using INT_PTR=intptr_t; using UINT_PTR=uintptr_t; using SIZE_T=size_t;
using HANDLE=void*; using HMODULE=void*; using HINSTANCE=void*; using HWND=void*;
using LPVOID=void*; using LPCVOID=const void*; using LPSTR=char*; using LPCSTR=const char*;
using LPWSTR=wchar_t*; using LPCWSTR=const wchar_t*; using LPDWORD=DWORD*;
using HRESULT=int32_t;
using uint=unsigned int;
// The SDK only exposes this member when embedding Windows critical sections;
// Mac-specific users will be adapted before linking against engine objects.
struct CRITICAL_SECTION { void* opaque[5]; };
struct GUID { uint32_t Data1; uint16_t Data2,Data3; uint8_t Data4[8]; };
using REFGUID=const GUID&;
inline bool operator==(const GUID& a,const GUID& b) { return memcmp(&a,&b,16)==0; }
inline bool operator!=(const GUID& a,const GUID& b) { return !(a==b); }
union LARGE_INTEGER { struct { uint32_t LowPart; int32_t HighPart; }; int64_t QuadPart; };
union ULARGE_INTEGER { struct { uint32_t LowPart,HighPart; }; uint64_t QuadPart; };
struct FILETIME { DWORD dwLowDateTime,dwHighDateTime; };
struct RECT { LONG left,top,right,bottom; };
struct POINT { LONG x,y; };
#define WINAPI
#define APIENTRY
#define CALLBACK
#define TRUE 1
#define FALSE 0
#define MAX_PATH 260
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define ZeroMemory(p,n) memset(p,0,n)
#define CopyMemory(d,s,n) memcpy(d,s,n)
#define MoveMemory(d,s,n) memmove(d,s,n)
#define CP_UTF8 65001
#define CP_ACP 0
#define _countof(a) (sizeof(a)/sizeof((a)[0]))
#define _In_
#define _Out_
#define _Inout_
#define _In_opt_
#define _Out_opt_
#define _Inout_opt_
#define _In_z_
#define _Out_z_
#define _Check_return_
#define _Success_(x)
#define _In_reads_(x)
#define _In_reads_bytes_(x)
#define _Out_writes_(x)
#define _Out_writes_bytes_(x)
#define _Out_writes_z_(x)
#define _Printf_format_string_
#define _Ret_maybenull_
#define _Ret_notnull_
#define _Inout_z_cap_c_(x)
#define _Inout_z_cap_(x)
#define DECLSPEC_DEPRECATED
#define _Ret_opt_
#define _Ret_z_
#define _Ret_opt_z_
#define __checkReturn
#define _In_opt_z_
#define _In_bytecount_(x)
struct EXCEPTION_POINTERS;
struct SYSTEMTIME { WORD wYear,wMonth,wDayOfWeek,wDay,wHour,wMinute,wSecond,wMilliseconds; };
extern "C" {
HANDLE HeapCreate(DWORD,size_t,size_t);
BOOL HeapDestroy(HANDLE);
void* HeapAlloc(HANDLE,DWORD,size_t);
BOOL HeapFree(HANDLE,DWORD,void*);
BOOL HeapSetInformation(HANDLE,int,void*,size_t);
BOOL CloseHandle(HANDLE);
BOOL QueryPerformanceCounter(LARGE_INTEGER*);
BOOL QueryPerformanceFrequency(LARGE_INTEGER*);
DWORD GetLastError();
void OutputDebugStringA(const char*);
HRESULT CoCreateGuid(GUID*);
int MultiByteToWideChar(UINT,DWORD,const char*,int,wchar_t*,int);
}
#define OutputDebugString OutputDebugStringA
inline LONG InterlockedIncrement(volatile LONG* p) { return __sync_add_and_fetch(p,1); }
inline LONG InterlockedDecrement(volatile LONG* p) { return __sync_sub_and_fetch(p,1); }
inline LONG InterlockedCompareExchange(volatile LONG* p,LONG v,LONG c) { return __sync_val_compare_and_swap(p,c,v); }
inline LONG InterlockedExchangeAdd(volatile LONG* p,LONG v) { return __sync_fetch_and_add(p,v); }
inline LONG InterlockedExchange(volatile LONG* p,LONG v) { return __sync_lock_test_and_set(p,v); }
inline void* InterlockedCompareExchangePointer(void* volatile* p,void* v,void* c) { return __sync_val_compare_and_swap(p,c,v); }
constexpr DWORD GENERIC_READ=0x80000000u, FILE_SHARE_READ=1, FILE_SHARE_WRITE=2;
constexpr DWORD OPEN_EXISTING=3, FILE_FLAG_SEQUENTIAL_SCAN=0x08000000u;
constexpr DWORD INVALID_FILE_SIZE=0xffffffffu, INVALID_FILE_ATTRIBUTES=0xffffffffu;
constexpr DWORD ERROR_FILE_NOT_FOUND=2;
constexpr int GetFileExInfoStandard=0, HeapCompatibilityInformation=0;
struct WIN32_FILE_ATTRIBUTE_DATA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime,ftLastAccessTime,ftLastWriteTime;
    DWORD nFileSizeHigh,nFileSizeLow;
};
extern "C" {
BOOL DeleteFileW(const wchar_t*);
HANDLE CreateFileW(const wchar_t*,DWORD,DWORD,void*,DWORD,DWORD,HANDLE);
DWORD GetFileSize(HANDLE,DWORD*);
BOOL ReadFile(HANDLE,void*,DWORD,DWORD*,void*);
BOOL GetFileAttributesExW(const wchar_t*,int,void*);
}
inline LONG CompareFileTime(const FILETIME* a,const FILETIME* b) {
    uint64_t av=(uint64_t(a->dwHighDateTime)<<32)|a->dwLowDateTime;
    uint64_t bv=(uint64_t(b->dwHighDateTime)<<32)|b->dwLowDateTime;
    return (av>bv)-(av<bv);
}
