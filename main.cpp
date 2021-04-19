#pragma once
#include "math_core.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");

	list_of_functions::test_of_sqlite();

	list_of_functions::print_info_about_sysyem();

	auto texts = list_of_functions::get_input_texts();

	cout << endl << endl << "Всего обнаружено " << texts->size() << " текстов." << endl;

	math_core _math_core(texts);
	
	_math_core.calculate_max_cont_size();

	cout << endl << endl << "Размер словаря: " << _math_core.get_max_cont_size() << " слов." << endl;

	_math_core.calculate_all_texts_stats();

	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
