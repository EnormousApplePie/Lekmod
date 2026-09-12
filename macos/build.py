#!/usr/bin/env python3
"""Build the x86_64 macOS gameplay library against the installed Civ V host."""
import argparse
import concurrent.futures
import os
import json
from pathlib import Path
import subprocess
import sys
import xml.etree.ElementTree as ET
from audit import check_imports

HERE = Path(__file__).resolve().parent
CORE = HERE.parent / "LEKMOD_DLL/CvGameCoreDLL_Expansion2"
BUILD = HERE / "build"
INCLUDES = [HERE / "include", CORE, CORE / "CvGameCoreDLLUtil/include",
            CORE / "CvWorldBuilderMap/include", CORE / "CvGameDatabase/include",
            CORE / "CvLocalization/include", CORE / "FirePlace/include",
            CORE / "FirePlace/include/FireWorks", CORE / "ThirdPartyLibs/Lua51/include"]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--release", action="store_true", help="Build optimized gameplay code")
    parser.add_argument("--check", action="store_true", help="Check the common headers only")
    parser.add_argument("--source", help="Compile one source relative to the core directory")
    parser.add_argument("--jobs", type=int, default=4)
    args = parser.parse_args()
    BUILD.mkdir(exist_ok=True)
    flags = ["clang++", "-arch", "x86_64", "-mmacosx-version-min=10.13",
             "-std=c++14", "-stdlib=libc++", "-fms-extensions", "-fdeclspec", "-fdelayed-template-parsing",
             "-fno-strict-aliasing", "-fwrapv", "-ffp-contract=off", "-O2" if args.release else "-O0", "-g",
             "-fvisibility=hidden", "-fvisibility-inlines-hidden",
             "-DFINAL_RELEASE", "-DNDEBUG", "-DFXS_IS_DLL", "-DLEKMOD_MACOS",
             "-Wno-unknown-pragmas", "-Wno-ignored-attributes", "-Wno-deprecated-declarations",
             "-Wno-microsoft-template", "-Wno-microsoft-cast", "-Wno-macro-redefined", "-ferror-limit=30",
             "-include", str(HERE / "include/native.hpp")]
    flags += ["-I" + str(p) for p in INCLUDES]
    if args.check:
        return subprocess.run(flags + ["-fsyntax-only", "-x", "c++", "-"],
                              input='#include "CvGameCoreDLLPCH.h"\n', text=True).returncode
    project = ET.parse(CORE / "CvGameCoreDLL_Expansion2.vcxproj")
    sources = [e.attrib["Include"] for e in project.findall(".//{*}ClCompile") if "Include" in e.attrib]
    if args.source:
        sources = [args.source]
    header_time = max(p.stat().st_mtime for root in [CORE, HERE / "include"]
                      for p in root.rglob("*") if p.is_file() and p.suffix in (".h", ".hpp", ".inl", ""))
    header_time = max(header_time, Path(__file__).stat().st_mtime)
    def compile_source(name):
        src = CORE / name.replace("\\", "/")
        obj = BUILD / (src.stem + ".o")
        command = flags + ["-c", str(src), "-o", str(obj)]
        stamp = obj.with_suffix(".command.json")
        if (obj.exists() and stamp.exists() and json.loads(stamp.read_text()) == command
                and obj.stat().st_mtime > max(src.stat().st_mtime, header_time)):
            return name, 0
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode == 0:
            stamp.write_text(json.dumps(command))
        (BUILD / (src.stem + ".log")).write_text(result.stdout + result.stderr)
        return name, result.returncode
    failed = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        for name, rc in pool.map(compile_source, sources):
            print(("FAIL " if rc else "OK   ") + name, flush=True)
            if rc:
                failed.append(name)
    if failed:
        print(f"{len(failed)}/{len(sources)} sources failed; diagnostics in {BUILD}")
        return 1
    if args.source:
        return 0
    candidate = BUILD / "candidate.dylib"
    objects = [BUILD / (Path(name.replace("\\", "/")).stem + ".o") for name in sources]
    # Engine imports resolve from the game executable; audit them before replacing the output.
    link = subprocess.run(["clang++", "-arch", "x86_64", "-mmacosx-version-min=10.13",
                           "-dynamiclib", "-stdlib=libc++", "-Wl,-undefined,dynamic_lookup",
                           "-Wl,-exported_symbol,_DllGetGameContext",
                           "-Wl,-install_name,@executable_path/libCvGameCoreDLL_Expansion2_DLL.dylib",
                           *map(str, objects), "-o", str(candidate)], capture_output=True, text=True)
    (BUILD / "link.log").write_text(link.stdout + link.stderr)
    if link.returncode:
        print(link.stderr)
        return link.returncode
    try:
        check_imports(candidate)
    except RuntimeError as error:
        print(error)
        return 1
    output = BUILD / "libCvGameCoreDLL_Expansion2_DLL.dylib"
    candidate.replace(output)
    print(f"Linked and audited {output}")
    return 0

if __name__ == "__main__":
    sys.exit(main())
