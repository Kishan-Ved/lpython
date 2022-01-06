#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>

#include <libasr/exception.h>
#include <libasr/utils.h>
#include <libasr/string_utils.h>

namespace LFortran {

bool read_file(const std::string &filename, std::string &text)
{
    std::ifstream ifs(filename.c_str(), std::ios::in | std::ios::binary
            | std::ios::ate);

    std::ifstream::pos_type filesize = ifs.tellg();
    if (filesize < 0) return false;

    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(filesize);
    ifs.read(&bytes[0], filesize);

    text = std::string(&bytes[0], filesize);
    return true;
}

bool present(Vec<char*> &v, const char* name) {
    for (auto &a : v) {
        if (std::string(a) == std::string(name)) {
            return true;
        }
    }
    return false;
}

Platform get_platform()
{
#ifdef _WIN32
    return Platform::Windows;
#else
#    ifdef __APPLE__
#        ifdef __aarch64__
    return Platform::macOS_ARM;
#        else
    return Platform::macOS_Intel;
#        endif
#    else
#        ifdef __FreeBSD__
    return Platform::FreeBSD;
#        else
    return Platform::Linux;
#        endif
#    endif
#endif
}

// Platform-specific initialization
// On Windows, enable colors in terminal. On other systems, do nothing.
// Return value: 0 on success, negative number on failure.
int initialize()
{
#ifdef _WIN32
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_stdout == INVALID_HANDLE_VALUE)
        return -1;

    DWORD mode;
    if (! GetConsoleMode(h_stdout, &mode))
        return -2;

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (! SetConsoleMode(h_stdout, mode))
        return -3;

    return 0;
#else
    return 0;
#endif
}

}
