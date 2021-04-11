#pragma once
#include "parser.h"


class analyzer
{
public:

	//constr
	analyzer() {}
	/*analyzer(std::list<std::string> list_of_all_parsed_text) : list_of_all_parsed_text(list_of_all_parsed_text) {
		_container_class = new hard_container_class;
	}

	analyzer(hard_container_class* _hard_container_class, std::list<std::string> list_of_all_parsed_text) : list_of_all_parsed_text(list_of_all_parsed_text), _container_class(_hard_container_class) {}
	*/
	//methods
	bool is_index_valid(int ind) const;
	void analyze_vec_of_tokens();
	void shape_vec_of_tokens();
	void shape_vec_tokens_of_text();
	void give_space(int maxsize);
	void update_dictionary();
	void upload_data();
	void clear();

	//getters&&setters
	int get_k();
	int get_counter_of_tokenizer();
	void set_list_of_all_parsed_text(std::list<std::string> list_of_all_parsed_text);
	void set_k(char _k);
	shared_ptr<container_class_interface> get_container_class();
	void set_container_class(shared_ptr<container_class_interface> _container_class);
	std::map<std::string, int> get_map_of_tokens();
	void set_map_of_tokens(string dictionary_file);

	//destr
	~analyzer() {
		this->update_dictionary();
		vec_of_tokens.clear();
		list_of_all_parsed_text.clear();
		//delete list_of_all_parsed_text;
	}

	vector<int> vec_of_tokens;

private:
	shared_ptr<container_class_interface> _container_class;
	short k;
	int counter_of_tokenizer;
	std::list<std::string> list_of_all_parsed_text;
	std::map<std::string, int> map_of_tokens;
};

