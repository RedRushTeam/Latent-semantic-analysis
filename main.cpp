#pragma once
#include "math_core.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");
	Eigen::initParallel();
	Eigen::setNbThreads(10);

	//list_of_functions::test_of_sqlite();

	list_of_functions::print_info_about_sysyem();

	auto texts = list_of_functions::get_input_texts();

	analyzer::set_number_of_texts(texts->size());

	//piecewise_container_class xxx(4, 50000);
	//cout <<  xxx.get_count_of_concret_collocation(1,2,0) << endl;
	//xxx.increment(1, 2, 0);cout << xxx.get_count_of_concret_collocation(1, 2, 0) << endl;

	//xxx.load_slice(2);

	//xxx.clear_map();

	math_core _math_core(texts);

	_math_core.calculate_max_cont_size();
	int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();
	int size_for_wichout_rare_words_in_texts_SVD = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts_and_SVD();

	cout << endl << endl << "����� ���������� " << texts->size() << " �������." << endl;
	cout << endl << "������������ ������ �������, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words;
	cout << endl << "������������ ������ �������, �������� ����� � ����������� � " << CUTOFF_FR_IN_TEXTS << " � ����� �������, � ��� ��, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words_in_texts;
	cout << endl << "������������ ������ �������, ������ ��� �����, � �������, �������, ��� " << DELETE_THRESHOLD << " � �������������� SVD: " << size_for_wichout_rare_words_in_texts_SVD;

	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
