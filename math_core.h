#pragma once
#include "analyzer.h"

class math_core
{
public:
	//constr
	math_core(shared_ptr<vector<fs::path>> vec_of_filepaths) : vec_of_filepaths(vec_of_filepaths), number_of_texts(vec_of_filepaths->size()) {
		analyzer::set_number_of_texts(this->number_of_texts);
	}

	//public methods
	void calculate_all();
	void calculate_max_cont_size();
	int calculate_max_cont_size_without_rare_words();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts_and_SVD();
	void calculate_mat_ozidanie();
	void calculate_mat_disperse();
	//void calculate_asymmetry_coefficient();
	//void calculate_excess_ratio();
	void find_fluctuations();
	void calculate_map_of_flukt_cooloc_fuzzy();
	void find_SVD_colloc();
	void SVD_colloc_algorithm(float* arr, size_t rows);

	//getters&setters
	shared_ptr<container_class_interface> get_mat_ozidanie() const;
	shared_ptr<container_class_interface> get_mat_disperse() const;
	int get_max_cont_size() const;

private:
	//private methods
	now_type divider(int size);

	//for colloc
	tsl::robin_map<pair<int, int>, float> cosinuses;

	//private elems
	shared_ptr<vector<fs::path>> vec_of_filepaths;

	//sets for flukt
	tsl::robin_set<three_coordinate_structure> set_of_fluct_cooloc;
	shared_ptr<tsl::robin_map<pair<int, int>, now_type>> map_of_flukt_cooloc_fuzzy;
	shared_ptr<tsl::robin_map<pair<int, int>, int>> helper_map_for_SVD_rows_colloc_numbers;

	//math cont
	shared_ptr<container_class_interface> mat_disperse;
	shared_ptr<container_class_interface> mat_ozidanie;

	//private perems
	int number_of_texts;
	int max_cont_size = 0;
	int number_of_slices;
};

