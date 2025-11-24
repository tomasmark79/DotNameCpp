#ifndef WINDOWS_HEADERS_HPP
#define WINDOWS_HEADERS_HPP

// Universal Windows.h wrapper with Raylib compatibility
// Include this instead of <windows.h> directly to avoid conflicts

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
// Disable min/max macros in windows.h to avoid conflicts with std::min/max
#endif

// Protect Raylib functions from Windows.h macro conflicts
#define Rectangle WindowsRectangle
#define CloseWindow WindowsCloseWindow
#define ShowCursor WindowsShowCursor
#define DrawText WindowsDrawText
#define PlaySound WindowsPlaySound
#define LoadImage WindowsLoadImage
#define DrawTextEx WindowsDrawTextEx

#include <windows.h>

// Restore Raylib function names
#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef DrawText
#undef PlaySound
#undef LoadImage
#undef DrawTextEx

#endif // _WIN32

#endif // WINDOWS_HEADERS_HPP
