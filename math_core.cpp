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
	this->mat_ozidanie = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);
	analyzer::set_container_mat_ozidanie(this->mat_ozidanie);

	this->mat_disperse = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);

	cout << "¬сего вычисление будет проиводитьс€ в " << this->number_of_slices << " этапов: ";

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices) {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
		}
		else {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
		}

		this->calculate_mat_ozidanie();
		this->calculate_mat_disperse();
		this->find_fluctuations();

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
					bool kond1 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) - /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k)/*)*/ > mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k)* (this->vec_of_filepaths->size());
					bool kond2 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) + /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k)/*)*/ < mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k)* (this->vec_of_filepaths->size());
					if (kond1 || kond2){
						#pragma omp critical (set_of_fluct_cooloc)
						{
							set_of_fluct_cooloc.insert(three_coordinate_structure{ (int)(mat_ozid_like_piese->get_downloaded_range().first + i), (int)j, (short)k });
						}
					}
				}
	}

	cout << endl << "„исло подозрительных коллокаций: " << set_of_fluct_cooloc.size();
}

void math_core::calculate_map_of_flukt_cooloc_fuzzy()
{
	//TODO check this
	dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->clear_vec();
	//dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->clear_vec();

	unordered_set<pair<int, int>> helper_set;

	for (auto obj : this->set_of_fluct_cooloc)
		helper_set.insert(make_pair(obj.first_coord, obj.second_coord));

	this->set_of_fluct_cooloc.clear();

	this->map_of_flukt_cooloc_fuzzy = make_shared<tsl::robin_map<pair<int, int>, now_type>>();
	
	for (auto& obj : helper_set)
		map_of_flukt_cooloc_fuzzy->insert(make_pair(obj, this->prepare_and_get_norm_mat_ozid_for_one_colloc(obj.first, obj.second)));

	this->helper_map_for_SVD_rows_colloc_numbers = make_shared<tsl::robin_map<int, pair<int, int>>>();

	int indexer = 0;
	for (auto& obj : *this->map_of_flukt_cooloc_fuzzy) {
		this->helper_map_for_SVD_rows_colloc_numbers->insert(make_pair(indexer, make_pair(obj.first.first, obj.first.second)));

		++indexer;
	}
	cout << endl << "„исло подозрительных коллокаций после свертки: " << this->helper_map_for_SVD_rows_colloc_numbers->size();

	analyzer::set_helper_map_for_SVD_rows_colloc_numbers(this->helper_map_for_SVD_rows_colloc_numbers);
	analyzer::set_map_of_flukt_cooloc_fuzzy(this->map_of_flukt_cooloc_fuzzy);

	this->set_of_fluct_cooloc.clear();
}

void math_core::find_SVD_colloc()
{
	size_t m = this->map_of_flukt_cooloc_fuzzy->size();
	size_t n = this->vec_of_filepaths->size();
	size_t lda = n;
	size_t ldu = m;
	size_t ldvt = n;
	
	float* a = new float[lda * m];

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			auto column = _analyzer.calculate_SVD_matrix_for_concret_text();

			int counter = 0;
			for (int i = j; i < lda * m; i += (*column).rows()) {
				a[i] = (*column)(counter, 0);
				++counter;
			}
		}
	}

	size_t svd_piece = SVD_PIECE;
	if (this->vec_of_filepaths->size() > 1000)
		svd_piece = svd_piece * 1000 / this->vec_of_filepaths->size();

	size_t pieces = this->map_of_flukt_cooloc_fuzzy->size() / svd_piece;

	list<pair<float*, int>> arrays_for_svd;

	for (int i = 0; i < pieces; ++i)
		arrays_for_svd.push_back(make_pair(new float[svd_piece], svd_piece));

	if (pieces * svd_piece != this->map_of_flukt_cooloc_fuzzy->size())
		arrays_for_svd.push_back(make_pair(new float[this->map_of_flukt_cooloc_fuzzy->size() - pieces * svd_piece], this->map_of_flukt_cooloc_fuzzy->size() - pieces * svd_piece));

	int index = 0;
	for (auto& obj : arrays_for_svd) {
		for (int i = index; i < index + obj.second; ++i)
			obj.first[i] = (float)a[i];

		index += obj.second;
		this->SVD_colloc_algorithm(obj.first, obj.second);
	}
}

void math_core::SVD_colloc_algorithm(float* arr, size_t rows)
{
	this->cosinuses.clear();

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

	info = LAPACKE_sgesvd(LAPACK_ROW_MAJOR, 'A', 'A', m, n, a, lda, s, u, ldu, vt, ldvt, superb);

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
			lenghts_texts_vector[i] += (*resized_V_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(i, j);
		}
		lenghts_texts_vector[i] = sqrt(lenghts_texts_vector[i]);
	}

	unordered_map<pair<int, int>, float> scalar_proizv;

	for (auto k = 0; k < resized_V_matrix_of_SVD->cols(); ++k)
		for (auto i = 0; i < resized_U_matrix_of_SVD->rows(); ++i)
			for (auto j = 0; j < resized_U_matrix_of_SVD->cols(); ++j) {
				auto iter = scalar_proizv.find(make_pair(i, k));
				if (iter == scalar_proizv.end())
					scalar_proizv.insert(make_pair(make_pair(i, k), (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(k, j)));
				else
					scalar_proizv[make_pair(i, k)] = iter->second + (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(k, j);
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

	cout << endl << " оличество термов после разложени€ с множителем " << KOEF_FOR_COLLOC_COS_DELETE << " :" << this->cosinuses.size();

	auto blyadovka1 = nullptr;
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
}

shared_ptr<unordered_set<int>> math_core::get_shrinked_cosinuses_terms()
{
	shared_ptr<unordered_set<int>> set_for_unique_terms = make_shared<unordered_set<int>>();

	for (auto obj : this->cosinuses)
		set_for_unique_terms->insert(obj.first.first);

	cout << endl << " оличество уникальных термов после свертки: " << set_for_unique_terms->size();

	return set_for_unique_terms;
}

void math_core::prepare_and_calculate_strange_matrix()
{
	this->find_SVD_terms();
	auto shrinked_cosinuses_terms_set = this->get_shrinked_cosinuses_terms();

	this->calculate_map_of_flukt_cooloc_fuzzy();
	this->find_SVD_colloc();

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

now_type math_core::prepare_and_get_norm_mat_ozid_for_one_colloc(int first_term, int second_term) const
{
	now_type norm_mat_ozid_for_one_colloc = 0;	//dima check this

	for(int i = global_var::COLLOC_DIST; 0 <= global_var::COLLOC_DIST; --i)
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
