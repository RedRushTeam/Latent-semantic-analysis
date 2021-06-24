#pragma once
#include "math_core.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");

	list_of_functions::is_cpp17_possible();
	list_of_functions::print_info_about_sysyem();

 	auto texts = list_of_functions::get_input_texts();
	cout << endl << endl << "¬сего обнаружено " << texts->size() << " текстов." << endl;

	math_core _math_core(texts);
	_math_core.calculate_max_cont_size();
	int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

	cout << endl << "ћаксимальный размер словар€, отбросив термы с " << CUTOFF << " и менее по€влени€ми: " << size_for_wichout_rare_words;
	cout << endl << "ћаксимальный размер словар€, отбросив термы с по€влени€ми в " << CUTOFF_FR_IN_TEXTS << " и менее текстах, а так же, отбросив термы с " << CUTOFF << " и менее по€влени€ми: " << size_for_wichout_rare_words_in_texts;
	//cout << endl << "ћаксимальный размер словар€, отбросив термы с косинусами, ниже " << DELETE_THRESHOLD << " равен: " << size_for_wichout_rare_words_in_texts_SVD;
	
	//cout << endl << "¬ычисление среднего значени€ выборки...";
	//_math_core.calculate_sample_mean();
	//cout << endl << "¬ычисление математического ожидани€...";
	//_math_core.calculate_mat_ozidanie();
	cout << endl << "¬ычисление математического ожидани€ и математической дисперсии...";
	_math_core.calculate_all();
	
	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
