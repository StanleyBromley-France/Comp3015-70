#pragma once
#include <string>
#pragma once

namespace ShaderInclude {
    // Hard-coded output directory (must exist beforehand)
    const std::string outputDir = "shader/out";

    // Recursively read a file, replace #include "..." with file contents
    std::string processFile(const std::string& filePath);

    // Process input file and write result into outputDir preserving filename
    void process(const std::string& inputPath);
}