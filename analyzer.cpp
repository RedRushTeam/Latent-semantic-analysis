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

		//����������� ������
	for (auto& obj : *this->list_of_all_lemmatized_text) {	//��������� � ������������ �������
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

	unordered_set<int> set_of_words_in_this_text;

	//����������� ������
	for (auto& obj : *this->list_of_all_lemmatized_text) {	//��������� � ������������ �������
		#pragma omp critical (maps_into_analyzer)
		{
			word_and_number_of_appearances_structure _key{ obj, 1, 1 };

			auto iter = this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key);

			if (iter == this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end()) {
				set_of_words_in_this_text.insert(this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size());
				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(_key, this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size()));
				
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

				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(__key);
				this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.insert(make_pair(__key, tmp_token));
			}
		}
	}

	#pragma omp critical (maps_into_analyzer)
	{
		this->counter_of_tokenizer_without_rare_words = this->map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.size();
	}
}

void analyzer::initialize_matrix_for_SVD()
{
	analyzer::all_matrix_for_SVD = make_shared<MatrixXf>(analyzer::counter_of_tokenizer_without_rare_words_and_text, analyzer::number_of_texts);
	analyzer::all_matrix_for_SVD->fill(0);

	int new_index = 0;
	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_) {
		obj.second = new_index;

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
			auto iter = map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.find(_key);
			if (iter != map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.end())
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

	//��������������� ���?
	for (int i = 0; i < singular_values_like_vectorXf.size(); ++i)
		svalues_as_MatrixXf->operator()(i, i) = singular_values_like_vectorXf[i];

	svalues_as_MatrixXf->conservativeResize(COLLOC_DIST, COLLOC_DIST);

	shared_ptr<MatrixXf> resized_V_matrix_of_SVD = make_shared<MatrixXf>();
	resized_V_matrix_of_SVD->resize(COLLOC_DIST, V_matrix_of_SVD.cols());

	shared_ptr<MatrixXf> resized_U_matrix_of_SVD = make_shared<MatrixXf>();
	resized_U_matrix_of_SVD->resize(U_matrix_of_SVD.rows(), COLLOC_DIST);

	//��������������� ���?
	for (int i = 0; i < COLLOC_DIST; ++i)
		for (int j = 0; j < V_matrix_of_SVD.cols(); ++j)
			resized_V_matrix_of_SVD->operator()(i, j) = V_matrix_of_SVD(i, j);

	for (int i = 0; i < U_matrix_of_SVD.rows(); ++i)
		for (int j = 0; j < COLLOC_DIST; ++j)
			resized_U_matrix_of_SVD->operator()(i, j) = U_matrix_of_SVD(i, j);

	auto restored_matrix = ((*resized_U_matrix_of_SVD) * (*svalues_as_MatrixXf)) * (*resized_V_matrix_of_SVD);

	vector<float> lenghts_words_vector;
	lenghts_words_vector.resize(U_matrix_of_SVD.rows(), NULL);

	//��������������� ���?
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

	unordered_map<pair<int, int>, float> scalar_proizv; //����, ��������, ��������� ������������

	//� ������ ��� ���� ���� ����� � ����� ��������������. ��������, ���� ����������� ���� �� ���������� unordered_map
	for (auto k = 0; k < V_matrix_of_SVD.rows(); ++k)
		for (auto i = 0; i < U_matrix_of_SVD.rows(); ++i)
			for (auto j = 0; j < U_matrix_of_SVD.cols(); ++j) {
				auto iter = scalar_proizv.find(make_pair(i, k));
				if(iter == scalar_proizv.end())
					scalar_proizv.insert(make_pair(make_pair(i, k), U_matrix_of_SVD(i, j) * V_matrix_of_SVD(k, j)));
				else 
					scalar_proizv[make_pair(i, k)] = iter->second + U_matrix_of_SVD(i, j) * V_matrix_of_SVD(k, j);
			}

	map<pair<int, int>, float> cosinuses; //����, ��������, ��������� ������������

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
			map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.insert(make_pair(analyzer::map_of_tokens_TOKEN_Word_and_number_of_appearances_struct_.find(obj.first.first)->second, obj.first.first));
		}

	analyzer::counter_of_tokenizer_without_rare_words_SVD = map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_SVD.size();

	return analyzer::counter_of_tokenizer_without_rare_words_SVD;
}

void analyzer::analyze_vec_of_tokens()
{
	this->lemmatize_all_words();

	//����������� ������
	for (auto& obj : *this->list_of_all_lemmatized_text) { //��������� � ������������ �������
		#pragma omp critical (maps_into_analyzer)
		{
			this->map_of_tokens_WORD_TOKEN.insert(make_pair(obj, this->map_of_tokens_WORD_TOKEN.size()));
		}
	}

	for (auto& obj : this->map_of_tokens_WORD_TOKEN) {	//��������� � ������������ �������
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
					int first_index = (*this->map_of_tokens_WORD_TOKEN.find(*it)).second;	//��������� � ������������ �������
					if (i > 0)
						this->_container_class->increment(first_index, this->map_of_tokens_WORD_TOKEN.find(*now_it)->second, i + (COLLOC_DIST - 1));	//��������� � ������������ �������
					else
						this->_container_class->increment(first_index, this->map_of_tokens_WORD_TOKEN.find(*now_it)->second, i + COLLOC_DIST);	//��������� � ������������ �������
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

	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		if (obj.first.number_of_appearances_of_this_word > cutoff) 
			++counter_without_rare_words;
		else
			map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(obj.first);
		
	counter_of_tokenizer_without_rare_words = counter_without_rare_words;
	return counter_without_rare_words;
}

int analyzer::get_counter_of_tokenizer_without_rare_words_with_cutoff_of_text(int cutoff, int cutoff_of_texts)
{
	int counter_without_rare_words = 0;

	
	for (auto& obj : analyzer::map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_)
		if (obj.first.number_of_appearances_of_this_word > cutoff && (obj.first.number_of_texts_in_which_term_occurs > cutoff_of_texts))
			++counter_without_rare_words;
		else
			map_of_tokens_Word_and_number_of_appearances_struct_TOKEN_.erase(obj.first);
		//������ ���
	counter_of_tokenizer_without_rare_words_and_text = counter_without_rare_words;
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

shared_ptr<container_class_interface> analyzer::get_container_class()
{
	return this->_container_class;
}

void analyzer::set_container_class(shared_ptr<container_class_interface> _container_class)
{
	this->_container_class = _container_class;
}