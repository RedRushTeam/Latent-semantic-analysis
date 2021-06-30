#pragma once
#include "list_of_types.h"

class list_of_functions
{
public:
	static void print_info_about_sysyem();
	static shared_ptr<vector<fs::path>> get_input_texts();
	static bool compress_file_for_path(fs::path path_to_compress);
	static bool decompress_file_for_path(fs::path path_to_decompress);
	static bool delete_file_for_path(fs::path path_to_delete);
	static bool is_cpp17_possible();
	static void print_matrix(const char* desc, MKL_INT m, MKL_INT n, double* a, MKL_INT lda);
};
