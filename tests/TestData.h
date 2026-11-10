#pragma once

#include <string>

// Locate fixture files beside this header, handling both Windows and Unix separators.
inline std::string testDataPath(const std::string& filename) {
    const std::string headerPath = __FILE__;
    const auto lastSeparator = headerPath.find_last_of("/\\");

    std::string testsDirectory;
    if (lastSeparator != std::string::npos) {
        testsDirectory = headerPath.substr(0, lastSeparator + 1);
    }

    return testsDirectory + "data/" + filename;
}
