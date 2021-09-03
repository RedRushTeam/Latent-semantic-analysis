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
	parser::prepair_stop_words();
	
	std::cout.flush();
	cout << endl << "���������� ������������� ������� ����������...";
	this->calculate_max_cont_size();
	//int size_for_wichout_rare_words = this->calculate_max_cont_size_without_rare_words();
	int size_for_wichout_rare_words_in_texts = this->calculate_max_cont_size_without_rare_words_and_frequency_in_texts();
	std::cout.flush();
	//cout << endl << "������������ ������ �������, �������� ����� � " << global_var::CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words;
	cout << endl << "������������ ������ �������, �������� ����� � ����������� � " << global_var::CUTOFF_FR_IN_TEXTS << " � ����� �������, � ��� ��, �������� ����� � " << global_var::CUTOFF << " � ����� �����������: " << size_for_wichout_rare_words_in_texts;
	//cout << endl << "������������ ������ �������, �������� ����� � ����������, ���� " << DELETE_THRESHOLD << " �����: " << size_for_wichout_rare_words_in_texts_SVD;

	std::cout.flush();
	cout << endl << "����� ���������� ����� ������������ � " << this->number_of_slices << " ������: ";

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices) {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), this->max_cont_size));
		}
		else {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
			dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->set_downloaded_range(make_pair(i * (global_var::SIZE_OF_PIECE), (i + 1) * (global_var::SIZE_OF_PIECE)));
		}

		cout << endl << "����-���� � ���������������� �����: " << parser::stop_words.size();

		cout << endl << "���������� � SVD ���������� ������...";	//6
		this->find_SVD_terms();
		std::cout.flush();
		cout << endl << "���������� ��������������� ��������...";	//1
		this->calculate_mat_ozidanie();
		std::cout.flush();
		cout << endl << "���������� �������������� ���������...";	//2
		this->calculate_mat_disperse();
		std::cout.flush();
		cout << endl << "���������� �������������� ����������...";	//3
		this->find_fluctuations();
		std::cout.flush();
		cout << endl << "���������� ����� �������������� ����������...";	//4
		this->shrink_set_of_fluct_cooloc();
		std::cout.flush();
		cout << endl << "���������� ���������� ��������������� ��������...";	//5
		this->shrink_mat_ozid();
		std::cout.flush();
		cout << endl << "���������� �������������� ��������������� ��������...";	//7
		this->calculate_norm_shrinked_mat_ozid();
		std::cout.flush();
		cout << endl << "���������� tf � idf...";	//8
		this->calculate_tf_idf();
		cout << endl << "SVD ���������� tf �������...";	//9
		this->prepare_tf_matrix_for_SVD();

		/*cout << endl << "SVD ���������� �������������� ����������...";
		this->find_SVD_colloc();
		std::cout.flush();*/

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->fill_vector((now_type)0.0);
		dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->fill_vector((now_type)0.0);

		std::cout.flush();
		cout << "(" << i + 1 << "/" << this->number_of_slices << ") ";
	}
}

void math_core::calculate_max_cont_size()
{
	analyzer::create_lemmatizer();
	analyzer::set_number_of_texts(this->vec_of_filepaths->size());

	for_each(execution::seq, this->vec_of_filepaths->begin(), this->vec_of_filepaths->end(), [](fs::path& _path) {
		parser _parser(_path);	//tut peredaetsa kopiya
		auto result_of_parse = _parser.parse();

		analyzer _analyzer(result_of_parse);
		_analyzer.calculate_counter_of_tokenizer_without_rare_words();
	});

}

void math_core::calculate_mat_ozidanie()
{
	this->mat_ozidanie = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);
	analyzer::set_container_mat_ozidanie(this->mat_ozidanie);

	for_each(execution::par_unseq, this->vec_of_filepaths->begin(), this->vec_of_filepaths->end(), [](fs::path& _path) {
		parser _parser(_path);	//tut peredaetsa kopiya
		auto result_of_parse = _parser.parse();

		analyzer _analyzer(result_of_parse);
		_analyzer.calculate_mat_ozidanie();
	});

	(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= (now_type)this->vec_of_filepaths->size();
}

void math_core::calculate_mat_disperse()
{
	this->mat_disperse = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->max_cont_size);

	mutex mu_for_disp_sum;
	for_each(execution::par_unseq, this->vec_of_filepaths->begin(), this->vec_of_filepaths->end(), [this, &mu_for_disp_sum](fs::path& _path) {
		parser _parser(_path);	//tut peredaetsa kopiya
		auto result_of_parse = _parser.parse();

		analyzer _analyzer(result_of_parse);
		auto now_cont = _analyzer.calculate_mat_disperse();
		now_cont->pow_all(2);

		mu_for_disp_sum.lock();
		(*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)) += now_cont;
		mu_for_disp_sum.unlock();
	});

	(*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)) /= (now_type)this->vec_of_filepaths->size();
}

void math_core::find_fluctuations()
{
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto mat_disperse_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse);

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->max_cont_size; ++i)
			for (int j = 0; j < this->max_cont_size; ++j)
				for (int k = 0; k <= global_var::COLLOC_DIST; ++k) {
					bool kond1 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) - /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k) * 4/*)*/ > mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size());
					bool kond2 = mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) + /*sqrt(*/mat_disperse_like_piese->get_count_of_concret_collocation(i, j, k) * 4/*)*/ < mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size());
					if (kond1 || kond2){
						#pragma omp critical (set_of_fluct_cooloc)
						{
							this->set_of_fluct_cooloc.insert(three_coordinate_structure{ (int)(mat_ozid_like_piese->get_downloaded_range().first + i), (int)j, (short)k });
						}
					}
				}
	}

	std::cout.flush();
	cout << endl << "����� �������������� ����������: " << set_of_fluct_cooloc.size();

	dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->clear_vec();
}

void math_core::shrink_set_of_fluct_cooloc()
{
	tsl::robin_set<three_coordinate_structure> set_for_deleted_colloc;

	for (auto obj : this->set_of_fluct_cooloc)
		if (this->mat_ozidanie->get_count_of_concret_collocation(obj.first_coord, obj.second_coord, obj.k) * this->vec_of_filepaths->size() < global_var::CUTOFF_FR_COLLOC_IN_TEXTS)
			set_for_deleted_colloc.insert(obj);

	for (auto obj : set_for_deleted_colloc)
		this->set_of_fluct_cooloc.erase(obj);

	std::cout.flush();
	cout << endl << "����� �������������� ����������, ����� ������ ������� ��������� " << global_var::CUTOFF_FR_COLLOC_IN_TEXTS << " �����: " << this->set_of_fluct_cooloc.size();
}

void math_core::shrink_set_of_fluct_cooloc_by_rare()
{
	tsl::robin_set<three_coordinate_structure> set_for_deleted_colloc;

	unordered_set<pair<int, int>> set_for_colloc_without_dist;

	for (auto obj : this->set_of_fluct_cooloc)
		set_for_colloc_without_dist.insert(make_pair(obj.first_coord, obj.second_coord));

	for (auto obj : set_for_colloc_without_dist) {
		short colloc_dist_counter = 0;
		for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
			if (this->set_of_fluct_cooloc.find(three_coordinate_structure{ obj.first, obj.second, (short)i }) != this->set_of_fluct_cooloc.end())
				++colloc_dist_counter;

		if (colloc_dist_counter < global_var::CUTOFF_FR_IN_FLUCT)
			for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
				if (this->set_of_fluct_cooloc.find(three_coordinate_structure{ obj.first, obj.second, (short)i }) != this->set_of_fluct_cooloc.end())
					set_for_deleted_colloc.insert(three_coordinate_structure{ obj.first, obj.second, (short)i });
	}

	for (auto obj : set_for_deleted_colloc)
		this->set_of_fluct_cooloc.erase(obj);
	std::cout.flush();
	cout << endl << "����� �������������� ����������, ������������� �� �����������, ����� " << global_var::CUTOFF_FR_IN_FLUCT << " �����: " << this->set_of_fluct_cooloc.size();
}

void math_core::shrink_mat_ozid()
{
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);

	tsl::robin_set<three_coordinate_structure> set_for_delete;

	for (auto obj : *mat_ozid_like_piese->get_vector_ptr()) {
		auto three = mat_ozid_like_piese->split_three_coordinates_from_one(obj.first);
		if (this->set_of_fluct_cooloc.find(three) == this->set_of_fluct_cooloc.end())
			set_for_delete.insert(three);
	}

	for (auto obj : set_for_delete)
		this->mat_ozidanie->erase_concret_colloc(obj.first_coord, obj.second_coord, obj.k);

}

void math_core::calculate_norm_shrinked_mat_ozid()
{
	auto mat_ozid_like_piece = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto shrinked_vec_mat_ozid = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->get_vector_ptr();

	unordered_set<pair<int, int>> set_for_checked_collocs;
	vector<now_type> numbers_for_one_colloc;
	numbers_for_one_colloc.reserve(global_var::COLLOC_DIST);

	for (auto obj : *shrinked_vec_mat_ozid) {

		auto three_coord = mat_ozid_like_piece->split_three_coordinates_from_one(obj.first);

		if (set_for_checked_collocs.find(make_pair(three_coord.first_coord, three_coord.second_coord)) != set_for_checked_collocs.end())
			continue;

		for (int k = 0; k <= global_var::COLLOC_DIST; ++k)
			numbers_for_one_colloc.push_back(mat_ozid_like_piece->get_count_of_concret_collocation(three_coord.first_coord, three_coord.second_coord, k));

		auto max_elem = *std::max_element(numbers_for_one_colloc.begin(), numbers_for_one_colloc.end());

		/*for (int k = 0; k <= global_var::COLLOC_DIST; ++k)
			numbers_for_one_colloc[k] /= max_elem;*/

		std::transform(numbers_for_one_colloc.begin(), numbers_for_one_colloc.end(), numbers_for_one_colloc.begin(), [&](now_type& obj0) {	return obj0 / max_elem;	});

		short counter = 0;
		for (auto obj : numbers_for_one_colloc) {
			if (shrinked_vec_mat_ozid->find(mat_ozid_like_piece->collect_one_coordinate_from_three(three_coord.first_coord, three_coord.second_coord, counter)) == shrinked_vec_mat_ozid->end()) {
				++counter;
				continue;
			}

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
	for (auto obj : *this->set_for_unique_terms)
	{
		this->helper_map_for_SVD_rows_colloc_numbers->insert(make_pair(indexer, three_coordinate_structure{ obj, -1, -1 }));
		++indexer;
	}

	unordered_set<pair<int, int>> u_set_for_shrinked_collocs_two_coordinate;

	for (auto obj : *mat_ozid_like_piese->get_vector_ptr())
		u_set_for_shrinked_collocs_two_coordinate.insert(make_pair(mat_ozid_like_piese->split_three_coordinates_from_one(obj.first).first_coord, mat_ozid_like_piese->split_three_coordinates_from_one(obj.first).second_coord));

	for (auto obj : u_set_for_shrinked_collocs_two_coordinate) {
		this->helper_map_for_SVD_rows_colloc_numbers->insert(make_pair(indexer, three_coordinate_structure{ obj.first, obj.second, -1 }));
		++indexer;
	}

	analyzer::set_helper_map_for_SVD_rows_colloc_numbers(this->helper_map_for_SVD_rows_colloc_numbers);

	this->set_of_fluct_cooloc.clear();
}

/*void math_core::find_SVD_colloc()
{
	dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse)->clear_vec();

	this->colloc_and_terms_after_SVD = make_shared<tsl::robin_set<three_coordinate_structure>>();
	analyzer::set_colloc_and_terms_after_SVD(this->colloc_and_terms_after_SVD);

	this->calculate_map_of_flukt_cooloc_fuzzy();

	size_t number_of_collocs = this->helper_map_for_SVD_rows_colloc_numbers->size() - this->set_for_unique_terms->size();

	size_t svd_piece = this->helper_map_for_SVD_rows_colloc_numbers->size();

	if ((this->helper_map_for_SVD_rows_colloc_numbers->size()) > (SVD_PIECE))
		svd_piece = SVD_PIECE; 

	if((this->helper_map_for_SVD_rows_colloc_numbers->size() - svd_piece) < this->vec_of_filepaths->size())
		svd_piece = this->helper_map_for_SVD_rows_colloc_numbers->size();


	size_t pieces = 0;

	if (svd_piece == this->helper_map_for_SVD_rows_colloc_numbers->size())
		pieces = 0;
	else
		pieces = number_of_collocs / (svd_piece - this->set_for_unique_terms->size());	//opasnoe delenie na 0

	if (number_of_collocs / (svd_piece - this->set_for_unique_terms->size()) < 0)
	{
		std::cout.flush();
		cout << endl << "���������� ������ ��������� ���������� ��� ��� �����";
		exit(-4);
	}

	std::cout.flush();
	cout << endl << "����� ���������� ����� ������������� � " << pieces + 1 << " �����";

	vector<float> only_terms_mass;
	only_terms_mass.resize(this->vec_of_filepaths->size() * this->max_cont_size, 0.f);

	analyzer::set_only_terms_mass(only_terms_mass.data());

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j0 = 0; j0 < this->vec_of_filepaths->size(); ++j0) {
			parser _parser((*this->vec_of_filepaths)[j0]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_matrix_only_for_terms(j0);
		}
	}

	vector<float> term_array;
	term_array.reserve(this->vec_of_filepaths->size() * this->set_for_unique_terms->size());

	for (auto obj : *this->set_for_unique_terms)
		for (int j1 = 0; j1 < this->vec_of_filepaths->size(); ++j1)
			term_array.push_back(analyzer::get_only_terms_mass()[obj * this->vec_of_filepaths->size() + j1]);

	vector<float> tmp;
	only_terms_mass.swap(tmp);

	vector<float> colloc_array;

	for (auto p = 0; p <= pieces; ++p) {
		if (p != pieces)
			//colloc_array.resize((svd_piece - this->set_for_unique_terms->size()) * this->vec_of_filepaths->size(), 0.f);
			colloc_array.resize(svd_piece * this->vec_of_filepaths->size() - term_array.size(), 0.f);
		else
			colloc_array.resize((number_of_collocs - p * (svd_piece - this->set_for_unique_terms->size())) * this->vec_of_filepaths->size(), 0.f);

		static auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);

		if (p)
			this->calculate_map_of_flukt_cooloc_fuzzy();

		#pragma omp parallel 
		{
			#pragma omp for schedule(static)
				for (int j2 = 0; j2 < this->vec_of_filepaths->size(); ++j2) {
					parser _parser((*this->vec_of_filepaths)[j2]);	//tut peredaetsa kopiya
					auto result_of_parse = _parser.parse();

					analyzer _analyzer(result_of_parse);
					auto column = _analyzer.calculate_SVD_matrix_for_concret_text();

					for (int i2 = p * (svd_piece - this->set_for_unique_terms->size()); i2 < (svd_piece - this->set_for_unique_terms->size()); ++i2)
					{
						colloc_array[i2 * this->vec_of_filepaths->size() + j2] = (*column)(i2, 0);

						#pragma omp critical (mia)
						{
							auto iter = this->helper_map_for_SVD_rows_colloc_numbers->find(i2);
							mat_ozid_like_piese->erase_concret_colloc(iter->second.first_coord, iter->second.second_coord, iter->second.k);
						}
					}
				}
		}	

		vector<float> svd_array;
		svd_array.resize(term_array.size() + colloc_array.size(), 0.f);

		std::copy(term_array.begin(), term_array.end(), svd_array.begin());
		std::copy(colloc_array.begin(), colloc_array.end(), svd_array.begin() + term_array.size());

		colloc_array.clear();
		colloc_array.shrink_to_fit();

		this->SVD_colloc_algorithm(svd_array.data(), svd_array.size() / this->vec_of_filepaths->size());

		svd_array.clear();
		svd_array.shrink_to_fit();

		std::cout.flush();
		cout << endl << "���������� ���������� ����� ���������� � ���������� " << KOEF_FOR_COLLOC_COS_DELETE << " :" << this->cosinuses.size();

		for (auto obj : this->cosinuses)
			this->colloc_and_terms_after_SVD->insert(this->helper_map_for_SVD_rows_colloc_numbers->find(obj.first.first).value());

		this->cosinuses.clear();
		std::cout.flush();
		cout << endl << "���������� ���������� ����� ���������� � �������, � ���������� " << KOEF_FOR_COLLOC_COS_DELETE << " :" << this->colloc_and_terms_after_SVD->size();
	}
	
	auto blyadovka1 = 1;
}*/

void math_core::SVD_colloc_algorithm(now_type* arr, size_t rows, bool is_this_SVD_for_terms)
{
	size_t m = rows;
	size_t n = this->vec_of_filepaths->size();
	size_t lda = n;
	size_t ldu = m;
	size_t ldvt = n;
	now_type* superb;

	if (m < n)
		superb = new now_type[m - 1]{};
	else
		superb = new now_type[n - 1]{};

	int* istat = new int[3];

	now_type* s = new now_type[n] {};
	now_type* u = new now_type[ldu * m]{};
	now_type* vt = new now_type[ldvt * n]{};
	now_type* a = arr;

	int res = LAPACKE_sgejsv(LAPACK_ROW_MAJOR, 'C', 'F', 'J', 'N', 'N', 'N', m, n, a, lda, s, u, ldu, vt, ldvt, superb, istat);

	if (res) {
		cout << endl << "� ��� ���-�� �� ���, ��� ������: " << res;
		system("pause");
	}

	delete[] s;

	shared_ptr<MatrixXf> resized_V_matrix_of_SVD = make_shared<MatrixXf>();
	resized_V_matrix_of_SVD->resize(global_var::COLLOC_DIST + (int)1, n);

	for (int j = 0; j < n; ++j)
		for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
			resized_V_matrix_of_SVD->operator()(i, j) = vt[j * n + i];

	delete[] vt;

	shared_ptr<MatrixXf> resized_U_matrix_of_SVD = make_shared<MatrixXf>();
	resized_U_matrix_of_SVD->resize(m, global_var::COLLOC_DIST + (int)1);

	int starter = 0;
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j <= global_var::COLLOC_DIST; ++j)
			resized_U_matrix_of_SVD->operator()(i, j) = u[starter + j];

		starter += n;
	}

	delete[] u;

	// ������ ����� � ����������
	vector<float> lenghts_colloc_vector;
	lenghts_colloc_vector.resize(m, 0.f);

	for (auto i = 0; i < resized_U_matrix_of_SVD->rows(); ++i) {
		for (auto j = 0; j < resized_U_matrix_of_SVD->cols(); ++j)
			lenghts_colloc_vector[i] += (*resized_U_matrix_of_SVD)(i, j) * (*resized_U_matrix_of_SVD)(i, j);

		lenghts_colloc_vector[i] = sqrt(lenghts_colloc_vector[i]); // ���������� ���� �������� ��������
	}

	vector<float> lenghts_texts_vector;
	lenghts_texts_vector.resize(n, 0.f);

	for (auto i = 0; i < resized_V_matrix_of_SVD->cols(); ++i) {
		for (auto j = 0; j < resized_V_matrix_of_SVD->rows(); ++j)
			lenghts_texts_vector[i] += (*resized_V_matrix_of_SVD)(j, i) * (*resized_V_matrix_of_SVD)(j, i);

		lenghts_texts_vector[i] = sqrt(lenghts_texts_vector[i]); // ���������� ���� �������� �������
	}

	unordered_map<pair<int, int>, float> scalar_proizv;

	for (auto k = 0; k < resized_V_matrix_of_SVD->cols(); ++k)
		for (auto i = 0; i < resized_U_matrix_of_SVD->rows(); ++i)
			for (auto j = 0; j < resized_U_matrix_of_SVD->cols(); ++j) {
				auto iter = scalar_proizv.find(make_pair(i, k));
				if (iter == scalar_proizv.end())
					scalar_proizv.insert(make_pair(make_pair(i, k), (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(j, k))); // ������� ���������� ������������ �� �����������
				else
					scalar_proizv[make_pair(i, k)] = iter->second + (*resized_U_matrix_of_SVD)(i, j) * (*resized_V_matrix_of_SVD)(j, k);
			}


	for (int i = 0; i < lenghts_colloc_vector.size(); ++i)
		for (int j = 0; j < lenghts_texts_vector.size(); ++j)
			this->cosinuses.insert(make_pair(make_pair(i, j), (scalar_proizv[make_pair(i, j)] / lenghts_colloc_vector[i] / lenghts_texts_vector[j])));

	scalar_proizv.clear();

	lenghts_texts_vector.clear();
	lenghts_texts_vector.shrink_to_fit();
	lenghts_colloc_vector.clear();
	lenghts_colloc_vector.shrink_to_fit();

	list<pair<int, int>> list_of_terms_will_be_deleted;
	if (is_this_SVD_for_terms) {
		//cout << endl << "����� ���������: " << this->cosinuses.size();
		for (auto& obj : this->cosinuses)
			if (obj.second < global_var::KOEF_FOR_COLLOC_COS_DELETE && (this->cosinuses.find(obj.first) != this->cosinuses.end()))
				list_of_terms_will_be_deleted.push_back(obj.first);

		for (auto& obj : list_of_terms_will_be_deleted)
			this->cosinuses.erase(obj);

		//cout << endl << "����� ��������� ����� ��������: " << this->cosinuses.size();
	}
	else {
		for (auto i = global_var::KOEF_FOR_COLLOC_COS_DELETE; i < (1.f - 0.01f); i += 0.01f) {
			for (auto& obj : this->cosinuses)
				if (obj.second < i && (this->cosinuses.find(obj.first) != this->cosinuses.end()))
					list_of_terms_will_be_deleted.push_back(obj.first);

			for (auto& obj : list_of_terms_will_be_deleted)
				this->cosinuses.erase(obj);

			for (auto obj : this->cosinuses)
				this->colloc_and_terms_after_SVD->insert(this->helper_map_for_SVD_rows_colloc_numbers->find(obj.first.first).value());

			std::cout.flush();
			cout << endl << "���������� �������� ������/���������� ����� ���������� � ������� tf/idf, � ���������� " << i << " :" << this->colloc_and_terms_after_SVD->size();

			this->colloc_and_terms_after_SVD->clear();
		}
	}
}

void math_core::find_SVD_terms()
{
	this->idf_matrix = make_shared<vector<now_type>>();
	this->idf_matrix->resize(this->max_cont_size, 0.f);
	analyzer::set_idf_matrix(this->idf_matrix);

	this->tf_matrix = make_shared<vector<vector<now_type>>>();
	this->tf_matrix->resize(this->max_cont_size);

	for (auto& obj : *this->tf_matrix)
		obj.resize(this->vec_of_filepaths->size(), 0.f);

	analyzer::set_tf_matrix(this->tf_matrix);

	/*#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_tf_matrix_for_only_terms(j);
		}
	}*/

	auto first_it = this->vec_of_filepaths->begin();
	for_each(execution::par_unseq, boost::counting_iterator<int>(0), boost::counting_iterator<int>(this->vec_of_filepaths->size()), [first_it](auto&& index) {
		parser _parser(first_it[index]);	//tut peredaetsa kopiya
		auto result_of_parse = _parser.parse();

		analyzer _analyzer(result_of_parse);
		_analyzer.calculate_tf_matrix_for_only_terms(index);
	});

	for (size_t i = 0; i < this->max_cont_size; ++i) 
		(*this->idf_matrix)[i] = std::count_if((*this->tf_matrix)[i].begin(), (*this->tf_matrix)[i].end(), [=](now_type obj) {
			if (obj != 0)
				return true;
			else
				return false;
		});

	for (size_t i = 0; i < this->idf_matrix->size(); ++i)
		if((*this->idf_matrix)[i] != 0)
			(*this->idf_matrix)[i] = log2f(this->vec_of_filepaths->size() / ((*this->idf_matrix)[i]));

	for (size_t j = 0; j < this->tf_matrix->begin()->size(); ++j)
		for (size_t i = 0; i < this->idf_matrix->size(); ++i)
			(*this->tf_matrix)[i][j] *= (*this->idf_matrix)[i];

	this->idf_matrix->clear();
	this->idf_matrix->shrink_to_fit();

	vector<now_type> tf_like_row;
	tf_like_row.resize(this->tf_matrix->begin()->size() * this->tf_matrix->size(), 0.f);

	for (size_t j = 0; j < this->tf_matrix->size(); ++j)
		for (size_t i = 0; i < this->tf_matrix->begin()->size(); ++i)
			tf_like_row[(size_t)(j * this->tf_matrix->begin()->size() + i)] = (*this->tf_matrix)[j][i];

	for (auto& obj : *this->tf_matrix) {
		obj.clear();
		obj.shrink_to_fit();
	}

	vector<now_type> tmp;
	this->tf_matrix->clear();
	this->tf_matrix->shrink_to_fit();

	//���������� ������� � SVD	tf_like_row
	this->SVD_colloc_algorithm(tf_like_row.data(), tf_like_row.size() / this->vec_of_filepaths->size(), true);

	tf_like_row.clear();

	cout << endl << "���������� �������� ������ ����� ���������� tf/idf, � ���������� " << global_var::KOEF_FOR_COLLOC_COS_DELETE << " :" << this->cosinuses.size();

	this->set_for_unique_terms = make_shared<unordered_set<int>>();

	for (auto obj : this->cosinuses)
		this->set_for_unique_terms->insert(obj.first.first);

	this->cosinuses.clear();
	std::cout.flush();
	cout << endl << "���������� �������� ������ ����� ���������� � ������� tf/idf, � ���������� " << global_var::KOEF_FOR_COLLOC_COS_DELETE << " :" << this->set_for_unique_terms->size();

}

void math_core::calculate_tf_idf()
{
	this->calculate_map_of_flukt_cooloc_fuzzy();

	this->idf_matrix = make_shared<vector<now_type>>();
	this->idf_matrix->resize(this->helper_map_for_SVD_rows_colloc_numbers->size(), 0.f);
	analyzer::set_idf_matrix(this->idf_matrix);

	this->tf_matrix = make_shared<vector<vector<now_type>>>();
	this->tf_matrix->resize(this->helper_map_for_SVD_rows_colloc_numbers->size());
	//this->colloc_and_terms_after_SVD->size()
	for (auto& obj : *this->tf_matrix)
		obj.resize(this->vec_of_filepaths->size(), 0.f);

	analyzer::set_tf_matrix(this->tf_matrix);

	/*#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_idf_tf_matrix(j);
		}
	}*/

	auto first_it = this->vec_of_filepaths->begin();
	for_each(execution::par_unseq, boost::counting_iterator<int>(0), boost::counting_iterator<int>(this->vec_of_filepaths->size()), [first_it](auto&& index) {
		parser _parser(first_it[index]);	//tut peredaetsa kopiya
		auto result_of_parse = _parser.parse();

		analyzer _analyzer(result_of_parse);
		_analyzer.calculate_idf_tf_matrix(index);
	});

	for (size_t i = 0; i < this->idf_matrix->size(); ++i)
		(*this->idf_matrix)[i] = log2f(this->vec_of_filepaths->size() / ((*this->idf_matrix)[i] + 1));
}

void math_core::prepare_tf_matrix_for_SVD()
{
	this->colloc_and_terms_after_SVD = make_shared<tsl::robin_set<three_coordinate_structure>>();

	for (size_t j = 0; j < this->tf_matrix->begin()->size(); ++j)
		for (size_t i = 0; i < this->idf_matrix->size(); ++i)
			(*this->tf_matrix)[i][j] *= (*this->idf_matrix)[i];

	//������� ��� ���

	this->idf_matrix->clear();
	this->idf_matrix->shrink_to_fit();

	vector<now_type> tf_like_row;
	tf_like_row.resize(this->tf_matrix->begin()->size() * this->tf_matrix->size(), 0.f);

	for (size_t j = 0; j < this->tf_matrix->size(); ++j)
		for (size_t i = 0; i < this->tf_matrix->begin()->size(); ++i)
			tf_like_row[(size_t)(j * this->tf_matrix->begin()->size() + i)] = (*this->tf_matrix)[j][i];

	for (auto& obj : *this->tf_matrix) {
		vector<now_type> tmp;
		obj.swap(tmp);
	}

	vector<now_type> tmp;
	this->tf_matrix->clear();
	this->tf_matrix->shrink_to_fit();

	//���������� ������� � SVD	tf_like_row
	this->SVD_colloc_algorithm(tf_like_row.data(), tf_like_row.size() / this->vec_of_filepaths->size(), false);

	this->colloc_and_terms_after_SVD = make_shared<tsl::robin_set<three_coordinate_structure>>();

	tf_like_row.clear();

	cout << endl << "���������� �������� ������/���������� ����� ���������� tf/idf, � ���������� " << 0.99 << " :" << this->cosinuses.size();

	for (auto obj : this->cosinuses)
		this->colloc_and_terms_after_SVD->insert(this->helper_map_for_SVD_rows_colloc_numbers->find(obj.first.first).value());

	this->cosinuses.clear();
	std::cout.flush();
	cout << endl << "���������� �������� ������/���������� ����� ���������� � ������� tf/idf, � ���������� " << 0.99 << " :" << this->colloc_and_terms_after_SVD->size();

	ofstream _fout("high-value terms and collocs.txt");

	cout << endl << "������ ������ ����� �� ����-�������: " << parser::stop_words.size();

	auto mat_ozid_like_piece = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto shrinked_vec_mat_ozid = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->get_vector_ptr();

	unordered_set<pair<int, int>> outputed_coolocs;

	std::ofstream csv_file("norm_shrinked_mat_ozid.csv");

	for (auto obj : *this->colloc_and_terms_after_SVD) {
		if (obj.second_coord == -1 || (outputed_coolocs.find(make_pair(obj.first_coord, obj.second_coord)) != outputed_coolocs.end()))
			continue;

		csv_file << analyzer::get_word_for_token(obj.first_coord) << ";" << analyzer::get_word_for_token(obj.second_coord) << ";";

		for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
			csv_file << to_string(mat_ozid_like_piece->get_count_of_concret_collocation(obj.first_coord, obj.second_coord, i)) << ";";

		outputed_coolocs.insert(make_pair(obj.first_coord, obj.second_coord));
		csv_file << "\n";
	}

	csv_file.close();
	outputed_coolocs.clear();
	dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie)->clear_vec();

	for (auto obj : *this->colloc_and_terms_after_SVD)
		if (obj.second_coord != -1) {
			_fout << analyzer::get_word_for_token(obj.first_coord) << " " << analyzer::get_word_for_token(obj.second_coord) << endl;
			
			/*if (parser::stop_words.find(analyzer::get_word_for_token(obj.first_coord)) != parser::stop_words.end() || (parser::stop_words.find(analyzer::get_word_for_token(obj.second_coord)) != parser::stop_words.end()))
				cout << endl << "��������� ��������. � ����� �������� ����-�����";

			if ((analyzer::get_word_for_token(obj.first_coord).find("�") != string::npos && (analyzer::get_word_for_token(obj.first_coord).size() >= 2 && analyzer::get_word_for_token(obj.first_coord).size() <= 4))) {
				cout << endl << "�������������� �������: ";
				for (auto obj : analyzer::get_word_for_token(obj.first_coord))
					cout << (int)obj << " ";
			}

			if((analyzer::get_word_for_token(obj.second_coord).find("�") != string::npos && (analyzer::get_word_for_token(obj.second_coord).size() >= 2 && analyzer::get_word_for_token(obj.second_coord).size() <= 4))) {
				cout << endl << "�������������� �������: ";
				for (auto obj : analyzer::get_word_for_token(obj.second_coord))
					cout << (int)obj << " ";
			}*/
		}
		else {
			_fout << analyzer::get_word_for_token(obj.first_coord) << endl;
		}

	this->colloc_and_terms_after_SVD->clear();
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

shared_ptr<unordered_set<int>> math_core::get_set_for_unique_terms() const
{
	return this->set_for_unique_terms;
}
