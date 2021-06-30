#pragma once
#include "math_core.h"

int main()
{
    auto start = clock();
    setlocale(LC_ALL, "Russian");

    list_of_functions::is_cpp17_possible();
    list_of_functions::print_info_about_sysyem();

    auto texts = list_of_functions::get_input_texts();
    cout << endl << endl << "����� ���������� " << texts->size() << " �������." << endl;

    math_core _math_core(texts);
    cout << endl << "���������� ������������� ������� ����������...";
    _math_core.calculate_max_cont_size();
    int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
    int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

    cout << endl << "������������ ������ �������, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words;
    cout << endl << "������������ ������ �������, �������� ����� � ����������� � " << CUTOFF_FR_IN_TEXTS << " � ����� �������, � ��� ��, �������� ����� � " << CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words_in_texts;
    //cout << endl << "������������ ������ �������, �������� ����� � ����������, ���� " << DELETE_THRESHOLD << " �����: " << size_for_wichout_rare_words_in_texts_SVD;

    cout << endl << "���������� ��������������� �������� � �������������� ���������...";
    _math_core.calculate_all();
    cout << endl << "������� � �������� ������...";
    _math_core.calculate_map_of_flukt_cooloc_fuzzy();
    cout << endl << "���������� SVD ����������...";
    _math_core.find_SVD_colloc();

    auto finish = clock();
    cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
    system("pause");
    return 0;
}
