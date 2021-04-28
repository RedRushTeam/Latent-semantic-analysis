#pragma once
#include "parser.h"

struct word_and_number_of_appearances_structure {
	string word;
	int number_of_appearances_of_this_word;
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
		this->create_lemmatizer();
	}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text) : 
		list_of_all_parsed_text(list_of_all_parsed_text) {
		this->_container_class = make_shared<piecewise_container_class>(this->k, this->counter_of_tokenizer);
		this->create_lemmatizer();
	}

	analyzer(shared_ptr<list<string>> list_of_all_parsed_text, shared_ptr<container_class_interface> _container_class) : 
		list_of_all_parsed_text(list_of_all_parsed_text), _container_class(_container_class){
		this->create_lemmatizer();
	}

	//methods
	void create_lemmatizer();
	void calculate_counter_of_tokenizer();
	void calculate_counter_of_tokenizer_without_rare_words();
	void analyze_vec_of_tokens();
	list<string>::iterator move_list_iterator(list<string>::iterator _it, int mover);

	//getters&&setters
	int get_k();
	int get_counter_of_tokenizer();
	int get_counter_of_tokenizer_without_rare_words_with_cutoff(int cutoff);
	void set_list_of_all_parsed_text(shared_ptr<list<string>> list_of_all_parsed_text);
	void set_k(short _k);
	shared_ptr<container_class_interface> get_container_class();
	void set_container_class(shared_ptr<container_class_interface> _container_class);

	//destr
	~analyzer() {
		sol_DeleteLemmatizator(lemmas_engine);
	}

	static inline int counter_of_tokenizer;

	static inline int counter_of_tokenizer_without_rare_words;

private:
	//private methods
	void lemmatize_all_words();

	HLEM lemmas_engine;
	shared_ptr<container_class_interface> _container_class;
	static inline int counter = 0;
	static inline short k;
	shared_ptr<list<string>> list_of_all_lemmatized_text;
	shared_ptr<list<string>> list_of_all_parsed_text;
	static inline unordered_map<string, int> map_of_tokens_WORD_TOKEN;
	static inline unordered_map<int, string> map_of_tokens_TOKEN_WORD;
	static inline unordered_map<word_and_number_of_appearances_structure, int> map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_;
};