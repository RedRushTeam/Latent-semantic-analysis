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

	void calculate_max_cont_size();
	int calculate_max_cont_size_without_rare_words();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts();
	int calculate_max_cont_size_without_rare_words_and_frequency_in_texts_and_SVD();
	void calculate_sample_mean();
	void calculate_mat_ozidanie();
	void calculate_mat_disperse();
	void calculate_sredne_kv_otklonenie();
	void calculate_sredne_kv_otklonenie_fixed();
	void calculate_asymmetry_coefficient();	//nn
	void calculate_excess_ratio();	//nn
	void find_fluctuations();
	void find_colloc_fluctuations();

	//getters&setters
	int get_max_cont_size() const;

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
	shared_ptr<container_class_interface> _all_texts_on_diagonal;
	shared_ptr<container_class_interface> mat_disperse;
	shared_ptr<container_class_interface> mat_ozidanie;

	shared_ptr<container_class_interface> sparse_sample_mean_all;
	shared_ptr<container_class_interface> sparse_excess_ratio;
	shared_ptr<container_class_interface> sparse_asymmetry_coefficient;
	shared_ptr<container_class_interface> sparse_all_texts_on_diagonal;
	shared_ptr<container_class_interface> sparse_SKO;
	shared_ptr<container_class_interface> sparse_mat_disperse;
	shared_ptr<container_class_interface> sparse_mat_ozidanie;

	int number_of_texts;
	int max_cont_size = 0;
	int number_of_slices;
};

