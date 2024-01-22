#include "file_utils.h"
#include "abort.h"

// define source and target paths
const fs::path cwd = fs::current_path();
const fs::path src = cwd / "filters";
const fs::path target = cwd / "scgms";



bool searchInFile(const fs::path& filePath, const std::string& searchStr) {
	bool found = false;

	if (fs::is_directory(filePath))
	{
		return found;
	}

	std::ifstream file(filePath);


	if (!file.is_open()) {
		std::cerr << "Error opening the file: " << filePath.string() << std::endl;
		return found;
	}

	std::string line;
	int lineNumber = 0;

	while (std::getline(file, line)) {
		lineNumber++;

		if (line.find(searchStr) != std::string::npos) {
			found = true;
		}
	}

	file.close();
	return found;
}

std::string fileNameWithoutExtension(const fs::path& filePath)
{
	fs::path filePathNonConst = filePath;
	filePathNonConst.replace_extension();
	return filePathNonConst.string();
}

void renameFile(const fs::directory_entry file, std::string appendString)
{
	const std::string fileExtension = file.path().extension().string();
	std::string newPath = fileNameWithoutExtension(file.path()) + appendString + fileExtension;
	fs::rename(file.path(), newPath);
}

std::string insertIntoFile(const fs::path filePath, std::string appendAfter, std::string appendString)
{
	std::string functionDeclaration;
	// open original descriptor file
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error opening the file: " << filePath.string() << std::endl;
		return "";
	}


	std::string newPath = filePath.string() + "_";


	// open the new file for writing
	std::ofstream newFile(newPath);


	// copy old file into new file, line by line
	std::string line;
	while (std::getline(file, line)) {
		auto pos = line.find(appendAfter);
		if (pos != std::string::npos) {
			// modify name of the function 
			line.insert(pos + appendAfter.length(), appendString);
			functionDeclaration = line;
		}
		newFile << line << std::endl;
	}

	file.close();
	fs::remove(filePath);
	newFile.close();
	fs::rename(newPath, filePath);

	auto pos = functionDeclaration.find("{");
	if (pos != std::string::npos) {
		functionDeclaration.erase(pos, 1);
	}
	functionDeclaration.append(";");
	return functionDeclaration;
}

std::string modifyDescriptor(fs::directory_entry& file, std::string searchString)
{
	std::string folderName = "";
	fs::path parentPath = file.path().parent_path();
	fs::path currentDirPath = file.path();
	while (parentPath.filename().string() != "scgms")
	{
		currentDirPath = parentPath;
		parentPath = parentPath.parent_path();
	}
	if (fs::is_directory(currentDirPath))
	{
		folderName = "_" + currentDirPath.filename().string();
		std::string declaration = insertIntoFile(file, searchString, folderName);

		fs::path headerPath(fileNameWithoutExtension(file.path()) + ".h");
		if (fs::exists(headerPath))
		{
			std::ofstream header(headerPath, std::ios::app);
			if (header.is_open())
			{
				header << std::endl << declaration;
			}
		}
		else
		{
			abort("Could not find descriptor header file for:\r\n" + file.path().string());
		}
	}
	return folderName;
}


bool searchInFolder(const fs::path & folderPath, const std::string & searchStr) {
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