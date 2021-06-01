#pragma once
#include "math_core.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");

	//vector<now_type> test;
	//test.resize(SIZE_OF_PIECE * 50000 * 3);

	list_of_functions::print_info_about_sysyem();

	auto x = piecewise_container_class(COLLOC_DIST, 50000, 0);
	x.download_vec(make_pair(0, 20));
 	//auto texts = list_of_functions::get_input_texts();
	/*list_of_functions::test_of_bit7z();
	piecewise_container_class xxx(4, 20000, true);*/
	//list_of_functions::test_of_libmdbx(4000000, true);
	/*cout <<  xxx.get_count_of_concret_collocation(1,2,0) << endl;
	xxx.increment(1, 2, 0);cout << xxx.get_count_of_concret_collocation(1, 2, 0) << endl;

	xxx.load_slice(2);

	xxx.clear_map();*/
	

	//cout << endl << endl << "Всего обнаружено " << texts->size() << " текстов." << endl;

	/*math_core _math_core(texts);

	_math_core.calculate_max_cont_size();
	int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

	cout << endl << "Максимальный размер словаря, отбросив термы с " << CUTOFF << " и менее появлениями: " << size_for_wichout_rare_words;
	cout << endl << "Максимальный размер словаря, отбросив термы с появлениями в " << CUTOFF_FR_IN_TEXTS << " и менее текстах, а так же, отбросив термы с " << CUTOFF << " и менее появлениями: " << size_for_wichout_rare_words_in_texts;
	//cout << endl << "Максимальный размер словаря, отбросив термы с косинусами, ниже " << DELETE_THRESHOLD << " равен: " << size_for_wichout_rare_words_in_texts_SVD;
	_math_core.calculate_sample_mean();
	*/
	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
