#pragma once
#include "list_of_types.h"

class list_of_functions
{
public:
	static void print_info_about_sysyem();

	static shared_ptr<vector<fs::path>> get_input_texts();

	static void test_of_sqlite();
};
