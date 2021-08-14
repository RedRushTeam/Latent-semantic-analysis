#pragma once
#include "math_core.h"

int main()
{
	/*size_t m = 5;
	size_t n = 4;
	size_t lda = n;
	size_t ldu = m;
	size_t ldvt = n;
	float* superb;

	std::mt19937 gen(time(0));
	std::uniform_real_distribution<> uid(0, 50);

	if (m < n)
		superb = new float[m - 1];
	else
		superb = new float[n - 1];

	int* istat = new int[5];

	float* s = new float[n];
	float* u = new float[ldu * m];
	float* vt = new float[ldvt * n];
	float* a = new float[m*n];

	for (int i = 0; i < m * n; ++i)
		a[i] = uid(gen);

	int res = LAPACKE_sgejsv(LAPACK_ROW_MAJOR, 'C', 'U', 'V', 'R', 'N', 'P', m, n, a, lda, s, u, ldu, vt, ldvt, superb, istat);

	for (int i = 0; i < 5; ++i)
		cout << istat[i] << " ";

	cout << endl;

	for (int i = 0; i < ldu * m; ++i)
		cout << u[i] << " ";*/

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
