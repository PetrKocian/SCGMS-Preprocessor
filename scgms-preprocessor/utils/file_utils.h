#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool searchInFile(const fs::path& filePath, const std::string& searchStr);
bool searchInFolder(const fs::path& folderPath, const std::string& searchStr);
std::string fileNameWithoutExtension(const fs::path& filePath);
std::string modifyDescriptor(const fs::path filePath, std::string origFunctionName, std::string appendToName);