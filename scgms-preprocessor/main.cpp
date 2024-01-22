#include "utils/file_utils.h"
#include "utils/abort.h"

namespace fs = std::filesystem;



int main(int argc, char** argv) {

	// clean scgms directory
	std::error_code errorCode;
	if (!fs::remove_all(target, errorCode)) {
		std::cout << "Removing scgms directory: "<< errorCode.message() << std::endl;
	}

	// copy all filter files to scgms directory
	fs::copy(src, target, fs::copy_options::recursive);

	// delete any non .cpp .c .h files as they won't be recognized by build tools for embedded targets
	for (auto file : fs::recursive_directory_iterator(target))
	{
		const std::string fileExtension = file.path().extension().string();
		if (!(fileExtension == ".cpp" || fileExtension == ".h") && !fs::is_directory(file))
		{
			if (!fs::remove(file.path()))
			{
				std::cout << errorCode.message() << std::endl;
			}	
		}
	}

	// delete DllMain files
	for (auto file : fs::recursive_directory_iterator(target))
	{
		if (searchInFile(file.path(), "DllMain"))
		{
			if (!fs::remove(file.path()))
			{
				std::cout << errorCode.message() << std::endl;
			}
		}
	}

	// holds new function names 
	std::vector<std::string> docreateFunctionNames;
	std::vector<std::string> getdescriptorsFunctionNames;


	// modify descriptor source files of filters and save filter folder names
	std::string folderName = "";
	for (auto file : fs::recursive_directory_iterator(target))
	{
		std::string folderName = "";
		const std::string fileExtension = file.path().extension().string();
		if(searchInFile(file, "do_create_filter") && fileExtension == ".cpp")
		{
			folderName = modifyDescriptor(file, "do_create_filter");
			docreateFunctionNames.push_back("do_create_filter" + folderName);

		}
		if (searchInFile(file, "do_get_filter_descriptors") && fileExtension == ".cpp")
		{
			folderName = modifyDescriptor(file, "do_get_filter_descriptors");
			getdescriptorsFunctionNames.push_back("do_get_filter_descriptors" + folderName);
		}
		renameFile(file, folderName);
	}


	// generate combined descriptors source files





	return 0;

}








/*






	std::vector<fs::path> filters_paths; 

	for (const auto& entry : fs::directory_iterator(src)) {
		filters_paths.push_back(entry);
		std::cout << entry << std::endl;
	}


	std::string includes = "";

	std::string allocation = "void configure(){\r\n"
							 "scgms::IFilter** terminal_filter = new scgms::IFilter*;\r\n";

	std::string manufacture = "";

	std::string configure = "";

	std::string execute = "void execute(scgms::IDevice_Event * event)\r\n"
						  "{\r\n"
						  "(*filter1)->Execute(event);\r\n"
						  "}";

	int filter_index = 0;

	for (auto guid : guids)
	{
		bool found = false;
		fs::path current_filter = "";
		std::wcout << L"Searching " + guid << std::endl;
		std::string converted_str = converter.to_bytes(guid);

		// find in which folder is the guid located
		for (const auto& filter_path : filters_paths)
		{
			if (searchInFolder(filter_path, converted_str))
			{
				std::cout << "found " + converted_str + " in " << filter_path << std::endl;
				current_filter = filter_path;
				filter_index++;
				found = true;
			}
		}

		// filter found
		if (found)
		{
			// include filter files
			for (const auto& entry : fs::recursive_directory_iterator(current_filter)) {
				if (fs::is_regular_file(entry.path())) {
					includes.append("#include \"" + entry.path().generic_string() + "\"\r\n"); //change to relative path?
				}
			}

			// allocate memory for filter
			allocation.append("scgms::IFilter** filter" + std::to_string(filter_index) + " = new scgms::IFilter*;\r\n");

			// manufacture filter and configure
			if (filter_index == 2)//guids.size()) //last filter
			{
				manufacture = "Manufacture_Object<" + getFilterObjectName(guid) + ">(filter" + std::to_string(filter_index) + ", *terminal_filter);\r\n" + manufacture;
				manufacture = "Manufacture_Object<CFilterTerminal>(terminal_filter); \r\n" + manufacture;


				configure = "(*filter"+ std::to_string(filter_index) +")->Configure(NULL, error_description);\r\n" + configure;
				configure = "(*terminal_filter)->Configure(NULL, error_description);\r\n" + configure;
				configure = "refcnt::wstr_list* error_description{};\r\n" + configure;


				//finished

			}
			else
			{
				manufacture = "Manufacture_Object<" + getFilterObjectName(guid) + ">(filter" + std::to_string(filter_index) + ", *filter" + std::to_string(filter_index + 1) + ");\r\n" + manufacture;
				configure = "(*filter" + std::to_string(filter_index) + ")->Configure(NULL, error_description);\r\n" + configure;
			}
		}





	}


	std::string main_file = "";

	main_file.append(includes);
	main_file.append("\r\n");


	main_file.append(allocation);
	main_file.append("\r\n");

	main_file.append(manufacture);
	main_file.append("\r\n");

	main_file.append(configure);
	main_file.append("}\r\n");

	main_file.append(execute);
	

	fs::path main_path = target / "main.cpp";
	std::ofstream outputFile(main_path);


	// Check if the file is open
	if (outputFile.is_open()) {
		outputFile << main_file;
	}
	
	std::cout << main_file;

	system("pause");
	return 0;
}

*/