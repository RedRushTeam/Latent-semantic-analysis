#pragma once 
#include "piecewise_container_class.h"

void piecewise_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)]++;
}

void piecewise_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)]--;
}

now_type piecewise_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return -1.;

	return this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)];
}

shared_ptr<container_class_interface> piecewise_container_class::pow_all(int stepen)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::sqrt_all()
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator+(shared_ptr<container_class_interface> summed_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator+(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator-(shared_ptr<container_class_interface> summed_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator-(now_type _num)
{
	return shared_ptr<container_class_interface>();
}
void piecewise_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{

}

void piecewise_container_class::operator+=(now_type _num)
{

}

void piecewise_container_class::operator-=(shared_ptr<container_class_interface> summed_class)
{

}

void piecewise_container_class::operator-=(now_type _koef)
{

}

void piecewise_container_class::operator/=(now_type _num)
{

}

bool piecewise_container_class::operator==(shared_ptr<container_class_interface> compared_class)
{
	return false;
}

bool piecewise_container_class::operator!=(shared_ptr<container_class_interface> compared_class)
{
	return false;
}

shared_ptr<container_class_interface> piecewise_container_class::operator*(shared_ptr<container_class_interface> multipliable_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator*(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator/(shared_ptr<container_class_interface> dividor_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator/(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

void piecewise_container_class::bailout(int rc, MDBX_env* env, MDBX_dbi dbi, MDBX_txn* txn, MDBX_cursor* cursor)
{
		fprintf(stderr, "mdbx_env_open: (%d) %s\n", rc, mdbx_strerror(rc));
		if (cursor)
			mdbx_cursor_close(cursor);
		if (txn)
			mdbx_txn_abort(txn);
		if (!dbi)
			mdbx_dbi_close(env, dbi);
		if (env)
			mdbx_env_close(env);}

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector.clear();
}

void piecewise_container_class::upload_vec()
{
	//method for Dima
	string textname = static_cast<string>(DB_PATH) + "text" + to_string(this->downloaded_text) + "_terms" + to_string(this->get_downloaded_range().first) + "-" + to_string(this->get_downloaded_range().second);
	if (fs::exists(textname))
		fs::remove(textname);

	int rc;

	MDBX_env* env = NULL;
	MDBX_dbi dbi = 0;
	MDBX_val index, number;
	MDBX_txn* txn = NULL;
	MDBX_cursor* cursor = NULL;

	auto kolichestvo_zapisey = (this->get_downloaded_range().second - this->get_downloaded_range().first) * this->get_count_of_collocations() * this->get_k();
	string _for_size = "99999!99999!4";
	auto start_size = (sizeof(_for_size) + sizeof(now_type)) * kolichestvo_zapisey; //(äëèíà êëþ÷à + äëèíà ÷èñëà) * íà êîëè÷åñòâî çàïèñåé

	/*ÑÎÇÄÀÍÈÅ ÔÀÉËÀ ÁÄ*/
	rc = mdbx_env_create(&env);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	/*ÂÛÄÅËÅÍÈÅ ÏÀÌßÒÈ*/
	//mdbx_env_set_mapsize(env, count_of_collocations * count_of_collocations * k); ///deprecated
	rc = mdbx_env_set_geometry(env, start_size, -1, start_size * 4, -1, -1, -1); //ìåñòî äëÿ ïîèñêà ñêîðîñòè
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	rc = mdbx_env_open(env, textname.c_str(),
		MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM, 0664); //0664 - what is it ?
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);


	rc = mdbx_txn_begin(env, NULL, MDBX_TXN_READWRITE, &txn);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	int vec_idx = 0;

	for (int i = this->get_downloaded_range().first; i <= this->get_downloaded_range().second; ++i)
		for (int j = 0; j < this->get_count_of_collocations(); ++j)
			for (int l = 0; l < this->get_k(); ++l) {
				if (!(vec_idx % 10000))
					rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
				if (rc)
					this->bailout(rc, env, dbi, txn, cursor);
				auto _index = to_string(i) + "!" + to_string(j) + "!" + to_string(l);
				now_type _value = this->downloaded_vector[vec_idx];
				vec_idx++;
				index.iov_len = sizeof(_index);
				index.iov_base = &_index;
				number.iov_len = sizeof(float);
				number.iov_base = &_value;

				rc = mdbx_put(txn, dbi, &index, &number, MDBX_UPSERT);
				if (rc)
					this->bailout(rc, env, dbi, txn, cursor);
				if (!(vec_idx % 10000)) {
					rc = mdbx_txn_commit(txn);
					txn = NULL;
				}
				if (rc)
					this->bailout(rc, env, dbi, txn, cursor);
				
			}

	if (txn != NULL)
		rc = mdbx_txn_commit(txn);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	_filenames[textname] = this->downloaded_range;

	this->downloaded_range = make_pair(-1, -1);
	this->downloaded_vector.clear();

}

void piecewise_container_class::download_vec(pair<int, int> frames) 
{
	this->downloaded_range = frames;

	string textname = static_cast<string>(DB_PATH) + "text" + to_string(this->downloaded_text) + "_terms" + to_string(this->get_downloaded_range().first) + "-" + to_string(this->get_downloaded_range().second);
	
	if (!fs::exists(textname))
		return;

	//here must be a checking of possibility to write slice into RAM

	//////////////////////////// ÒÐÀÍÇÀÊÖÈß ÏÎËÓ×ÅÍÈß ÄÀÍÍÛÕ ÈÇ ÁÀÇÛ

	int rc;
	float _value = 0.0;

	MDBX_env* env = NULL;
	MDBX_dbi dbi = 0;
	MDBX_val index, number;
	MDBX_txn* txn = NULL;
	MDBX_cursor* cursor = NULL;

	rc = mdbx_env_create(&env);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	rc = mdbx_env_open(env, textname.c_str(),
		MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM, 7777); //0664 - what is it ?
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	rc = mdbx_txn_begin(env, NULL, MDBX_TXN_RDONLY, &txn);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	//rc = mdbx_cursor_open(txn, dbi, &cursor);
	/*rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);*/

	rc = mdbx_cursor_open(txn, dbi, &cursor);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);

	/*rc = mdbx_cursor_get(cursor, &index, &number, MDBX_NEXT);
	if (rc)
		this->bailout(rc, env, dbi, txn, cursor);*/

	int found = 0;
	int vec_idx = 0;

	/*auto _index = 2 * this->get_count_of_collocations() * (this->get_k() + 1) + 3 * (this->get_k() + 1) + 1; //231
	index.iov_base = &_index;
	index.iov_len = sizeof(int);
	number.iov_base = nullptr;
	number.iov_len = sizeof(float);*/

	//rc = mdbx_get(txn, dbi, &index, &number);
	char sval[32];

	for (int i = this->get_downloaded_range().first; i < this->get_downloaded_range().second; ++i)
		for (int j = 0; j < this->get_count_of_collocations(); ++j)
			for (int l = 0; l <= this->get_k(); ++l) {
				auto _index = i * this->get_count_of_collocations() * (this->get_k()+1) + j * (this->get_k() + 1) + l;
				index.iov_base = &index;
				index.iov_len = sizeof(int);
				//number.iov_base = &_value;
				//number.iov_len = sizeof(_value);
				//number.iov_len = sizeof(float);

				
				while ((rc = mdbx_cursor_get(cursor, &index, &number, MDBX_NEXT)==0)) {
					
					cout << " value: " << *(now_type*)number.iov_base;
					this->downloaded_vector[vec_idx] = *(now_type*)number.iov_base;
					vec_idx++;
					found += 1;
				}
				if (rc != MDBX_NOTFOUND || found == 0)
					;
				else 
					rc = MDBX_SUCCESS;
				
			}
	this->bailout(rc, env, dbi, txn, cursor);
}

bool piecewise_container_class::is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension)
{
	if(this->downloaded_range.first < first_dimension && (this->downloaded_range.second > first_dimension))
		return true;

	return false;
}

int piecewise_container_class::collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const
{
	return first_dimension * this->get_count_of_collocations() * COLLOC_DIST + second_dimension * COLLOC_DIST + third_dimension;
}

void piecewise_container_class::set_downloaded_range(pair<int, int> downloaded_range)
{
	this->downloaded_range = downloaded_range;
}

pair<int, int> piecewise_container_class::get_downloaded_range() const
{
	return this->downloaded_range;
}

void piecewise_container_class::summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] += _num;
}

void piecewise_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = perem;
}
