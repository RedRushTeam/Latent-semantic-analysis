#include "analyzer.h"

void analyzer::create_lemmatizer()
{
	#pragma omp critical (lemmatizer_download)
	{
		const char* dict_path = NULL;
		const char* language = "russian";

		if (dict_path == NULL)
			dict_path = LEMADR;

		int flags = FLAGS;

		cout << endl << "Loading the lemmatizator from: " << dict_path;

		analyzer::lemmas_engine = sol_LoadLemmatizatorA(dict_path, flags);

		if (analyzer::lemmas_engine == NULL)
		{
			cout << "Could not load the lemmatizator from: " << dict_path << " With flag: " << flags;
			system("pause");
			exit(1);
		}
	}
}

void analyzer::calculate_counter_of_tokenizer()
{
	this->lemmatize_all_words();

		//критическая секция
	for (auto& obj : *this->list_of_all_lemmatized_text) {	//обращение к критическому ресурсу
		#pragma omp critical (maps_into_analyzer)
		{
			analyzer::map_of_tokens_WORD_TOKEN.insert(make_pair(obj, analyzer::map_of_tokens_WORD_TOKEN.size()));
		}
	}

	#pragma omp critical (maps_into_analyzer)
	{
		analyzer::counter_of_tokenizer = analyzer::map_of_tokens_WORD_TOKEN.size();
	}
}

void analyzer::calculate_counter_of_tokenizer_without_rare_words()
{
	this->lemmatize_all_words();

	unordered_set<int> set_of_words_in_this_text;

	this->lemmatize_all_words();

	for (auto& obj : *this->list_of_all_lemmatized_text) {
		#pragma omp critical (maps_into_analyzer)
		{
			word_and_number_of_appearances_structure _key{ obj, 1, 1 };

			auto iter = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key);

			if (iter == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end()) {
				set_of_words_in_this_text.insert(analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size());
				analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(_key, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size()));
			}
			else {
				int tmp_token = iter->second;
				word_and_number_of_appearances_structure __key;
				if (set_of_words_in_this_text.find(tmp_token) == set_of_words_in_this_text.end()) {
					__key = { obj, iter->first.number_of_appearances_of_this_word + 1, iter->first.number_of_texts_in_which_term_occurs + 1 };
					set_of_words_in_this_text.insert(tmp_token);
				}
				else
					__key = { obj, iter->first.number_of_appearances_of_this_word + 1, iter->first.number_of_texts_in_which_term_occurs };

				analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(__key);
				analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(__key, tmp_token));
			}
		}
	}

	#pragma omp critical (maps_into_analyzer)
	{
		this->counter_of_tokenizer_without_rare_words = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size();
	}
}

void analyzer::initialize_matrix_for_SVD()
{
	analyzer::all_matrix_for_SVD = make_shared<MatrixXf>(analyzer::counter_of_tokenizer_without_rare_words_and_text, analyzer::number_of_texts);
	analyzer::all_matrix_for_SVD->fill(0);

	int new_index = 0;
	for (auto it = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.begin(); it != analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end(); ++it) {
		it.value() = new_index;

		++new_index;
	}

	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.insert(make_pair(obj.second, obj.first));
}

void analyzer::calculate_counter_of_tokenizer_SVD_words()
{
	this->lemmatize_all_words();

	shared_ptr<list<int>> list_of_all_tokens_of_text = make_shared<list<int>>();

	for (auto& obj : *this->list_of_all_lemmatized_text) {
		word_and_number_of_appearances_structure _key{ obj, 1, 1 };
		#pragma omp critical (maps_into_analyzer)
		{
			auto iter = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key);
			if (iter != analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
				list_of_all_tokens_of_text->push_back(iter->second);
		}
	}

	#pragma omp critical (couter_of_texts)
	{
		static int couter_of_texts = 0;
		for (auto& obj : *list_of_all_tokens_of_text)
			if(analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj) != analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.end())
				(*analyzer::all_matrix_for_SVD)(obj, couter_of_texts) = (*analyzer::all_matrix_for_SVD)(obj, couter_of_texts) + 1;

		++couter_of_texts;
	}
}

int analyzer::get_counter_of_tokenizer_without_rare_words_SVD()
{
	shared_ptr<BDCSVD<MatrixXf>> BDCSVD_svd = make_shared<BDCSVD<MatrixXf>>(*(analyzer::all_matrix_for_SVD), ComputeThinV | ComputeThinU);
	auto singular_values_like_vectorXf = BDCSVD_svd->singularValues();
	auto V_matrix_of_SVD = BDCSVD_svd->matrixV();
	auto U_matrix_of_SVD = BDCSVD_svd->matrixU();

	shared_ptr<MatrixXf> svalues_as_MatrixXf = make_shared<MatrixXf>();
	svalues_as_MatrixXf->resize(singular_values_like_vectorXf.size(), singular_values_like_vectorXf.size());
	svalues_as_MatrixXf->fill(0.);

	//распараллелрить тут?
	for (int i = 0; i < singular_values_like_vectorXf.size(); ++i)
		(*svalues_as_MatrixXf)(i, i) = singular_values_like_vectorXf[i];

	svalues_as_MatrixXf->conservativeResize(global_var::COLLOC_DIST + 1, global_var::COLLOC_DIST + 1);

	shared_ptr<MatrixXf> resized_V_matrix_of_SVD = make_shared<MatrixXf>();
	resized_V_matrix_of_SVD->resize(global_var::COLLOC_DIST, V_matrix_of_SVD.cols());

	shared_ptr<MatrixXf> resized_U_matrix_of_SVD = make_shared<MatrixXf>();
	resized_U_matrix_of_SVD->resize(U_matrix_of_SVD.rows(), global_var::COLLOC_DIST);

	//распараллелрить тут?
	for (int i = 0; i <= global_var::COLLOC_DIST; ++i)
		for (int j = 0; j < V_matrix_of_SVD.cols(); ++j)
			resized_V_matrix_of_SVD->operator()(i, j) = V_matrix_of_SVD(i, j);

	for (int i = 0; i < U_matrix_of_SVD.rows(); ++i)
		for (int j = 0; j <= global_var::COLLOC_DIST; ++j)
			resized_U_matrix_of_SVD->operator()(i, j) = U_matrix_of_SVD(i, j);

	//auto restored_matrix = ((*resized_U_matrix_of_SVD) * (*svalues_as_MatrixXf)) * (*resized_V_matrix_of_SVD);

	vector<float> lenghts_words_vector;
	lenghts_words_vector.resize(U_matrix_of_SVD.rows(), NULL);

	//распараллелрить тут?
	for (auto i = 0; i < U_matrix_of_SVD.rows(); ++i) {
		for (auto j = 0; j < U_matrix_of_SVD.cols(); ++j) {
			lenghts_words_vector[i] += U_matrix_of_SVD(i, j) * U_matrix_of_SVD(i, j);
		}
		lenghts_words_vector[i] = sqrt(lenghts_words_vector[i]);
	}

	vector<float> lenghts_texts_vector;
	lenghts_texts_vector.resize(V_matrix_of_SVD.rows(), NULL); //Mabe need a cols, not rows


	for (auto i = 0; i < V_matrix_of_SVD.rows(); ++i) {
		for (auto j = 0; j < V_matrix_of_SVD.cols(); ++j) {
			lenghts_texts_vector[i] += V_matrix_of_SVD(i, j) * V_matrix_of_SVD(i, j);
		}
		lenghts_texts_vector[i] = sqrt(lenghts_texts_vector[i]);
	}

	unordered_map<pair<int, int>, float> scalar_proizv; //терм, документ, скалярное произведение

	//Я считаю что этот цикл можно и нужно оптимизировать. Возможно, есть возможность уйти от медленного unordered_map
	for (auto k = 0; k < V_matrix_of_SVD.rows(); ++k)
		for (auto i = 0; i < U_matrix_of_SVD.rows(); ++i)
			for (auto j = 0; j < U_matrix_of_SVD.cols(); ++j) {
				auto iter = scalar_proizv.find(make_pair(i, k));
				if(iter == scalar_proizv.end())
					scalar_proizv.insert(make_pair(make_pair(i, k), U_matrix_of_SVD(i, j) * V_matrix_of_SVD(k, j)));
				else 
					scalar_proizv[make_pair(i, k)] = iter->second + U_matrix_of_SVD(i, j) * V_matrix_of_SVD(k, j);
			}

	map<pair<int, int>, float> cosinuses; //терм, документ, скалярное произведение

	for (int i = 0; i < lenghts_words_vector.size(); ++i)
		for (int j = 0; j < lenghts_texts_vector.size(); ++j) {
			auto iter = cosinuses.find(make_pair(i, j));
			if (iter == cosinuses.end())
				cosinuses.insert(make_pair(make_pair(i, j), (lenghts_words_vector[i] / lenghts_texts_vector[j])));
			else
				cosinuses[make_pair(i, j)] = iter->second / lenghts_words_vector[i] / lenghts_texts_vector[j];
		}

	list<pair<int, int>> list_of_terms_will_be_deleted;

	for (auto& obj : cosinuses)
		if (obj.second < DELETE_THRESHOLD && (cosinuses.find(obj.first) != cosinuses.end()))
				list_of_terms_will_be_deleted.push_back(obj.first);

	for (auto& obj : list_of_terms_will_be_deleted)
		cosinuses.erase(obj);

	string prev_word = "";

	for (auto& obj : cosinuses)
		if (analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj.first.first) != analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.end() && !(prev_word == (analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj.first.first))->second.word)) {
			prev_word = (analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj.first.first))->second.word;
			analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.insert(make_pair(analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj.first.first)->second, obj.first.first));
		}

	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.clear();
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.begin(), analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.end());
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.clear();

	int tokens_new = 0;
	string zero_token;
	for (auto it = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.begin(); it != analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end(); ++it) {
		if (!tokens_new)
			zero_token = it.key().word;

		it.value() = tokens_new;
		++tokens_new;
	}

	//стоп слово А всегда имеет нулевой номер
	word_and_number_of_appearances_structure _key = { zero_token, 1, 1 };
	word_and_number_of_appearances_structure __key = { (string)"А", 1, 1 };
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key).value() = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value();
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value() = 0;

	analyzer::counter_of_tokenizer_without_rare_words_SVD = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size();

	return analyzer::counter_of_tokenizer_without_rare_words_SVD;
}

void analyzer::calculate_mat_ozidanie()
{
	this->lemmatize_all_words();

	for (auto it = this->list_of_all_lemmatized_text->begin(); it != this->list_of_all_lemmatized_text->end(); ++it) {
		#pragma omp critical (maps_into_analyzer)
		{
			for (int i = -global_var::COLLOC_DIST - 1; i <= global_var::COLLOC_DIST + 1; ++i)
				if (i != 0) {
					auto now_it = this->move_list_iterator(it, i);
					if (now_it == this->list_of_all_lemmatized_text->end())
						continue;

					word_and_number_of_appearances_structure _key = { *it, 1, 1 };
					word_and_number_of_appearances_structure __key = { *now_it, 1, 1 };

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					int first_index = (*this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key)).second;	//обращение к критическому ресурсу		//быть может, тут не нужна потокобезопасность?

					if (first_index < dynamic_pointer_cast<piecewise_container_class>(this->_mat_ozidanie)->get_downloaded_range().first ||
						(first_index > dynamic_pointer_cast<piecewise_container_class>(this->_mat_ozidanie)->get_downloaded_range().second))
						continue;

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					if (i > 0)
						this->_mat_ozidanie->summ_for_concret_colloc(first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, i - 1, (now_type)1.);	//обращение к критическому ресурсу
					else
						this->_mat_ozidanie->summ_for_concret_colloc(first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, abs(i) - 1, (now_type)1.);	//обращение к критическому ресурсу				
				}
		}
	}
}

shared_ptr<container_class_interface> analyzer::calculate_mat_disperse()
{
	this->lemmatize_all_words();

	auto now_text_container = make_shared<piecewise_container_class>(global_var::COLLOC_DIST, this->_mat_ozidanie->get_count_of_collocations(), dynamic_pointer_cast<piecewise_container_class>(this->_mat_ozidanie)->get_downloaded_range());

	for (auto it = this->list_of_all_lemmatized_text->begin(); it != this->list_of_all_lemmatized_text->end(); ++it) {
		#pragma omp critical (maps_into_analyzer)
		{
			for (int i = -global_var::COLLOC_DIST - 1; i <= global_var::COLLOC_DIST + 1; ++i)
				if (i != 0) {
					auto now_it = this->move_list_iterator(it, i);
					if (now_it == this->list_of_all_lemmatized_text->end())
						continue;

					word_and_number_of_appearances_structure _key = { *it, 1, 1 };
					word_and_number_of_appearances_structure __key = { *now_it, 1, 1 };

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					int first_index = (*this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key)).second;	//обращение к критическому ресурсу		//быть может, тут не нужна потокобезопасность?

					if (first_index < dynamic_pointer_cast<piecewise_container_class>(now_text_container)->get_downloaded_range().first ||
						(first_index > dynamic_pointer_cast<piecewise_container_class>(now_text_container)->get_downloaded_range().second))
						continue;

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					if (i > 0)
						now_text_container->summ_for_concret_colloc(first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, i - 1, (now_type)1.);	//обращение к критическому ресурсу
					else
						now_text_container->summ_for_concret_colloc(first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, abs(i) - 1, (now_type)1.);	//обращение к критическому ресурсу				
				}
		}
	}

	//#pragma omp critical (ozidanie)
	//{
	//	*now_text_container -= analyzer::_mat_ozidanie;
	//}

	return now_text_container;
}

shared_ptr<MatrixXf> analyzer::calculate_SVD_matrix_for_concret_text()
{
	auto matrix_for_all_SVD = make_shared<MatrixXf>(analyzer::helper_map_for_SVD_rows_colloc_numbers->size(), 1);

	matrix_for_all_SVD->fill(NULL);

	this->lemmatize_all_words();

	tsl::robin_map<three_coordinate_structure, int> map_of_tokens_TOKEN_DATA;

	for (auto it = this->list_of_all_lemmatized_text->begin(); it != this->list_of_all_lemmatized_text->end(); ++it) {
		#pragma omp critical (maps_into_analyzer)
		{
			for (int i = -global_var::COLLOC_DIST - 1; i <= global_var::COLLOC_DIST + 1; ++i)
				if (i != 0) {
					auto now_it = this->move_list_iterator(it, i);
					if (now_it == this->list_of_all_lemmatized_text->end())
						continue;

					word_and_number_of_appearances_structure _key = { *it, 1, 1 };
					word_and_number_of_appearances_structure __key = { *now_it, 1, 1 };

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					int first_index = (*this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key)).second;	//обращение к критическому ресурсу		//быть может, тут не нужна потокобезопасность?

					if (first_index < dynamic_pointer_cast<piecewise_container_class>(this->_mat_ozidanie)->get_downloaded_range().first ||
						(first_index > dynamic_pointer_cast<piecewise_container_class>(this->_mat_ozidanie)->get_downloaded_range().second))
						continue;

					if (analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key) == analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
						continue;

					

					if (i > 0) {
						if (!analyzer::inverse_helper_map_for_SVD_rows_colloc_numbers->contains(three_coordinate_structure{first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, (short)(i - 1)}))
							continue;	//проверка на то, что пара с подозрением на флуктуацию
						three_coordinate_structure ___key = { first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, i - 1 };
						auto iter_ = map_of_tokens_TOKEN_DATA.find(___key);
						if (iter_ == map_of_tokens_TOKEN_DATA.end())
							map_of_tokens_TOKEN_DATA[___key] = (float)1.;
						else
							iter_.value() = iter_.value() + 1;
					}
					else {
						if (!analyzer::inverse_helper_map_for_SVD_rows_colloc_numbers->contains(three_coordinate_structure{ first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, (short)(abs(i) - 1) }))
							continue;	//проверка на то, что пара с подозрением на флуктуацию
						three_coordinate_structure ___key = { first_index, analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key)->second, abs(i) - 1 };
						auto iter_ = map_of_tokens_TOKEN_DATA.find(___key);
						if (iter_ == map_of_tokens_TOKEN_DATA.end())
							map_of_tokens_TOKEN_DATA[___key] = (float)1.;
						else
							iter_.value() = iter_.value() + 1;
					}
				}
		}
	}

	auto size = inverse_helper_map_for_SVD_rows_colloc_numbers->size();
	
	for (auto obj : map_of_tokens_TOKEN_DATA) {
		auto iter = inverse_helper_map_for_SVD_rows_colloc_numbers->find(obj.first);

		if (iter == inverse_helper_map_for_SVD_rows_colloc_numbers->end())
			continue;

		auto shit = iter.value();

		(*matrix_for_all_SVD)(iter.value(), 0) += (now_type)obj.second;
	}

	map_of_tokens_TOKEN_DATA.clear();

	return matrix_for_all_SVD;
}

list<string>::iterator analyzer::move_list_iterator(list<string>::iterator _it, int mover)
{
	if (!mover)
		return this->list_of_all_lemmatized_text->end();

	if (mover < 0) {
		for (int i = mover; i != 0; ++i) {
			if (_it == this->list_of_all_lemmatized_text->begin())
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

string analyzer::get_word_for_token(int token)
{
	return analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(token).value().word;
}

void analyzer::calculate_matrix_only_for_terms(int number_of_text)
{
	this->lemmatize_all_words();

	for (auto it = this->list_of_all_lemmatized_text->begin(); it != this->list_of_all_lemmatized_text->end(); ++it) {
		#pragma omp critical (maps_into_analyzer)
		{
			word_and_number_of_appearances_structure _key = { *it, 1, 1 };

			analyzer::only_terms_mass[(*this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key)).second * analyzer::number_of_texts + number_of_text] += (now_type)1.;	//обращение к критическому ресурсу
		}
	}
}

void analyzer::lemmatize_all_words()
{
	this->list_of_all_lemmatized_text = make_shared<list<string>>();

	char utf8[128];

	for (auto& obj : *this->list_of_all_parsed_text) {
		sol_GetLemmaA(analyzer::lemmas_engine, obj.c_str(), utf8, sizeof(utf8));
		this->list_of_all_lemmatized_text->push_back((string)(utf8));
	}
}

int analyzer::get_k()
{
	return analyzer::k;
}

int analyzer::get_number_of_texts()
{
	return analyzer::number_of_texts;
}

int analyzer::get_counter_of_tokenizer()
{
	return analyzer::counter_of_tokenizer;
}

int analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff(int cutoff)
{
	int counter_without_rare_words = 0;

	list<word_and_number_of_appearances_structure> list_of_indexes_for_delete;

	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		if (obj.first.number_of_appearances_of_this_word > cutoff)
			++counter_without_rare_words;
		else
			list_of_indexes_for_delete.push_back(obj.first);

	for (auto obj : list_of_indexes_for_delete) 
		analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(obj);
		
	int tokens_new = 0;
	string zero_token;
	for (auto it = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.begin(); it != analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end(); ++it) {
		if (!tokens_new)
			zero_token = it.key().word;

		it.value() = tokens_new;
		++tokens_new;
	}

	//стоп слово А всегда имеет нулевой номер
	word_and_number_of_appearances_structure _key = { zero_token, 1, 1 };
	word_and_number_of_appearances_structure __key = { (string)"А", 1, 1 };
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key).value() = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value();
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value() = 0;

	analyzer::counter_of_tokenizer_without_rare_words = counter_without_rare_words;
	return counter_without_rare_words;
}

int analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(int cutoff, int cutoff_of_texts)
{
	int counter_without_rare_words = 0;

	list<word_and_number_of_appearances_structure> list_of_indexes_for_delete;

	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		if (obj.first.number_of_appearances_of_this_word > cutoff && (obj.first.number_of_texts_in_which_term_occurs > cutoff_of_texts))
			++counter_without_rare_words;
		else
			list_of_indexes_for_delete.push_back(obj.first);
		
	for (auto obj : list_of_indexes_for_delete) 
		analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(obj);

	int tokens_new = 0;
	string zero_token;
	for (auto it = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.begin(); it != analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end(); ++it) {
		if (!tokens_new)
			zero_token = it.key().word;

		it.value() = tokens_new;
		++tokens_new;
	}

	//стоп слово А всегда имеет нулевой номер
	word_and_number_of_appearances_structure _key = { zero_token, 1, 1 };
	word_and_number_of_appearances_structure __key = { (string)"А", 1, 1 };
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key).value() = analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value();
	analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(__key).value() = 0;
	
	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.insert(make_pair(obj.second, obj.first));

	analyzer::counter_of_tokenizer_without_rare_words_and_text = counter_without_rare_words;
	return counter_without_rare_words;
}

void analyzer::set_list_of_all_parsed_text(shared_ptr<list<string>> list_of_all_parsed_text)
{
	this->list_of_all_parsed_text = list_of_all_parsed_text;
}

void analyzer::set_k(short _k)
{
	analyzer::k = k;
}

void analyzer::set_number_of_texts(int number_of_texts)
{
	analyzer::number_of_texts = number_of_texts;
}

void analyzer::set_helper_map_for_SVD_rows_colloc_numbers(shared_ptr<tsl::robin_map<size_t, three_coordinate_structure>> helper_map_for_SVD_rows_colloc_numbers)
{
	analyzer::inverse_helper_map_for_SVD_rows_colloc_numbers = make_shared<tsl::robin_map<three_coordinate_structure, size_t>>();

	for (auto obj : *helper_map_for_SVD_rows_colloc_numbers)
		analyzer::inverse_helper_map_for_SVD_rows_colloc_numbers->insert(make_pair(obj.second, obj.first));

	analyzer::helper_map_for_SVD_rows_colloc_numbers = helper_map_for_SVD_rows_colloc_numbers;
}

shared_ptr<container_class_interface> analyzer::get_container_mat_ozidanie()
{
	return analyzer::_mat_ozidanie;
}

void analyzer::set_container_mat_ozidanie(shared_ptr<container_class_interface> _mat_ozidanie)
{
	analyzer::_mat_ozidanie = _mat_ozidanie;
}

shared_ptr<container_class_interface> analyzer::get_container_mat_disperse()
{
	return analyzer::_mat_disperse;
}

void analyzer::set_container_mat_disperse(shared_ptr<container_class_interface> _mat_disperse)
{
	analyzer::_mat_disperse = _mat_disperse;
}

float* analyzer::get_only_terms_mass()
{
	return analyzer::only_terms_mass;
}

void analyzer::set_only_terms_mass(float* only_terms_mass)
{
	analyzer::only_terms_mass = only_terms_mass;
}
