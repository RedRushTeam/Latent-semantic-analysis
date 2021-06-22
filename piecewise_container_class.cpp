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

void piecewise_container_class::sqrt_all()
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type obj) {	return sqrt(obj); });
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
	for (size_t i = 0; i < this->downloaded_vector.size(); ++i)
		this->downloaded_vector[i] -= dynamic_pointer_cast<piecewise_container_class>(summed_class)->get_count_of_concret_collocation_with_one_coordinate(i);
}

void piecewise_container_class::operator-=(now_type _koef)
{

}

void piecewise_container_class::operator/=(now_type _num)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {	return obj /= _num; });
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

void piecewise_container_class::operator/(shared_ptr<container_class_interface> dividor_class)
{

}

void piecewise_container_class::operator/(now_type _koef)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {return obj /= _koef; });
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
		mdbx_env_close(env);
}

void piecewise_container_class::operator*=(now_type _num)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type obj) {	return obj * _num; });
}

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector.clear();
	this->downloaded_vector.shrink_to_fit();
	make_pair(-1, -1);
}

void piecewise_container_class::upload_vec()	//ошибки с удалением
{
	//method for Dima
	

	auto zapisey_in_file = 10000000;
	size_t kolichestvo_zapisey = ((size_t)this->get_downloaded_range().second - (size_t)this->get_downloaded_range().first) * (size_t)this->get_count_of_collocations() * ((size_t)this->get_k() + 1);
	size_t number_of_full_files = kolichestvo_zapisey / zapisey_in_file;

	size_t start_size = (sizeof(int) + sizeof(now_type)) * zapisey_in_file;
	size_t number_of_terms_in_one_file = zapisey_in_file / this->get_count_of_collocations() / (this->get_k() + 1);
	size_t tails = number_of_full_files * number_of_terms_in_one_file;

#pragma omp parallel num_threads(4)
	{
		#pragma omp for schedule(static)
		for (int t = 0; t <= number_of_full_files; t++)
		{
			int rc;

			MDBX_env* env = NULL;
			MDBX_dbi dbi = 0;
			MDBX_val index, number;
			MDBX_txn* txn = NULL;
			MDBX_cursor* cursor = NULL;

			number.iov_len = sizeof(float);
			index.iov_len = sizeof(size_t);

			int left_term = number_of_terms_in_one_file * t;
			int right_term = left_term + number_of_terms_in_one_file - 1;

			if (t == number_of_full_files)
				if (tails <= this->downloaded_range.second) {
					left_term = tails;
					right_term = this->downloaded_range.second;
				}

			string textname = this->path_to_db.string() + "\\text" + to_string(this->downloaded_text) + "_terms[" + to_string(left_term) + "-" + to_string(right_term) + "]";
			list_of_functions::delete_file_for_path(textname);

			rc = mdbx_env_create(&env);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);

			rc = mdbx_env_set_geometry(env, start_size, -1, start_size * 10, -1, -1, -1);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);

			rc = mdbx_env_open(env, textname.c_str(),
				MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM | MDBX_UTTERLY_NOSYNC, 0664);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);


			rc = mdbx_txn_begin(env, NULL, MDBX_TXN_READWRITE, &txn);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);

			rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);

			size_t vec_idx = t * number_of_terms_in_one_file * this->get_count_of_collocations() * ((size_t)this->get_k() + 1);

			for (int i = left_term; i <= right_term; ++i)
				for (int j = 0; j < this->get_count_of_collocations(); ++j)
					for (int l = 0; l < this->get_k(); ++l) {
						if ((vec_idx) && !(vec_idx % 100000)) {
							rc = mdbx_txn_begin(env, NULL, MDBX_TXN_READWRITE, &txn);
							if (rc)
								this->bailout(rc, env, dbi, txn, cursor);
						}

						size_t _index = collect_one_coordinate_from_three(i, j, l);
						now_type _value = this->downloaded_vector[vec_idx];

						index.iov_base = &_index;
						number.iov_base = &_value;

						rc = mdbx_put(txn, dbi, &index, &number, MDBX_UPSERT);

						if (rc)
							this->bailout(rc, env, dbi, txn, cursor);

						vec_idx++;

						if ((vec_idx) && !(vec_idx % 100000)) {
							rc = mdbx_txn_commit(txn);
							if (rc)
								this->bailout(rc, env, dbi, txn, cursor);
							txn = NULL;
						}

					}

			if (txn != NULL)
				rc = mdbx_txn_commit(txn);
			if (rc)
				this->bailout(rc, env, dbi, txn, cursor);

			this->bailout(rc, env, dbi, txn, cursor);
			list_of_functions::compress_file_for_path(textname);
			list_of_functions::delete_file_for_path(textname);

			_filenames[textname + ".7z"] = make_pair(left_term, right_term);
		}
	}
}

void piecewise_container_class::download_vec()
{	
	string textname = this->path_to_db.string() + "\\text" + to_string(this->downloaded_text) + "_terms[" + to_string(this->get_downloaded_range().first) + "-" + to_string(this->get_downloaded_range().second) + "]";

	if (!fs::exists(textname))
		return;

	list_of_functions::decompress_file_for_path(textname + ".7z");
	list_of_functions::delete_file_for_path(textname + ".7z");

	

	//here must be a checking of possibility to write slice into RAM


		int rc;
		float _value = 0.0;

		MDBX_env* env = NULL;
		MDBX_dbi dbi = 0;
		MDBX_val index, number;
		MDBX_txn* txn = NULL;
		MDBX_cursor* cursor = NULL;

		index.iov_len = sizeof(size_t);
		number.iov_len = sizeof(now_type);

		rc = mdbx_env_create(&env);
		if (rc)
			this->bailout(rc, env, dbi, txn, cursor);

		rc = mdbx_env_open(env, textname.c_str(),
			MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM | MDBX_UTTERLY_NOSYNC, 7777);
		if (rc)
			this->bailout(rc, env, dbi, txn, cursor);

		rc = mdbx_txn_begin(env, NULL, MDBX_TXN_RDONLY, &txn);
		if (rc)
			this->bailout(rc, env, dbi, txn, cursor);

		rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
		if (rc)
			this->bailout(rc, env, dbi, txn, cursor);

		rc = mdbx_cursor_open(txn, dbi, &cursor);
		if (rc)
			this->bailout(rc, env, dbi, txn, cursor);


		size_t vec_idx = 0;

		for (int i = this->get_downloaded_range().first; i < this->get_downloaded_range().second; ++i)
			for (int j = 0; j < this->get_count_of_collocations(); ++j)
				for (int l = 0; l <= this->get_k(); ++l) {

					size_t _index = collect_one_coordinate_from_three(i, j, l);

					index.iov_base = &_index;
					number.iov_base = &_value;

					rc = mdbx_get(txn, dbi, &index, &number);

					if (!rc) {
						this->downloaded_vector[vec_idx] = *(now_type*)number.iov_base;
						vec_idx++;
					}

				}

		this->bailout(rc, env, dbi, txn, cursor);
	
}

bool piecewise_container_class::is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension)
{
	if (this->downloaded_range.first <= first_dimension && (this->downloaded_range.second > first_dimension))
		return true;

	return false;
}

size_t piecewise_container_class::collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const
{
	size_t part = first_dimension / SIZE_OF_PIECE;
	size_t ret = ((size_t)first_dimension * (size_t)this->get_count_of_collocations() * (size_t)(COLLOC_DIST + 1) + second_dimension * (COLLOC_DIST + 1) + third_dimension) - (size_t)(this->downloaded_vector.capacity() * part);
	return ret;
}

void piecewise_container_class::fill_vector(now_type number_for_fill)
{
	fill(this->downloaded_vector.begin(), this->downloaded_vector.end(), number_for_fill);
}

now_type piecewise_container_class::get_count_of_concret_collocation_with_one_coordinate(size_t _i)
{
	return this->downloaded_vector.at(_i);
}

void piecewise_container_class::set_count_of_concret_collocation_with_one_coordinate(size_t _i, now_type _perem)
{
	this->downloaded_vector.at(_i) = _perem;
}

size_t piecewise_container_class::get_vector_size()
{
	return this->downloaded_vector.size();
}

void piecewise_container_class::set_downloaded_range(pair<int, int> downloaded_range)
{
	//this->downloaded_vector.resize((downloaded_range.second - downloaded_range.first + 1) * this->get_count_of_collocations() * (this->get_k() + 1));// check it (is +1 needed?)
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

	size_t index = this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension);
	
	this->downloaded_vector[index] += _num;
}

void piecewise_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = perem;
}