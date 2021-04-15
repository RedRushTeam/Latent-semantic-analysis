#include "list_of_functions.h"

void list_of_functions::print_info_about_sysyem()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "This system have " << statex.ullTotalPhys / 1024 / 1024 << " MB of physical memory." << endl << endl;
	cout << "This system have " << statex.ullTotalPageFile / 1024 / 1024 << " MB of paging file." << endl;
}

shared_ptr<vector<fs::path>> list_of_functions::get_input_texts()
{
	auto input_path = TEXTS_PATH/*fs::current_path()/"inputfiles"*/;
	fs::recursive_directory_iterator begin(input_path);
	fs::recursive_directory_iterator end;
	shared_ptr<std::vector<fs::path>> txtFiles = make_shared<std::vector<fs::path>>();
	std::copy_if(begin, end, std::back_inserter(*txtFiles), [](const fs::path& path) {
		return fs::is_regular_file(path) && (path.extension() == ".txt" || path.extension() == ".TXT"); });
	return txtFiles;
}
