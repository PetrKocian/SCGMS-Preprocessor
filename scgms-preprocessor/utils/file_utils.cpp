#include "file_utils.h"


bool searchInFile(const fs::path& filePath, const std::string& searchStr) {
	std::ifstream file(filePath);
	bool found = false;

	if (!file.is_open()) {
		std::cerr << "Error opening the file: " << filePath.string() << std::endl;
		return found;
	}

	std::string line;
	int lineNumber = 0;

	while (std::getline(file, line)) {
		lineNumber++;

		if (line.find(searchStr) != std::string::npos) {
			std::cout << "Found '" << searchStr << "' in file " << filePath.string() << ", line " << lineNumber << ": " << line << std::endl;
			found = true;
		}
	}

	file.close();
	return found;
}

std::string modifyDescriptor(const fs::path filePath, std::string origFunctionName, std::string appendToName)
{
	std::string functionDeclaration;
	// open original descriptor file
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error opening the file: " << filePath.string() << std::endl;
		return "";
	}

	fs::path filePathNonConst = filePath;
	filePathNonConst.replace_extension();
	std::string newPath = filePathNonConst.string() + appendToName + ".cpp";


	// open the new file for writing
	std::ofstream newFile(newPath);


	// copy old file into new file, line by line
	std::string line;
	while (std::getline(file, line)) {
		auto pos = line.find(origFunctionName);
		if (pos != std::string::npos) {
			// modify name of the function 
			line.insert(pos + origFunctionName.length(), appendToName);
			functionDeclaration = line;
		}
		newFile << line << std::endl;
	}

	file.close();
	fs::remove(filePath);
	newFile.close();

	auto pos = functionDeclaration.find("{");
	if (pos != std::string::npos) {
		functionDeclaration.erase(pos, 1);
	}
	functionDeclaration.append(";");
	return functionDeclaration;
}


bool searchInFolder(const fs::path& folderPath, const std::string& searchStr) {
	bool found = false;
	for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
		if (fs::is_regular_file(entry.path())) {
			if (searchInFile(entry.path(), searchStr))
			{
				found = true;
			}
		}
	}
	return found;
}