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
	this->max_cont_size = analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff(CUTOFF);

	this->number_of_slices = (int)ceil((float)this->max_cont_size / (float)SIZE_OF_PIECE);

	return this->max_cont_size;
}

int math_core::calculate_max_cont_size_without_rare_words_and_frequency_in_texts()
{
	this->max_cont_size = analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(CUTOFF, CUTOFF_FR_IN_TEXTS);

	this->number_of_slices = (int)ceil((float)this->max_cont_size / (float)SIZE_OF_PIECE);

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

	this->number_of_slices = (int)ceil((float)this->max_cont_size / (float)SIZE_OF_PIECE);

	return this->max_cont_size;
}

void math_core::calculate_all()
{
	this->mat_ozidanie = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "mat_ozidanie");
	analyzer::set_container_mat_ozidanie(this->mat_ozidanie);

	this->mat_disperse = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "mat_disperse");
	analyzer::set_container_mat_disperse(this->mat_disperse);

	cout << "Всего вычисление будет проиводиться в " << this->number_of_slices * 2 << " этапов: ";

	for (int i = 0; i < this->number_of_slices * 2; ++i) {
		if (i + 1 == this->number_of_slices) {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (SIZE_OF_PIECE / 2), this->max_cont_size));
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(i * (SIZE_OF_PIECE / 2), this->max_cont_size));
		}
		else {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * (SIZE_OF_PIECE / 2), (i + 1) * (SIZE_OF_PIECE / 2)));
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(i * (SIZE_OF_PIECE / 2), (i + 1) * (SIZE_OF_PIECE / 2)));
		}

		this->calculate_mat_ozidanie();
		this->calculate_mat_disperse();
		this->find_fluctuations();

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->fill_vector((now_type)0.0);
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->fill_vector((now_type)0.0);

		cout << "(" << i + 1 << "/" << this->number_of_slices * 2 << ") ";
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
			cout << i << " ";
			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_counter_of_tokenizer_without_rare_words();
		}
	}
}

/*void math_core::calculate_sample_mean()
{
	this->sample_mean_all = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "sample_mean");
	analyzer::set_container_sample_mean_all(this->sample_mean_all);

	this->_all_texts_on_diagonal = make_shared<sparce_container_class>(COLLOC_DIST, this->max_cont_size);
	analyzer::set_all_texts_on_diagonal(this->_all_texts_on_diagonal);	//все на главной диагонали

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices)
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, this->max_cont_size));
		else
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, (i + 1) * SIZE_OF_PIECE));

		#pragma omp parallel 
		{
			#pragma omp for schedule(static)
			for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
				parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
				auto result_of_parse = _parser.parse();
				cout << i << " ";
				analyzer _analyzer(result_of_parse);
				_analyzer.calculate_sample_mean_all();
			}
		}

		dynamic_pointer_cast<sparce_container_class>(analyzer::get_all_texts_on_diagonal())->calculate_and_sum_parametr_to_one_term(this->sample_mean_all);

		(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())) /= ((now_type)this->vec_of_filepaths->size());

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->upload_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->fill_vector((now_type)0.0);
	}

	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->clear_vec();
}*/

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

	*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse) += this->mat_ozidanie;

<<<<<<< HEAD
	(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= (now_type)(this->max_cont_size * (1 + COLLOC_DIST)); // делить на число текстов
=======
	(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= (now_type)this->vec_of_filepaths->size();
>>>>>>> 18f3eda3b31e926d7b8162b9487041dd336d81a1
}

void math_core::calculate_mat_disperse()
{
	*dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse) -= this->mat_ozidanie;
	this->mat_disperse->pow_all(2);	//tut isp pow
	(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= (now_type)this->vec_of_filepaths->size();
}

void math_core::calculate_sredne_kv_otklonenie()
{
	//this->sredne_kv_otklonenie = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size);
	//this->sredne_kv_otklonenie = this->mat_disperse->pow_all(2);
}

void math_core::calculate_sredne_kv_otklonenie_fixed()
{
	//this->sredne_kv_otklonenie_fixed = (*this->mat_disperse * (this->vec_of_filepaths->size() / (this->vec_of_filepaths->size() - 1)))->sqrt_all();
}

void math_core::find_fluctuations()
{
	auto mat_ozid_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_ozidanie);
	auto mat_disp_like_piese = dynamic_pointer_cast<piecewise_container_class>(this->mat_disperse);

	for (size_t i = 0; i < (SIZE_OF_PIECE / 2); ++i)
		for (size_t j = 0; j < (SIZE_OF_PIECE / 2); ++j)
			for (size_t k = 0; k <= COLLOC_DIST; ++k)
				if (mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) - sqrt(mat_disp_like_piese->get_count_of_concret_collocation(i, j, k)) > mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size()) ||
					(mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) + sqrt(mat_disp_like_piese->get_count_of_concret_collocation(i, j, k)) < mat_ozid_like_piese->get_count_of_concret_collocation(i, j, k) * (this->vec_of_filepaths->size())))
					set_of_fluct_cooloc.insert(three_coordinate_structure{ (int)i, (int)j, (short)k });

	mat_ozid_like_piese->clear_vec();
	mat_disp_like_piese->clear_vec();
}

void math_core::calculate_map_of_flukt_cooloc_fuzzy()
{
	this->map_of_flukt_cooloc_fuzzy = make_shared<tsl::robin_map<pair<int, int>, now_type>>();
	
	for (auto& obj : this->set_of_fluct_cooloc) {
		auto iter = this->map_of_flukt_cooloc_fuzzy->find(make_pair(obj.first_coord, obj.second_coord));

		if (iter == this->map_of_flukt_cooloc_fuzzy->end())
			this->map_of_flukt_cooloc_fuzzy->insert(make_pair(make_pair(obj.first_coord, obj.second_coord), 1 - obj.k * 0.2));
		else 
			this->map_of_flukt_cooloc_fuzzy->insert(make_pair(make_pair(obj.first_coord, obj.second_coord), iter->second + (now_type)(1 - obj.k * 0.2)));	//todo check this logic
	}

	this->helper_vec_for_SVD_rows_colloc_numbers = make_shared<vector<pair<int, int>>>();

	this->helper_vec_for_SVD_rows_colloc_numbers->reserve(this->map_of_flukt_cooloc_fuzzy->size());

	for (auto& obj : *this->map_of_flukt_cooloc_fuzzy)
		this->helper_vec_for_SVD_rows_colloc_numbers->push_back(make_pair(obj.first.first, obj.first.second));

	analyzer::set_helper_vec_for_SVD_rows_colloc_numbers(this->helper_vec_for_SVD_rows_colloc_numbers);
	analyzer::set_map_of_flukt_cooloc_fuzzy(this->map_of_flukt_cooloc_fuzzy);

	this->set_of_fluct_cooloc.clear();
}

void math_core::find_SVD_coolc()
{
	this->calculate_map_of_flukt_cooloc_fuzzy();

	MatrixXf matrix_for_all_SVD(this->map_of_flukt_cooloc_fuzzy->size(), this->vec_of_filepaths->size());

	matrix_for_all_SVD.fill(NULL);

	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
			parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			auto column = _analyzer.calculate_SVD_matrix_for_concret_text();

			matrix_for_all_SVD.col(j) = *column;	//может быть тут нужна критическая секция, а может и нет, нужны тесты
		}
	}
}

int math_core::get_max_cont_size() const
{
	return this->max_cont_size;
}
