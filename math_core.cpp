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

	//this->max_cont_size = analyzer::get_counter_of_tokenizer();

	//this->number_of_slices = ceil(SIZE_OF_PIECE / this->max_cont_size);
}

void math_core::calculate_sample_mean()
{
	this->sample_mean_all = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "zati4ka");
	analyzer::set_container_sample_mean_all(this->sample_mean_all);
	
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

		(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())) /= ((now_type)this->vec_of_filepaths->size());

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->upload_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->fill_vector((now_type)0.0);
	}

	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_sample_mean_all())->clear_vec();
}

void math_core::calculate_mat_ozidanie()
{
	this->mat_ozidanie = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "zati4ka");
	analyzer::set_container_mat_ozidanie(this->mat_ozidanie);

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices)
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, this->max_cont_size));
		else
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, (i + 1) * SIZE_OF_PIECE));

		#pragma omp parallel 
		{
			#pragma omp for schedule(static)
			for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
				parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
				auto result_of_parse = _parser.parse();
				cout << i << " ";
				analyzer _analyzer(result_of_parse);
				_analyzer.calculate_mat_ozidanie();
			}
		}

		(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())) /= ((now_type)this->divider(this->vec_of_filepaths->size() * (2 + 2 * COLLOC_DIST)));

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->upload_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->fill_vector((now_type)0.0);
	}

	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->clear_vec();
}

void math_core::calculate_mat_disperse()
{
	this->mat_disperse = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "zati4ka");
	analyzer::set_container_mat_disperse(this->mat_disperse);

	for (int i = 0; i < this->number_of_slices; ++i) {
		if (i + 1 == this->number_of_slices)
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, this->max_cont_size));
		else
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(i * SIZE_OF_PIECE, (i + 1) * SIZE_OF_PIECE));

		#pragma omp parallel 
		{
			#pragma omp for schedule(static)
			for (int j = 0; j < this->vec_of_filepaths->size(); ++j) {
				parser _parser((*this->vec_of_filepaths)[j]);	//tut peredaetsa kopiya
				auto result_of_parse = _parser.parse();
				cout << i << " ";
				analyzer _analyzer(result_of_parse);
				_analyzer.calculate_mat_disperse();
			}
		}

		(*dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())) /= ((now_type)this->divider(this->vec_of_filepaths->size() * (2 + 2 * COLLOC_DIST)));

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->upload_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->fill_vector((now_type)0.0);
	}

	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->clear_vec();

	int now_size_of_peace = SIZE_OF_PIECE / 2;	//There may be data loss due to incorrect rounding
	size_t size_of_slices = (int)ceil((float)this->max_cont_size / (float)now_size_of_peace);

	for (int j = 0; j < size_of_slices; ++j) {
		if (j + 1 == size_of_slices) {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(j * now_size_of_peace, this->max_cont_size));
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(j * now_size_of_peace, this->max_cont_size));
		}
		else {
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->set_downloaded_range(make_pair(j * now_size_of_peace, (j + 1) * now_size_of_peace));
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_downloaded_range(make_pair(j * now_size_of_peace, (j + 1) * now_size_of_peace));
		}

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->download_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->download_vec();

		for (size_t i = 0; i < dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->get_vector_size(); ++i)
			dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->set_count_of_concret_collocation_with_one_coordinate(i, dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->get_count_of_concret_collocation_with_one_coordinate(i) - dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->get_count_of_concret_collocation_with_one_coordinate(i) * dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->get_count_of_concret_collocation_with_one_coordinate(i));
	
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->upload_vec();
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->upload_vec();

		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->fill_vector((now_type)0.0);
		dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->fill_vector((now_type)0.0);
	}

	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_ozidanie())->clear_vec();
	dynamic_pointer_cast<piecewise_container_class>(analyzer::get_container_mat_disperse())->clear_vec();
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
	/*auto sum1 = this->calculate_parametr_to_one_term(this->mat_ozidanie) + this->calculate_parametr_to_one_term(this->sredne_kv_otklonenie_fixed);
	auto razn1 = this->calculate_parametr_to_one_term(this->mat_ozidanie) - this->calculate_parametr_to_one_term(this->sredne_kv_otklonenie_fixed);
	hard_container_class chart1;
	auto keks = this->max_cont_size;
	chart1.give_space(keks, GAP);

	for (int i = 0; this->vec_of_hard_container_class.size() > i; ++i) {
		this->prepare_data_in_container_class(i);
		for (auto q = 0; q < keks; ++q)
			for (auto j = 0; j < keks; ++j)
				for (auto p = -GAP - 1; p <= GAP; ++p)
					chart1[q][q][p] = chart1[q][q][p] + this->vec_of_hard_container_class[i][q][j][p];
		vec_of_hard_container_class[i].clear();
	}


	auto tmp1 = chart1 / vec_of_hard_container_class.size();
	chart1.clear();
	chart1 = tmp1;
	analyzer helper;
	helper.set_map_of_tokens("dictionary.txt");
	ofstream ff("fluctuation.txt");
	for (int i = 1; i < chart1.get_counter_of_tokenizer(); i++)
		for (int l = -GAP - 1; l <= GAP; ++l)
			if ((chart1[i][i][l] > sum1[i][i][l]) || (chart1[i][i][l] < razn1[i][i][l])) {
				for (auto q : helper.get_map_of_tokens())
					if (q.second == i) {
						ff << q.first << " ";
						break;
					}
				break;
			}
	chart1.clear();*/
}

shared_ptr<container_class_interface> math_core::calculate_parametr_to_one_term(shared_ptr<container_class_interface> _parametr)
{
	//todo
	auto one_term_matrix = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size, "zati4ka");

	for (auto q = 0; q < _parametr->get_count_of_collocations(); ++q)
		for (auto j = 0; j < _parametr->get_count_of_collocations(); ++j)
			for (auto p = -COLLOC_DIST - 1; p <= COLLOC_DIST; ++p)
					one_term_matrix->set_count_of_concret_collocation(q, q, p, one_term_matrix->get_count_of_concret_collocation(q, q, p) + _parametr->get_count_of_concret_collocation(q, j, p));

	return one_term_matrix;
}

int math_core::get_max_cont_size() const
{
	return this->max_cont_size;
}
