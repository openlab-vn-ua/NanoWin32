// NanoWin32
// -----------------------------------------------------------------------
// Simple library to subset Win32(64) API functions implemenation on POSIX
// This software distributed by MIT license

// MSVC-compatible wprintf/wcsanf format support
// (removes differences between processing %s,%c in "wide" printf functions between MSVC and GLIBC)
// Remapping code (if any need besides header)

#if defined(__GNUC__)

// So far all logic in header

#endif // __GNUC__
