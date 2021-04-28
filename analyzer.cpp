#include "analyzer.h"

void analyzer::create_lemmatizer()
{
	#pragma omp critical (lemmatizer_download)
	{
		const char* dict_path = NULL;
		const char* language = "russian";

		if (dict_path == NULL)
			dict_path = LEMADR;

		int flags = LEME_DEFAULT;


		printf("Loading the lemmatizator from %s\n", dict_path);

		this->lemmas_engine = sol_LoadLemmatizatorA(dict_path, flags);

		cout << "Proanalizirovano: " << counter << endl;;

		if (this->lemmas_engine == NULL)
		{
			printf("Could not load the lemmatizator from %s\n", dict_path);
			printf("With flag %s\n", flags);
			system("pause");
			exit(1);
		}

		++counter;
	}

	//delete dict_path;
	//delete language;
}

void analyzer::calculate_counter_of_tokenizer()
{
	this->lemmatize_all_words();

		//критическая секция
	for (auto& obj : *this->list_of_all_lemmatized_text) {	//обращение к критическому ресурсу
		#pragma omp critical (maps_into_analyzer)
		{
			this->map_of_tokens_WORD_TOKEN.insert(make_pair(obj, this->map_of_tokens_WORD_TOKEN.size()));
		}
	}

	#pragma omp critical (maps_into_analyzer)
	{
		this->counter_of_tokenizer = this->map_of_tokens_WORD_TOKEN.size();
	}
}

void analyzer::calculate_counter_of_tokenizer_without_rare_words()
{
	this->lemmatize_all_words();

	//критическая секция
	for (auto& obj : *this->list_of_all_lemmatized_text) {	//обращение к критическому ресурсу
		#pragma omp critical (maps_into_analyzer)
		{
			word_and_number_of_appearances_structure _key{ obj, 1 };

			auto iter = this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key);

			if (iter == this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(_key, this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size()));
			else {
				word_and_number_of_appearances_structure _key{ obj, iter->first.number_of_appearances_of_this_word + 1 };
				int tmp_token = iter->second;
				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(_key);
				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(_key, tmp_token));
			}
		}
	}

	/*#pragma omp critical (maps_into_analyzer)
	{
		this->counter_of_tokenizer_without_rare_words = this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size();
	}*/
}

void analyzer::analyze_vec_of_tokens()
{
	this->lemmatize_all_words();

	//критическая секция
	for (auto& obj : *this->list_of_all_lemmatized_text) { //обращение к критическому ресурсу
		#pragma omp critical (maps_into_analyzer)
		{
			this->map_of_tokens_WORD_TOKEN.insert(make_pair(obj, this->map_of_tokens_WORD_TOKEN.size()));
		}
	}

	for (auto& obj : this->map_of_tokens_WORD_TOKEN) {	//обращение к критическому ресурсу
		#pragma omp critical (maps_into_analyzer)
		{
			this->map_of_tokens_TOKEN_WORD.insert(make_pair(obj.second, obj.first));
		}
	}

	for (auto it = this->list_of_all_lemmatized_text->begin(); it != this->list_of_all_lemmatized_text->end(); ++it) {
		for (int i = -COLLOC_DIST; i <= COLLOC_DIST; ++i)
			if (i != 0) {
				auto now_it = this->move_list_iterator(it, i);
				if (now_it == this->list_of_all_lemmatized_text->end())
					continue;
				#pragma omp critical (maps_into_analyzer)
				{
					int first_index = (*this->map_of_tokens_WORD_TOKEN.find(*it)).second;	//обращение к критическому ресурсу
					if (i > 0)
						this->_container_class->increment(first_index, this->map_of_tokens_WORD_TOKEN.find(*now_it)->second, i + (COLLOC_DIST - 1));	//обращение к критическому ресурсу
					else
						this->_container_class->increment(first_index, this->map_of_tokens_WORD_TOKEN.find(*now_it)->second, i + COLLOC_DIST);	//обращение к критическому ресурсу
				}
			}
	}
}

list<string>::iterator analyzer::move_list_iterator(list<string>::iterator _it, int mover)
{
	if (!mover)
		return this->list_of_all_lemmatized_text->end();

	if (mover < 0) {
		for (int i = mover; i != 0; ++i) {
			if (_it == --this->list_of_all_lemmatized_text->begin())
				return this->list_of_all_lemmatized_text->end();
			--_it;
		}
	} 
	else {
		for (int i = mover; i != 0; --i) {
			if (_it == this->list_of_all_lemmatized_text->end())
				return this->list_of_all_lemmatized_text->end();
			++_it;
		}
	}

	return _it;
}

void analyzer::lemmatize_all_words()
{
	this->list_of_all_lemmatized_text = make_shared<list<string>>();

	char utf8[256];

	for (auto& obj : *this->list_of_all_parsed_text) {
		sol_GetLemmaA(this->lemmas_engine, obj.c_str(), utf8, sizeof(utf8));
		this->list_of_all_lemmatized_text->push_back((string)(utf8));
	}

	//utf8[0] = 0;
}

int analyzer::get_k()
{
	return this->k;
}

int analyzer::get_counter_of_tokenizer()
{
	return this->counter_of_tokenizer;
}

int analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff(int cutoff)
{
	int counter_without_rare_words = 0;

	for (auto& obj : this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		if (obj.first.number_of_appearances_of_this_word > cutoff)
			++counter_of_tokenizer_without_rare_words;
		
	return counter_without_rare_words;
}

void analyzer::set_list_of_all_parsed_text(shared_ptr<list<string>> list_of_all_parsed_text)
{
	this->list_of_all_parsed_text = list_of_all_parsed_text;
}

void analyzer::set_k(short _k)
{
	this->k = k;
}

shared_ptr<container_class_interface> analyzer::get_container_class()
{
	return this->_container_class;
}

void analyzer::set_container_class(shared_ptr<container_class_interface> _container_class)
{
	this->_container_class = _container_class;
}