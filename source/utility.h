#pragma once

#include <memory>

namespace el {

    std::unique_ptr<char[]> fileread(const char* filename);
    std::unique_ptr<char[]> fstreamread(const char* filename);

}
