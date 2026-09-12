#pragma once
#include <mach-o/dyld.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

namespace LekmodMac {

// Aspyr 180925's network initializer passes a writable local std::string to
// pre-game slot 185 at 0x100781e9e, then passes that SAME local to the host's
// registration setter at 0x100781ea8. Limit this adaptation to that exact caller;
// normal callers of the const-reference API must never have their input changed.
inline void adaptRegistration(const std::string& version, const void* caller)
{
    const uintptr_t callReturn = 0x100781ea4ULL + _dyld_get_image_vmaddr_slide(0);
    if (reinterpret_cast<uintptr_t>(caller) != callReturn)
        return;
    const unsigned char callSequence[] = {
        0xff, 0x90, 0xc8, 0x05, 0x00, 0x00, // call pre-game slot 185
        0x48, 0x8d, 0x7d, 0x80,             // address of the same stack string
        0xe8, 0x23, 0x42, 0x36, 0x00        // call host registration setter
    };
    if (std::memcmp(reinterpret_cast<const void*>(callReturn - 6),
                    callSequence, sizeof(callSequence)) != 0)
        return;
    if (version != "180925 FINAL_RELEASE")
        return;

    const char* image = _dyld_get_image_name(0);
    if (!image)
        return;
    std::string path(image);
    const std::string suffix("/Contents/MacOS/Civilization V");
    if (path.size() < suffix.size()
        || path.compare(path.size() - suffix.size(), suffix.size(), suffix) != 0)
        return;
    path.resize(path.size() - suffix.size());
    path += "/Contents/Resources/lekmod-crossplay.txt";
    FILE* flag = std::fopen(path.c_str(), "r");
    if (!flag)
        return;
    char setting[64] = {};
    const bool enabled = std::fgets(setting, sizeof(setting), flag)
        && std::strcmp(setting, "403694 FINAL_RELEASE\n") == 0;
    std::fclose(flag);
    if (!enabled)
        return;

    // The inspected caller constructs a non-const local. Its six-digit build
    // prefix can be replaced in place without allocation or changing its ABI.
    std::string& writable = const_cast<std::string&>(version);
    for (unsigned i = 0; i < 6; ++i)
        writable[i] = "403694"[i];
    std::fprintf(stderr, "LEKMOD_MACOS_REGISTRATION_V2: %s\n", version.c_str());
}

} // namespace LekmodMac
