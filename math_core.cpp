#include "math_core.h"

now_type math_core::divider(int size)
{
	int starter = 1;
	int finisher = size;
	auto sum = (starter + finisher) * (finisher - starter + 1) / 2;
	return sum;
}

void math_core::calculate_all_texts_stats()
{
	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->vec_of_filepaths->size(); ++i) {
			parser _parser((*this->vec_of_filepaths)[i]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			this->vec_of_container_classes.push_back(make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size));

			analyzer _analyzer(result_of_parse, this->vec_of_container_classes[i]);
			_analyzer.analyze_vec_of_tokens();
		}
	}
}

int math_core::calculate_max_cont_size_without_rare_words()
{
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

	analyzer _analyzer;

	return _analyzer.get_counter_of_tokenizer_without_rare_words_with_cutoff(CUTOFF);
}

int math_core::calculate_max_cont_size_without_rare_words_and_frequency_in_texts()
{
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

	analyzer _analyzer;

	return _analyzer.get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(CUTOFF, CUTOFF_FR_IN_TEXTS);
}

void math_core::calculate_max_cont_size()
{
	#pragma omp parallel 
	{
		#pragma omp for schedule(static)
		for (int i = 0; i < this->vec_of_filepaths->size(); ++i) {
			parser _parser((*this->vec_of_filepaths)[i]);	//tut peredaetsa kopiya
			auto result_of_parse = _parser.parse();

			analyzer _analyzer(result_of_parse);
			_analyzer.calculate_counter_of_tokenizer();
		}
	}

	this->max_cont_size = analyzer::counter_of_tokenizer;
}

void math_core::calculate_sample_mean()
{
	this->sample_mean_all = make_shared<piecewise_container_class> (COLLOC_DIST, this->max_cont_size);

	for (int i = 0; this->vec_of_container_classes.size() > i; ++i)
		*this->sample_mean_all += (vec_of_container_classes[i]);

	*this->sample_mean_all /= (now_type)this->vec_of_container_classes.size();
}

void math_core::calculate_mat_ozidanie()
{
	this->mat_ozidanie = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size);

	for (int i = 0; this->vec_of_container_classes.size() > i; ++i)
		*this->mat_ozidanie += this->vec_of_container_classes[i];

	*this->mat_ozidanie /= (now_type)this->divider(this->vec_of_container_classes.size() * (2 + 2 * COLLOC_DIST));
}

void math_core::calculate_mat_disperse()
{
	this->mat_disperse = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size);

	for (int i = 0; this->vec_of_container_classes.size() > i; ++i)
		*this->mat_disperse += vec_of_container_classes[i]->pow_all(2);

	*this->mat_disperse /= (now_type)this->divider(this->vec_of_container_classes.size() * (2 + 2 * COLLOC_DIST));

	*this->mat_disperse -= this->mat_ozidanie->pow_all(2);
}

void math_core::calculate_sredne_kv_otklonenie()
{
	//this->sredne_kv_otklonenie = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size);
	this->sredne_kv_otklonenie = this->mat_disperse->pow_all(2);
}

void math_core::calculate_sredne_kv_otklonenie_fixed()
{
	this->sredne_kv_otklonenie_fixed = (*this->mat_disperse * (this->vec_of_container_classes.size() / (this->vec_of_container_classes.size() - 1)))->sqrt_all();
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
	auto one_term_matrix = make_shared<piecewise_container_class>(COLLOC_DIST, this->max_cont_size);

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
