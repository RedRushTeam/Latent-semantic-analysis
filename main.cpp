#pragma once
#include "math_core.h"

int main()
{
    auto start = clock();
    setlocale(LC_ALL, "Russian");

    list_of_functions::is_cpp17_possible();
    list_of_functions::print_info_about_sysyem();

    auto texts = list_of_functions::get_input_texts();
    cout << endl << endl << "¬сего обнаружено " << texts->size() << " текстов." << endl;

    math_core _math_core(texts);
    cout << endl << "¬ычисление максимального размера контейнера...";
    _math_core.calculate_max_cont_size();
    int size_for_wichout_rare_words = _math_core.calculate_max_cont_size_without_rare_words();
    int size_for_wichout_rare_words_in_texts = _math_core.calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

    cout << endl << "ћаксимальный размер словар€, отбросив термы с " << CUTOFF << " и менее по€влени€ми: " << size_for_wichout_rare_words;
    cout << endl << "ћаксимальный размер словар€, отбросив термы с по€влени€ми в " << CUTOFF_FR_IN_TEXTS << " и менее текстах, а так же, отбросив термы с " << CUTOFF << " и менее по€влени€ми: " << size_for_wichout_rare_words_in_texts;
    //cout << endl << "ћаксимальный размер словар€, отбросив термы с косинусами, ниже " << DELETE_THRESHOLD << " равен: " << size_for_wichout_rare_words_in_texts_SVD;

    cout << endl << "¬ычисление математического ожидани€ и математической дисперсии...";
    _math_core.calculate_all();
    cout << endl << "ѕереход к нечеткой логике...";
    _math_core.calculate_map_of_flukt_cooloc_fuzzy();
    cout << endl << "¬ычисление SVD коллокаций...";
    _math_core.find_SVD_colloc();

    auto finish = clock();
    cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
    system("pause");
    return 0;
}
