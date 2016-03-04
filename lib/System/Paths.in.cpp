// Copyright (c) 2013-2016 Josh Blum
// SPDX-License-Identifier: BSL-1.0

#include <Pothos/System/Paths.hpp>
#include <Pothos/System/Exception.hpp>
#include <Poco/Environment.h>
#include <Poco/Path.h>
#include <Poco/Foundation.h>

std::string Pothos::System::getRootPath(void)
{
    if (Poco::Environment::has("POTHOS_ROOT"))
        return Poco::Path(Poco::Environment::get("POTHOS_ROOT")).absolute().toString();

    // Get the path to the current dynamic linked library.
    // The path to this library can be used to determine
    // the installation root without prior knowledge.
    #if defined(POCO_OS_FAMILY_WINDOWS)
    char path[MAX_PATH];
    HMODULE hm = NULL;
    if (GetModuleHandleExA(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCSTR) &Pothos::System::getRootPath, &hm))
    {
        const DWORD size = GetModuleFileNameA(hm, path, sizeof(path));
        if (size != 0)
        {
            const Poco::Path libPath(std::string(path, size), Poco::Path::PATH_WINDOWS);
            return libPath.parent().popDirectory().toString();
        }
    }
    #endif

    //assume that the root path is always in UNIX style
    const Poco::Path configPrefix = Poco::Path("@POTHOS_ROOT@", Poco::Path::PATH_UNIX);
    return configPrefix.absolute().toString();
}

std::string Pothos::System::getDataPath(void)
{
    Poco::Path dataPath(getRootPath());
    dataPath.append("share");
    dataPath.append("Pothos");
    return dataPath.absolute().toString();
}

std::string Pothos::System::getUserDataPath(void)
{
    Poco::Path dataPath;
    if (Poco::Environment::has("XDG_DATA_HOME"))
    {
        dataPath = Poco::Path(Poco::Environment::get("XDG_DATA_HOME"));
    }
    else if (Poco::Environment::has("APPDATA"))
    {
        dataPath = Poco::Path(Poco::Environment::get("APPDATA"));
    }
    else
    {
        dataPath = Poco::Path::home();
        dataPath.append(".local");
        dataPath.append("share");
    }
    dataPath.append("Pothos");
    return dataPath.absolute().toString();
}

std::string Pothos::System::getUserConfigPath(void)
{
    Poco::Path configPath;
    if (Poco::Environment::has("XDG_CONFIG_HOME"))
    {
        configPath = Poco::Path(Poco::Environment::get("XDG_CONFIG_HOME"));
    }
    else if (Poco::Environment::has("APPDATA"))
    {
        configPath = Poco::Path(Poco::Environment::get("APPDATA"));
    }
    else
    {
        configPath = Poco::Path::home();
        configPath.append(".config");
    }
    configPath.append("Pothos");
    return configPath.absolute().toString();
}

std::string Pothos::System::getPothosUtilExecutablePath(void)
{
    Poco::Path utilPath(Pothos::System::getRootPath());
    utilPath.append("bin");
    utilPath.append("PothosUtil");
    #if defined(POCO_OS_FAMILY_WINDOWS)
    utilPath.setExtension("exe");
    #endif

    return utilPath.absolute().toString();
}

std::string Pothos::System::getPothosRuntimeLibraryPath(void)
{
    Poco::Path dllPath(Pothos::System::getRootPath());
    #if defined(POCO_OS_FAMILY_WINDOWS)
    dllPath.append("bin");
    #elif defined(POCO_OS_FAMILY_UNIX)
    dllPath.append("lib@LIB_SUFFIX@");
    #endif
    dllPath.append(POTHOS_LIBRARY_NAME);

    return dllPath.absolute().toString();
}

std::string Pothos::System::getPothosDevIncludePath(void)
{
    Poco::Path incPath(Pothos::System::getRootPath());
    incPath.append("include");
    return incPath.absolute().toString();
}

std::string Pothos::System::getPothosDevLibraryPath(void)
{
    Poco::Path libPath(Pothos::System::getRootPath());
    libPath.append("lib@LIB_SUFFIX@");
    return libPath.absolute().toString();
}
