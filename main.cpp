#pragma once
#include "analyzer.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");

	list_of_functions::print_info_about_sysyem();

	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
