#pragma once

#include <memory>
#include <string>

namespace el {

    // Reference(S):
    // - https://web.archive.org/web/20181115035420/http://cnicholson.net/2011/01/stupid-c-tricks-a-better-sizeof_array/
    // - https://stackoverflow.com/questions/4415530/equivalents-to-msvcs-countof-in-other-compilers 
    template <typename T, size_t N>
    size_t countof(T(&arr)[N])
    {
        return std::extent< T[N] >::value;
    }

    std::unique_ptr<char[]> fileread(const std::string& filename);
    std::unique_ptr<char[]> fstreamread(const std::string& filename);

} // namespace el {
