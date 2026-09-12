#pragma once
#include "FMemoryStream.h"
#include "CvDllContext.h"
#include "DatabaseResults.h"
#include "CvLocalization.h"
// Sizes reconstructed from the installed Aspyr 180925 x86_64 executable.
static_assert(sizeof(void*) == 8, "Mac engine requires x86_64 pointers");
static_assert(sizeof(wchar_t) == 4, "Mac engine uses native UTF-32 wchar_t");
static_assert(sizeof(dword) == 4, "CRC and engine DWORDs must remain 32-bit");
static_assert(sizeof(GUID) == 16, "Game interface GUID layout");
static_assert(sizeof(FString) == 8, "Engine FString layout");
static_assert(sizeof(Database::Results) == 112, "Engine database results layout");
static_assert(sizeof(Localization::String) == 144, "Engine localization layout");
static_assert(sizeof(FDataStream) == 24, "Engine stream base layout");
static_assert(sizeof(FMemoryStream) == 72, "Engine memory stream layout");
static_assert(sizeof(CvDllGameContext) == 136, "Game context layout");
