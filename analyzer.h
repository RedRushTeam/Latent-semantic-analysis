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

struct three_coordinate_structure {
	int first_coord;
	int second_coord;
	short k;
};

namespace std
{
	template<> struct hash<three_coordinate_structure>
	{
		typedef three_coordinate_structure argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<int>{}(s.first_coord));
			boost::hash_combine(seed, std::hash<int>{}(s.second_coord));
			boost::hash_combine(seed, std::hash<short>{}(s.k));
			return seed;
		}
	};
}

inline bool operator== (three_coordinate_structure lefty, three_coordinate_structure righty)
{
	if (lefty.first_coord == righty.first_coord && (lefty.second_coord == righty.second_coord) && (lefty.k == righty.k))
		return true;
	return false;
};

struct hash_word_and_number_of_appearances_structure {
	std::size_t operator()(const word_and_number_of_appearances_structure& e) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, std::hash<string>{}(e.word));
		return seed;
	}

	std::size_t operator()(int id) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, std::hash<int>{}(id));
		return seed;
	}
};

struct equal_word_and_number_of_appearances_structure {
	using is_transparent = void;

	bool operator()(const word_and_number_of_appearances_structure& e1, const word_and_number_of_appearances_structure& e2) const {
		return e1.word == e2.word;
	}
};

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
	void calculate_sample_mean_all();
	void calculate_mat_ozidanie();
	void calculate_mat_disperse();

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
	static shared_ptr<container_class_interface> get_container_mat_ozidanie();
	static void set_container_mat_ozidanie(shared_ptr<container_class_interface> _mat_ozidanie);
	static shared_ptr<container_class_interface> get_container_mat_disperse();
	static void set_container_mat_disperse(shared_ptr<container_class_interface> _mat_disperse);
	static shared_ptr<container_class_interface> get_all_texts_on_diagonal();
	static void set_all_texts_on_diagonal(shared_ptr<container_class_interface> _all_texts_on_diagonal);

	//destr
	~analyzer() {
		//sol_DeleteLemmatizator(lemmas_engine);
	}

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
	static inline shared_ptr<container_class_interface> _sample_mean_all;
	static inline shared_ptr<container_class_interface> _mat_ozidanie;
	static inline shared_ptr<container_class_interface> _mat_disperse;
	static inline shared_ptr<container_class_interface> _all_texts_on_diagonal;

	//dictionaries
	static inline tsl::robin_map<string, int> map_of_tokens_WORD_TOKEN;
	static inline tsl::robin_map<int, string> map_of_tokens_TOKEN_WORD;
	static inline tsl::robin_map<word_and_number_of_appearances_structure, int, hash_word_and_number_of_appearances_structure, equal_word_and_number_of_appearances_structure, std::allocator<pair<word_and_number_of_appearances_structure, int>>, true> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_;
	static inline tsl::robin_map<int, word_and_number_of_appearances_structure> map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_;
	static inline tsl::robin_map<word_and_number_of_appearances_structure, int> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD;

	//SVD matrix for max size calculation
	static inline shared_ptr<MatrixXf> all_matrix_for_SVD;

	//max_size
	static inline int counter_of_tokenizer;
	static inline int counter_of_tokenizer_without_rare_words;
	static inline int counter_of_tokenizer_without_rare_words_and_text;
	static inline int counter_of_tokenizer_without_rare_words_SVD;
};