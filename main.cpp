#pragma once
#include "math_core.h"

int main(int argc, char* argv[])
{
	auto start = clock();
	setlocale(LC_ALL, "Russian");

	list_of_functions::is_cpp17_possible();
	list_of_functions::print_info_about_sysyem();

 	auto texts = list_of_functions::get_input_texts();
	cout << endl << endl << "����� ���������� " << texts->size() << " �������." << endl;

	math_core _math_core(texts);
	_math_core.calculate_max_cont_size();
	int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

	cout << endl << "������������ ������ �������, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words;
	cout << endl << "������������ ������ �������, �������� ����� � ����������� � " << CUTOFF_FR_IN_TEXTS << " � ����� �������, � ��� ��, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words_in_texts;
	//cout << endl << "������������ ������ �������, �������� ����� � ����������, ���� " << DELETE_THRESHOLD << " �����: " << size_for_wichout_rare_words_in_texts_SVD;
	
	//cout << endl << "���������� �������� �������� �������...";
	//_math_core.calculate_sample_mean();
	//cout << endl << "���������� ��������������� ��������...";
	//_math_core.calculate_mat_ozidanie();
	cout << endl << "���������� ��������������� �������� � �������������� ���������...";
	_math_core.calculate_all();
	
	auto finish = clock();
	cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
	system("pause");
	return 0;
}
