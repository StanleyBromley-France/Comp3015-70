#include "shader_include.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

// Helper to extract directory from a path (everything up to the last '/')
static std::string parentDir(const std::string& path) {
    auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return "";
    return path.substr(0, pos + 1);
}

// Helper to extract filename from a path (after the last '/')
static std::string filename(const std::string& path) {
    auto pos = path.find_last_of("/\\");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

// Trim leading whitespace
static std::string trimLeft(const std::string& s) {
    auto pos = s.find_first_not_of(" \t");
    return pos == std::string::npos ? "" : s.substr(pos);
}

namespace ShaderInclude {

    // Internal recursive worker
    static std::string processFileInternal(
        const std::string& filePath,
        std::vector<std::string>& includeStack)
    {
        // Detect circular include
        for (auto& p : includeStack) {
            if (p == filePath) {
                std::ostringstream err;
                err << "Circular include detected:\n";
                for (auto& q : includeStack) err << "  " << q << "\n";
                err << "  " << filePath;
                throw std::runtime_error(err.str());
            }
        }
        includeStack.push_back(filePath);

        // Open input file
        std::ifstream in(filePath);
        if (!in.is_open()) {
            throw std::runtime_error("Failed to open shader file: " + filePath);
        }

        std::stringstream ss;
        std::string rawLine;
        std::string baseDir = parentDir(filePath);
        size_t lineNumber = 0;

        while (std::getline(in, rawLine)) {
            ++lineNumber;
            auto line = trimLeft(rawLine);

            if (line.rfind("#include", 0) == 0) {
                // Must have two quotes
                auto firstQ = line.find('"');
                auto secondQ = (firstQ != std::string::npos)
                    ? line.find('"', firstQ + 1)
                    : std::string::npos;
                if (firstQ == std::string::npos || secondQ == std::string::npos) {
                    std::ostringstream err;
                    err << filePath << "(" << lineNumber << "): "
                        << "Malformed #include directive: " << rawLine;
                    throw std::runtime_error(err.str());
                }
                std::string incName = line.substr(firstQ + 1, secondQ - firstQ - 1);
                ss << processFileInternal(incName, includeStack);
            }
            else {
                ss << rawLine << "\n";
            }
        }

        includeStack.pop_back();
        return ss.str();
    }

    // Public API
    std::string processFile(const std::string& filePath) {
        std::vector<std::string> includeStack;
        try {
            return processFileInternal(filePath, includeStack);
        }
        catch (const std::exception& e) {
            std::cerr << "ShaderInclude error: " << e.what() << "\n";
            return "";
        }
    }

    void process(const std::string& inputPath) {
        auto content = processFile(inputPath);
        std::ofstream out(outputDir + "/" + filename(inputPath));
        if (!out.is_open()) {
            std::cerr << "ShaderInclude error: Failed to open output file: "
                << outputDir << "/" << filename(inputPath) << "\n";
            return;
        }
        out << content;
    }

} // namespace ShaderInclude