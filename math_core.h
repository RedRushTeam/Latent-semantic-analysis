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

	//SVD methods
	void calculate_SVD_matrix();
	VectorXf calculate_Singular_Value();
	MatrixXf get_singular_V_matrix();
	MatrixXf get_singular_U_matrix();
	void calculate_colloc_SVD();
	VectorXf calculate_colloc_Singular_Value();
	MatrixXf get_colloc_singular_V_matrix();
	MatrixXf get_colloc_singular_U_matrix();
	vector<pair<int, int>>* get_helper_vec();

	int calculate_max_cont_size();	//first
	int calculate_max_cont_size_without_rare_words();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts_and_SVD();
	void calculate_sample_mean();	//second
	void calculate_mat_ozidanie();	//third
	void calculate_mat_disperse();	//4
	void calculate_sredne_kv_otklonenie();	//5
	void calculate_sredne_kv_otklonenie_fixed();	//6
	void calculate_asymmetry_coefficient();	//nn
	void calculate_excess_ratio();	//nn
	void out_for_chart();	//nn
	void find_fluctuations();	//7
	void find_colloc_fluctuations();	//8

	//getters&setters
	shared_ptr<container_class_interface> get_sredne_kv_otklonenie() const;
	shared_ptr<container_class_interface> get_sredne_kv_otklonenie_fixed() const;
	shared_ptr<container_class_interface> get_mat_ozidanie() const;
	shared_ptr<container_class_interface> get_mat_disperse() const;
	shared_ptr<container_class_interface> get_asymmetry_coefficient() const;
	shared_ptr<container_class_interface> get_excess_ratio() const;
	shared_ptr<container_class_interface> get_sample_mean_all() const;
	int get_length_of_all_container_class() const;
	shared_ptr<container_class_interface> calculate_parametr_to_one_term(shared_ptr<container_class_interface> _parametr);
	shared_ptr<container_class_interface> calculate_parametr_to_one_colloc(shared_ptr<container_class_interface> _parametr);
	int get_max_cont_size() const;

	//bool remove_container_class(shared_ptr<container_class_interface> _shared_ptr<container_class_interface>);	//fix this!

private:
	//private methods
	now_type divider(int size);

	//private elems for SVD
	shared_ptr<MatrixXf> m;
	shared_ptr<BDCSVD<MatrixXf>> BDCSVD_svd = nullptr;		//SVD for big matrix

	//for colloc
	MatrixXf* m_colloc_matrix;
	BDCSVD<MatrixXf>* BDCSVD_svd_colloc = nullptr;		//SVD for big matrix
	vector<pair<int, int>>* helper_vec;
	multiset<pair<int, int>>* possible_collocs;

	//private elems
	shared_ptr<vector<fs::path>> vec_of_filepaths;

	shared_ptr<container_class_interface> sample_mean_all;
	shared_ptr<container_class_interface> excess_ratio;
	shared_ptr<container_class_interface> asymmetry_coefficient;
	shared_ptr<container_class_interface> sredne_kv_otklonenie_fixed;
	shared_ptr<container_class_interface> sredne_kv_otklonenie;
	shared_ptr<container_class_interface> mat_disperse;
	shared_ptr<container_class_interface> mat_ozidanie;
	int number_of_texts;
	int max_cont_size = 0;
	int number_of_slices;
};

