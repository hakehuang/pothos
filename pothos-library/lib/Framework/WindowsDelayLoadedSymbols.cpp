// Copyright (c) 2013-2014 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include <Pothos/Config.hpp>
#include <windows.h>
#include <iostream>
#include <cassert>

/***********************************************************************
 * delay load the kernel32 library
 **********************************************************************/
static FARPROC GetKernelProcAddress(LPCSTR lpProcName)
{
    static HMODULE hKernel = LoadLibrary("kernel32.dll");
    assert(hKernel);
    FARPROC r = GetProcAddress(hKernel, lpProcName);
    if (not r) std::cerr << "This kernel32 does not support " << lpProcName << std::endl;
    return r;
}

/***********************************************************************
 * numa information
 **********************************************************************/
BOOL DL_GetNumaAvailableMemoryNodeEx(USHORT Node, PULONGLONG AvailableBytes)
{
    typedef BOOL (WINAPI * GetNumaAvailableMemoryNodeEx_t)(USHORT, PULONGLONG);
    static auto fcn = (GetNumaAvailableMemoryNodeEx_t)GetKernelProcAddress("GetNumaAvailableMemoryNodeEx");
    if (not fcn) return false;
    return fcn(Node, AvailableBytes);
}

BOOL DL_GetNumaNodeProcessorMaskEx(USHORT Node, PGROUP_AFFINITY ProcessorMask)
{
    typedef BOOL (WINAPI * GetNumaNodeProcessorMaskEx_t)(USHORT, PGROUP_AFFINITY);
    static auto fcn = (GetNumaNodeProcessorMaskEx_t)GetKernelProcAddress("GetNumaNodeProcessorMaskEx");
    if (not fcn) return false;
    return fcn(Node, ProcessorMask);
}

/***********************************************************************
 * numa allocation -- with non-numa backup support
 **********************************************************************/
LPVOID DL_VirtualAllocExNuma(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect, DWORD nndPreferred)
{
    typedef LPVOID (WINAPI * VirtualAllocExNuma_t)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD, DWORD);
    static auto fcn = (VirtualAllocExNuma_t)GetKernelProcAddress("VirtualAllocExNuma");
    if (not fcn) return VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
    return fcn(hProcess, lpAddress, dwSize, flAllocationType, flProtect, nndPreferred);
}

HANDLE DL_CreateFileMappingNuma(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCTSTR lpName, DWORD nndPreferred)
{
    typedef HANDLE (WINAPI * CreateFileMappingNuma_t)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCTSTR, DWORD);
    static auto fcn = (CreateFileMappingNuma_t)GetKernelProcAddress("CreateFileMappingNumaA");
    if (not fcn) return CreateFileMapping(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
    return fcn(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName, nndPreferred);
}

LPVOID DL_MapViewOfFileExNuma(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress, DWORD nndPreferred)
{
    typedef LPVOID (WINAPI * MapViewOfFileExNuma_t)(HANDLE, DWORD, DWORD, DWORD, SIZE_T, LPVOID, DWORD);
    static auto fcn = (MapViewOfFileExNuma_t)GetKernelProcAddress("MapViewOfFileExNuma");
    if (not fcn) return MapViewOfFileEx(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap, lpBaseAddress);
    return fcn(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap, lpBaseAddress, nndPreferred);
}
