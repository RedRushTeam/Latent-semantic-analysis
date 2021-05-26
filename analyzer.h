#pragma once
#include "parser.h"

struct word_and_number_of_appearances_structure {
	string word;
	int number_of_appearances_of_this_word;
	int number_of_texts_in_which_term_occurs;
};

namespace std
{
	template<> struct hash<word_and_number_of_appearances_structure>
	{
		typedef word_and_number_of_appearances_structure argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<string>{}(s.word));
			return seed;
		}
	};
}

namespace std
{
	template<> struct hash<pair<int, int>>
	{
		typedef pair<int, int> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<int>{}(s.first));
			boost::hash_combine(seed, std::hash<int>{}(s.second));
			return seed;
		}
	};
}

inline bool operator== (word_and_number_of_appearances_structure lefty, word_and_number_of_appearances_structure righty)
{
	if (lefty.word == righty.word)
		return true;
	return false;
};

class analyzer
{
public:

	//constr
	analyzer() {
		//this->create_lemmatizer();
	}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text) : 
		list_of_all_parsed_text(list_of_all_parsed_text) {
		this->_container_class = make_shared<piecewise_container_class>(this->k, this->counter_of_tokenizer, "zati4ka");
		//this->create_lemmatizer();
	}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text, shared_ptr<container_class_interface> _container_class) : 
		list_of_all_parsed_text(list_of_all_parsed_text), _container_class(_container_class){
		//this->create_lemmatizer();
	}

	//methods
	static void create_lemmatizer();
	void calculate_counter_of_tokenizer();							//LV1
	void calculate_counter_of_tokenizer_without_rare_words();		//LV2
	static void initialize_matrix_for_SVD();						//LV3(1)
	void calculate_counter_of_tokenizer_SVD_words();				//LV3(2)
	void analyze_vec_of_tokens();
	list<string>::iterator move_list_iterator(list<string>::iterator _it, int mover);
	//methods for math calculate
	void calculate_sample_mean_all();

	//getters&&setters
	static int get_k();
	static void set_k(short _k);
	static int get_number_of_texts();
	static void set_number_of_texts(int number_of_texts);
	void set_list_of_all_parsed_text(shared_ptr<list<string>> list_of_all_parsed_text);
	//for counters
	static int get_counter_of_tokenizer();
	static int get_counter_of_tokenizer_without_rare_words_with_cutoff(int cutoff);
	static int get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(int cutoff, int cutoff_of_texts);
	static int get_counter_of_tokenizer_without_rare_words_SVD();
	//math getters and setters
	static shared_ptr<container_class_interface> get_container_sample_mean_all();
	static void set_container_sample_mean_all(shared_ptr<container_class_interface> _sample_mean_all);
	//i think that now this unneded
	shared_ptr<container_class_interface> get_container_class();
	void set_container_class(shared_ptr<container_class_interface> _container_class);

	//destr
	~analyzer() {
		//sol_DeleteLemmatizator(lemmas_engine);
	}

private:
	//private methods
	void lemmatize_all_words();

	//perems
	static inline HLEM lemmas_engine;
	shared_ptr<container_class_interface> _container_class;
	static inline short k;
	static inline int number_of_texts = 0;
	shared_ptr<list<string>> list_of_all_lemmatized_text;
	shared_ptr<list<string>> list_of_all_parsed_text;

	//math containers
	static inline shared_ptr<container_class_interface> _sample_mean_all;

	//dictionaries
	static inline unordered_map<string, int> map_of_tokens_WORD_TOKEN;
	static inline unordered_map<int, string> map_of_tokens_TOKEN_WORD;
	static inline unordered_map<word_and_number_of_appearances_structure, int> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_;
	static inline unordered_map<int, word_and_number_of_appearances_structure> map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_;
	static inline unordered_map<word_and_number_of_appearances_structure, int> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD;

	//SVD matrix for max size calculation
	static inline shared_ptr<MatrixXf> all_matrix_for_SVD;

	//max_size
	static inline int counter_of_tokenizer;
	static inline int counter_of_tokenizer_without_rare_words;
	static inline int counter_of_tokenizer_without_rare_words_and_text;
	static inline int counter_of_tokenizer_without_rare_words_SVD;
};