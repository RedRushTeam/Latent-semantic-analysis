#pragma once
#include "parser.h"

class analyzer
{
public:

	//constr
	analyzer() {}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text) : 
		list_of_all_parsed_text(list_of_all_parsed_text) {}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text, shared_ptr<container_class_interface> _container_class) : 
		list_of_all_parsed_text(list_of_all_parsed_text) {}

	//methods
	static void create_lemmatizer();
	void calculate_counter_of_tokenizer();							//LV1
	void calculate_counter_of_tokenizer_without_rare_words();		//LV2
	static void initialize_matrix_for_SVD();						//LV3(1)
	void calculate_counter_of_tokenizer_SVD_words();				//LV3(2)
	list<string>::iterator move_list_iterator(list<string>::iterator _it, int mover);
	static string get_word_for_token(int token);
	//methods for math calculate
	void calculate_mat_ozidanie();
	shared_ptr<container_class_interface> calculate_mat_disperse();
	shared_ptr<MatrixXf> calculate_SVD_matrix_for_concret_text();

	//getters&&setters
	static int get_k();
	static void set_k(short _k);
	static int get_number_of_texts();
	static void set_number_of_texts(int number_of_texts);
	static void set_helper_map_for_SVD_rows_colloc_numbers(shared_ptr<tsl::robin_map<pair<int, int>, int>> helper_map_for_SVD_rows_colloc_numbers);
	static void set_map_of_flukt_cooloc_fuzzy(shared_ptr<tsl::robin_map<pair<int, int>, now_type>> map_of_flukt_cooloc_fuzzy);
	void set_list_of_all_parsed_text(shared_ptr<list<string>> list_of_all_parsed_text);
	//for counters
	static int get_counter_of_tokenizer();
	static int get_counter_of_tokenizer_without_rare_words_with_cutoff(int cutoff);
	static int get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(int cutoff, int cutoff_of_texts);
	static int get_counter_of_tokenizer_without_rare_words_SVD();
	//math getters and setters
	static shared_ptr<container_class_interface> get_container_mat_ozidanie();
	static void set_container_mat_ozidanie(shared_ptr<container_class_interface> _mat_ozidanie);
	static shared_ptr<container_class_interface> get_container_mat_disperse();
	static void set_container_mat_disperse(shared_ptr<container_class_interface> _mat_disperse);

	//destr
	~analyzer() {}

private:
	//private methods
	void lemmatize_all_words();

	//perems
	static inline HLEM lemmas_engine;
	static inline short k;
	static inline int number_of_texts = 0;
	shared_ptr<list<string>> list_of_all_lemmatized_text;
	shared_ptr<list<string>> list_of_all_parsed_text;

	//math containers
	static inline shared_ptr<container_class_interface> _mat_ozidanie;
	static inline shared_ptr<container_class_interface> _mat_disperse;

	//dictionaries
	static inline tsl::robin_map<string, int> map_of_tokens_WORD_TOKEN;
	static inline tsl::robin_map<int, string> map_of_tokens_TOKEN_WORD;
	static inline tsl::robin_map<word_and_number_of_appearances_structure, int, hash_word_and_number_of_appearances_structure, equal_word_and_number_of_appearances_structure, std::allocator<pair<word_and_number_of_appearances_structure, int>>, true> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_;
	static inline tsl::robin_map<int, word_and_number_of_appearances_structure> map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_;
	static inline tsl::robin_map<word_and_number_of_appearances_structure, int> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD;

	//SVD matrix for max size calculation
	static inline shared_ptr<MatrixXf> all_matrix_for_SVD;

	//data for SVD calculation
	static inline shared_ptr<tsl::robin_map<pair<int, int>, int>> helper_map_for_SVD_rows_colloc_numbers;
	static inline shared_ptr<tsl::robin_map<pair<int, int>, now_type>> map_of_flukt_cooloc_fuzzy;

	//max_size
	static inline int counter_of_tokenizer;
	static inline int counter_of_tokenizer_without_rare_words;
	static inline int counter_of_tokenizer_without_rare_words_and_text;
	static inline int counter_of_tokenizer_without_rare_words_SVD;
};