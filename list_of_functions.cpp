#include "list_of_functions.h"

void list_of_functions::print_info_about_sysyem()
{
#ifdef _WIN64
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "This system have " << statex.ullTotalPhys / 1024 / 1024 << " MB of physical memory." << endl;
	cout << "This system have " << statex.ullTotalPageFile / 1024 / 1024 << " MB of paging file." << endl;
#endif
	
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

void list_of_functions::get_perems_from_console()
{
	cout << "Enter the size of the bundle if the volume required to count 1000 texts is approximately 150000: ";
	cin >> global_var::SIZE_OF_PIECE;

	cout << "Enter the search range for collocation pairs: ";
	cin >> global_var::COLLOC_DIST;

	cout << "Enter the cutoff frequency for a specific term: ";
	cin >> global_var::CUTOFF;

	cout << "Enter the number of texts in which a specific term should appear: ";
	cin >> global_var::CUTOFF_FR_IN_TEXTS;
}

bool list_of_functions::delete_file_for_path(fs::path path_to_delete)
{
	if (!fs::exists(path_to_delete))
		return false;
	
	if (fs::is_directory(path_to_delete)) {	//это директория
		fs::recursive_directory_iterator begin(path_to_delete);
		fs::recursive_directory_iterator end;
		shared_ptr<std::vector<fs::path>> files_for_delete = make_shared<std::vector<fs::path>>();
		std::copy_if(begin, end, std::back_inserter(*files_for_delete), [](const fs::path& path) {	return fs::is_regular_file(path); });

		for(auto obj : *files_for_delete)
			fs::remove(obj);
	}
	else
		fs::remove(path_to_delete);

	return true;
}

bool list_of_functions::is_cpp17_possible()
{
#if __cplusplus >= 201703L
	cout << "your compiler now works with c++17" << endl;
	return true;
#else 
	cout << "your compiler don't support c++17; programm will stop" << endl;
	cout << __cplusplus;
	
	exit(-1);
#endif
}

void list_of_functions::print_matrix(const char* desc, MKL_INT m, MKL_INT n, double* a, MKL_INT lda) {
	MKL_INT i, j;
	printf("\n %s\n", desc);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) printf(" %6.2f", a[i * lda + j]);
		printf("\n");
	}
}