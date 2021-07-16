#pragma once
#include "math_core.h"

int main()
{
    auto start = clock();
    setlocale(LC_ALL, "Russian");

    list_of_functions::is_cpp17_possible();
    list_of_functions::print_info_about_sysyem();

    auto texts = list_of_functions::get_input_texts();
    cout << endl << endl << "Всего обнаружено " << texts->size() << " текстов." << endl;

    math_core _math_core(texts);
    _math_core.calculate_all();

    auto finish = clock();
    cout << endl << endl << ">>> " << finish - start << "ms <<<" << endl;
    system("pause");
    return 0;
}
