#include "math_core.h"

now_type math_core::divider(int size)
{
	int starter = 1;
	int finisher = size;
	auto sum = (starter + finisher) * (finisher - starter + 1) / 2;
	return sum;
}

int math_core::calculate_max_cont_size_without_rare_words()
{
	this->max_cont_size = analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff(global_var::CUTOFF);

	this->number_of_slices = (int)ceil((float)this->max_cont_size * 3 / (float)global_var::SIZE_OF_PIECE);

	return this->max_cont_size;
}

int math_core::calculate_max_cont_size_without_rare_words_and_frequency_in_texts()
{
	this->max_cont_size = analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(global_var::CUTOFF, global_var::CUTOFF_FR_IN_TEXTS);

	this->number_of_slices = (int)ceil((float)this->max_cont_size * 3 / (float)global_var::SIZE_OF_PIECE);

	return this->max_cont_size;
}

int math_core::calculate_max_cont_size_without_rare_words_and_frequency_in_texts_and_SVD()
{
	analyzer::initialize_matrix_for_SVD();

	#pragma omp parallel
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->vec_of_filepaths->size(); ++i) {
			parser _parser((*this->vec_of_filepaths)[i]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_counter_of_tokenizer_SVD_words();
		}
	}

	this->max_cont_size = analyzer::get_counter_of_tokenizer_without_rare_words_SVD();

	this->number_of_slices = (int)ceil((float)this->max_cont_size * 3 / (float)global_var::SIZE_OF_PIECE);

	return this->max_cont_size;
}

void math_core::calculate_all()
{
	cout << endl << "���������� ������������� ������� ����������...";
	this->calculate_max_cont_size();
	int size_for_wichout_rare_words = this->calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = this->calculate_max_cont_size_without_rare_words_and_frequency_in_texts();

	cout << endl << "������������ ������ �������, �������� ����� � " << global_var::CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words;
	cout << endl << "������������ ������ �������, �������� ����� � ����������� � " << global_var::CUTOFF_FR_IN_TEXTS << " � ����� �������, � ��� ��, �������� ����� � " << global_var::CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words_in_texts;
	//cout << endl << "������������ ������ �������, �������� ����� � ����������, ���� " << DELETE_THRESHOLD << " �����: " << size_for_wichout_rare_words_in_texts_SVD;


	this->mat_ozidanie = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);
	analyzer::set_container_mat_ozidanie(this->mat_ozidanie);

	this->mat_disperse = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);

	cout << "����� ���������� ����� ������������ � " << this->number_of_slices << " ������: ";

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices) {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
		}
		else {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
		}

		cout << endl << "���������� ��������������� ��������...";
		this->calculate_mat_ozidanie();
		cout << endl << "���������� �������������� ���������...";
		this->calculate_mat_disperse();
		cout << endl << "���������� �������������� ����������...";
		this->find_fluctuations();
		cout << endl << "���������� ����� �������������� ����������...";
		this->shrink_set_of_fluct_cooloc();
		cout << endl << "���������� ���������� ��������������� ��������...";
		this->shrink_mat_ozid();
		cout << endl << "���������� ������ ��� SVD ����������...";
		this->find_SVD_terms();
		this->get_shrinked_cosinuses_terms();
		cout << endl << "���������� ��������������� �����...";
		this->calculate_map_of_flukt_cooloc_fuzzy();
		cout << endl << "SVD ���������� �������������� ����������...";
		this->find_SVD_colloc();

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->fill_vector((now_type)0.0);
		dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->fill_vector((now_type)0.0);

		cout << "(" << i + 1 << "/" << this->number_of_slices << ") ";
	}
}

void math_core::calculate_max_cont_size()
{
	analyzer::create_lemmatizer();
	analyzer::set_number_of_texts(this->vec_of_filepaths->size());
	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->vec_of_filepaths->size(); ++i) {
			parser _parser((*this->vec_of_filepaths)[i]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();
			
			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_counter_of_tokenizer_without_rare_words();
		}
	}
}

void math_core::calculate_mat_ozidanie()
{
	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_mat_ozidanie();
		}
	}

	(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= (now_type)this->vec_of_filepaths->size();
}

void math_core::calculate_mat_disperse()
{
	#pragma omp parallel
	{
		#pragma omp for schedule(static)
	
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			auto now_cont = _analyzer.calculate_mat_disperse();
			now_cont->pow_all(2);
			#pragma omp critical (disperse)
			{
				(*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)) += now_cont;
			}
		}
	}

	(*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)) /= (now_type)this->vec_of_filepaths->size();
}

void math_core::find_fluctuations()
{
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto mat_disperse_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse);

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = mat_ozid_like_piese->get_downloaded_range().first; i < mat_ozid_like_piese->get_downloaded_range().second; ++i)
			for (int j = 0; j < this->max_cont_size; ++j)
				for (int k = 0; k <= global_var::COLLOC_DIST; ++k) {
					bool kond1 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) - /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k)/*)*/ > mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size());
					bool kond2 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) + /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k)/*)*/ < mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size());
					if (kond1 || kond2){
						#pragma omp critical (set_of_fluct_cooloc)
						{
							this->set_of_fluct_cooloc.insert(three_coordinate_structure{ (int)(mat_ozid_like_piese->get_downloaded_range().first + i), (int)j, (short)k });
						}
					}
				}
	}

	cout << endl << "����� �������������� ����������: " << set_of_fluct_cooloc.size();
}

void math_core::shrink_mat_ozid()
{
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);

	tsl::robin_set<three_coordinate_structure> set_for_delete;
	size_t a = 0, b = 0;
	for (auto obj : *mat_ozid_like_piese->get_vector_ptr()) {
		auto three = mat_ozid_like_piese->split_three_coordinates_from_one(obj.first);
		if (this->set_of_fluct_cooloc.find(three) == this->set_of_fluct_cooloc.end()) {
			set_for_delete.insert(three);
			++a;
		}
	}

	for (auto obj : set_for_delete) {
		this->mat_ozidanie->erase_concret_colloc(obj.first_coord, obj.second_coord, obj.k);
		++b;
	}
	cout << "debug:" << set_for_delete.size() << " " << a << " " << b << endl;
	cout << endl << "����� ���������� � ��� �������� ����� ������: " << dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->get_vector_ptr()->size();
}

void math_core::shrink_set_of_fluct_cooloc()
{
	tsl::robin_set<three_coordinate_structure> set_for_deleted_colloc;

	for (auto obj : this->set_of_fluct_cooloc)
		if (this->mat_ozidanie->get_count_of_concret_collocation(obj.first_coord, obj.second_coord, obj.k) * this->vec_of_filepaths->size() < global_var::CUTOFF_FR_COLLOC_IN_TEXTS)
			set_for_deleted_colloc.insert(obj);

	for (auto obj : set_for_deleted_colloc)
		this->set_of_fluct_cooloc.erase(obj);

	cout << endl << "����� �������������� ����������, ����� ������ ������� ��������� " << global_var::CUTOFF_FR_COLLOC_IN_TEXTS << " �����: " << this->set_of_fluct_cooloc.size();
}

void math_core::calculate_norm_shrinked_mat_ozid()
{
	//this->shrink_mat_ozid();

	auto mat_ozid_like_piece = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto shrinked_vec_mat_ozid = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->get_vector_ptr();

	list<now_type> numbers_for_one_colloc;
	unordered_set<pair<int, int>> set_for_checked_collocs;

	for (auto obj : *shrinked_vec_mat_ozid) {
		auto three_coord = mat_ozid_like_piece->split_three_coordinates_from_one(obj.first);

		if (set_for_checked_collocs.find(make_pair(three_coord.first_coord, three_coord.second_coord)) != set_for_checked_collocs.end())
			continue;

		for (int k = 0; k <= global_var::COLLOC_DIST; ++k)
			numbers_for_one_colloc.push_back(mat_ozid_like_piece->get_count_of_concret_collocation(three_coord.first_coord, three_coord.second_coord, k));

		auto max_elem = std::max_element(numbers_for_one_colloc.begin(), numbers_for_one_colloc.end());

		std::transform(numbers_for_one_colloc.begin(), numbers_for_one_colloc.end(), numbers_for_one_colloc.begin(), [&](now_type& obj0) {return obj0 / *max_elem;	});

		short counter = 0; 
		for (auto obj : numbers_for_one_colloc) {
			mat_ozid_like_piece->set_count_of_concret_collocation(three_coord.first_coord, three_coord.second_coord, counter, obj);
			++counter;
		}

		set_for_checked_collocs.insert(make_pair(three_coord.first_coord, three_coord.second_coord));
		numbers_for_one_colloc.clear();
	}
}

void math_core::calculate_map_of_flukt_cooloc_fuzzy()
{
	this->helper_map_for_SVD_rows_colloc_numbers = make_shared<tsl::robin_map<size_t, three_coordinate_structure>>();
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	
	size_t indexer = 0;
	for (auto& obj : *mat_ozid_like_piese->get_vector_ptr()) {
		this->helper_map_for_SVD_rows_colloc_numbers->insert(make_pair(indexer, mat_ozid_like_piese->split_three_coordinates_from_one(obj.first)));
		++indexer;
	}
	
	//cout << endl << "����� �������������� ���������� ����� �������: " << this->helper_map_for_SVD_rows_colloc_numbers->size();

	analyzer::set_helper_map_for_SVD_rows_colloc_numbers(this->helper_map_for_SVD_rows_colloc_numbers);

	this->set_of_fluct_cooloc.clear();
}

void math_core::find_SVD_colloc()
{
	dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->clear_vec();

	size_t m = this->helper_map_for_SVD_rows_colloc_numbers->size();
	size_t n = this->vec_of_filepaths->size();
	size_t lda = n;
	//size_t ldu = m;
	size_t ldvt = n;

	size_t svd_piece = SVD_PIECE;
	if (this->vec_of_filepaths->size() > 1000)
		svd_piece = svd_piece * 1000 / this->vec_of_filepaths->size();

	size_t pieces = 0;
	if (svd_piece != this->set_for_unique_terms.get()->size())
		pieces = this->helper_map_for_SVD_rows_colloc_numbers->size() / (svd_piece - this->set_for_unique_terms.get()->size());

	if (pieces < 0)
	{
		cout << endl << "���������� ������ ��������� ���������� ��� ��� �����";
		exit(-4);
	}

	cout << endl << "����� ���������� ����� ������������� � " << pieces+1 << " �����";

	int idx = 0;
	float* svd_array = new float[lda * svd_piece];
	for (int i = 0; i < this->set_for_unique_terms.get()->size(); ++i)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j)
		{
			svd_array[idx] = analyzer::get_only_terms_mass()[i * this->vec_of_filepaths->size() + j];
			++idx;
		}

	float* a;
	size_t size_a = 0;

	for (auto p = 0; p <= pieces; ++p) {
		if (p != pieces) {
			a = new float[lda * svd_piece - idx];
			size_a = lda * svd_piece;
		}
		else
			if (m - pieces * svd_piece) {
				size_a = (m - pieces * svd_piece) * lda - idx;
				a = new float[size_a];
			}
			else
				continue;

		#pragma omp parallel 
		{
			#pragma omp for schedule(static)
			for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
				parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
				auto result_of_parse = _parser.parse();

				analyzer _analyzer(result_of_parse);
				auto column = _analyzer.calculate_SVD_matrix_for_concret_text();
				int counter = p * n;
				for (int i = j; i < size_a; i += (size_a/lda)) {
					a[i] = (*column)(counter, 0);
					++counter;
				}
			}
		}
		
		int new_idx = idx;
		int a_idx = 0;
		for (int i = new_idx; i < (lda * svd_piece); ++i)
		{
			svd_array[i] = a[a_idx];
			++a_idx;
		}

		/*debug*/ cout << endl << "svd_piece = " << svd_piece << " really = " << idx + a_idx << endl; /*debug*/
		this->SVD_colloc_algorithm(svd_array, svd_piece);
		cout << endl << "���������� ���������� ����� ���������� � ���������� " << KOEF_FOR_COLLOC_COS_DELETE << " :" << this->cosinuses.size();
	}

	auto blyadovka1 = 1;
}

void math_core::SVD_colloc_algorithm(float* arr, size_t rows)
{
	//this->cosinuses.clear();

	size_t m = rows;
	size_t n = this->vec_of_filepaths->size();
	size_t lda = n;
	size_t ldu = m;
	size_t ldvt = n;
	int info;
	float* superb;

	if (m < n)
		superb = new float[m - 1];
	else
		superb = new float[n - 1];

	float* s = new float[n];
	float* u = new float[ldu * m];
	float* vt = new float[ldvt * n];
	float* a = arr;

	LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'A', 'A', m, n, a, lda, s, u, ldu, vt, ldvt, superb);

	if (info > 0) {
		cout << "The algorithm computing SVD failed to converge." << endl;
		exit(-1488);
	}
	delete[] a;
	delete[] s;

	shared_ptr<MatrixXf> resized_V_matrix_of_SVD = make_shared<MatrixXf>();
	resized_V_matrix_of_SVD->resize(global_var::COLLOC_DIST + 1, n);

	for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
		for (int j = 0; j < n; ++j)
			resized_V_matrix_of_SVD->operator()(i, j) = vt[i * n + j];

	delete[] vt;

	shared_ptr<MatrixXf> resized_U_matrix_of_SVD = make_shared<MatrixXf>();
	resized_U_matrix_of_SVD->resize(m, global_var::COLLOC_DIST + 1);

	int starter = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j <= global_var::COLLOC_DIST; ++j)
			resized_U_matrix_of_SVD->operator()(i, j) = u[starter + j];
		starter += n;
	}
	delete[] u;

	vector<float> lenghts_colloc_vector;
	lenghts_colloc_vector.resize(m, NULL);

	for (auto i = 0; i < resized_U_matrix_of_SVD->rows(); ++i) {
		for (auto j = 0; j < resized_U_matrix_of_SVD->cols(); ++j) {
			lenghts_colloc_vector[i] += (*resized_U_matrix_of_SVD)(i, j) * (*resized_U_matrix_of_SVD)(i, j);
		}
		lenghts_colloc_vector[i] = sqrt(lenghts_colloc_vector[i]);
	}

	vector<float> lenghts_texts_vector;
	lenghts_texts_vector.resize(n, NULL);

	for (auto i = 0; i < resized_V_matrix_of_SVD->cols(); ++i) {
		for (auto j = 0; j < resized_V_matrix_of_SVD->rows(); ++j) {
			lenghts_texts_vector[i] += (*resized_V_matrix_of_SVD)(j, i) * (*resized_V_matrix_of_SVD)(j, i);
		}
		lenghts_texts_vector[i] = sqrt(lenghts_texts_vector[i]);
	}

	unordered_map<pair<int, int>, float> scalar_proizv;

	for (auto k = 0; k < resized_V_matrix_of_SVD->cols(); ++k)
		for (auto i = 0; i < resized_U_matrix_of_SVD->rows(); ++i)
			for (auto j = 0; j < resized_U_matrix_of_SVD->cols(); ++j) {
				auto iter = scalar_proizv.find(make_pair(i, k));
				if (iter == scalar_proizv.end())
					scalar_proizv.insert(make_pair(make_pair(i, k), (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(j, k)));
				else
					scalar_proizv[make_pair(i, k)] = iter->second + (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(j, k);
			}

	for (int i = 0; i < lenghts_colloc_vector.size(); ++i)
		for (int j = 0; j < lenghts_texts_vector.size(); ++j) {
			auto iter = this->cosinuses.find(make_pair(i, j));
			if (iter == this->cosinuses.end())
				this->cosinuses.insert(make_pair(make_pair(i, j), (lenghts_colloc_vector[i] / lenghts_texts_vector[j])));
			else
				this->cosinuses[make_pair(i, j)] = iter->second / lenghts_colloc_vector[i] / lenghts_texts_vector[j];
		}

	list<pair<int, int>> list_of_terms_will_be_deleted;

	for (auto& obj : this->cosinuses)
		if (obj.second < KOEF_FOR_COLLOC_COS_DELETE && (this->cosinuses.find(obj.first) != this->cosinuses.end()))
			list_of_terms_will_be_deleted.push_back(obj.first);

	for (auto& obj : list_of_terms_will_be_deleted)
		this->cosinuses.erase(obj);
}

void math_core::find_SVD_terms()
{
	size_t m = this->max_cont_size;
	size_t n = this->vec_of_filepaths->size();
	size_t lda = n;
	size_t ldu = m;
	size_t ldvt = n;

	shared_ptr<float[]> only_terms_mass(new float[lda * m]{});
	analyzer::set_only_terms_mass(only_terms_mass);

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_matrix_only_for_terms(j);
		}
	}

	this->SVD_colloc_algorithm(only_terms_mass.get(), this->max_cont_size);
	cout << endl << "���������� ������ ����� ���������� � ���������� " << KOEF_FOR_COLLOC_COS_DELETE << " :" << this->cosinuses.size();
}

shared_ptr<unordered_set<int>> math_core::get_shrinked_cosinuses_terms()
{
	this->set_for_unique_terms = make_shared<unordered_set<int>>();

	for (auto obj : this->cosinuses)
		this->set_for_unique_terms->insert(obj.first.first);

	cout << endl << "���������� ���������� ������ ����� �������: " << set_for_unique_terms->size();

	this->cosinuses.clear();

	return set_for_unique_terms;
}

shared_ptr<container_class_interface> math_core::get_mat_ozidanie() const
{
	return this->mat_ozidanie;
}

shared_ptr<container_class_interface> math_core::get_mat_disperse() const
{
	return this->mat_disperse;
}

int math_core::get_max_cont_size() const
{
	return this->max_cont_size;
}

now_type math_core::prepare_and_get_s_norm_for_one_colloc(int first_term, int second_term) const
{
	now_type norm_mat_ozid_for_one_colloc = 0;	//dima check this

	for(int i = global_var::COLLOC_DIST; 0 <= i; --i)
		if (i - 1 >= 0) {
			now_type tmp_perem_for_first = this->mat_ozidanie->get_count_of_concret_collocation(first_term, second_term, i - 1);

			if (i == global_var::COLLOC_DIST) {
				now_type tmp_perem_for_second = this->mat_ozidanie->get_count_of_concret_collocation(first_term, second_term, i);
				norm_mat_ozid_for_one_colloc = tmp_perem_for_first + tmp_perem_for_second - tmp_perem_for_first * tmp_perem_for_second;
			}
			else {
				norm_mat_ozid_for_one_colloc = tmp_perem_for_first + norm_mat_ozid_for_one_colloc - tmp_perem_for_first * norm_mat_ozid_for_one_colloc;
			}
		}

	return norm_mat_ozid_for_one_colloc;
}
