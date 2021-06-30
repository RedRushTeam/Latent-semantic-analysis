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

void piecewise_container_class::pow_all(int stepen)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {	return pow(obj, stepen); });
}

void piecewise_container_class::sqrt_all()
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {	return sqrt(obj); });
}

void piecewise_container_class::operator+=(now_type _num)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {	return obj + _num; });
}

void piecewise_container_class::operator-=(shared_ptr<container_class_interface> summed_class)
{
	if (summed_class->get_count_of_collocations() != this->get_count_of_collocations()) {	//TODO add check downloaded range!
		exit(-228);
		return;
	}

	for (size_t i = 0; i < this->downloaded_vector.size(); ++i)
		this->downloaded_vector[i] = this->downloaded_vector[i] - dynamic_pointer_cast<piecewise_container_class>(summed_class)->get_count_of_concret_collocation_with_one_coordinate(i);	//TODO KOSTIL FOR TEST!!!!
}

void piecewise_container_class::operator-=(now_type _koef)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {	return (obj - _koef); });
}

void piecewise_container_class::operator/=(now_type _num)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type& obj) {return (obj / _num); });
}

void piecewise_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
	for (size_t i = 0; i < this->downloaded_vector.size(); ++i)
		this->downloaded_vector[i] += dynamic_pointer_cast<piecewise_container_class>(summed_class)->get_count_of_concret_collocation_with_one_coordinate(i);
}

void piecewise_container_class::operator*=(now_type _num)
{
	std::transform(this->downloaded_vector.begin(), this->downloaded_vector.end(), this->downloaded_vector.begin(), [&](now_type obj) {	return obj * _num; });
}

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector.clear();
	this->downloaded_vector.shrink_to_fit();
	this->downloaded_range = make_pair(-1, -1);
}

bool piecewise_container_class::is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension)
{
	if (this->downloaded_range.first <= first_dimension && (this->downloaded_range.second >= first_dimension)) //CHECK SECOND CONDITION
		return true;

	return false;
}

size_t piecewise_container_class::collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const
{
	size_t part = first_dimension / (SIZE_OF_PIECE / 2);
	size_t ret = ((size_t)first_dimension * (size_t)this->get_count_of_collocations() * (size_t)(COLLOC_DIST + 1) + (size_t)second_dimension * (COLLOC_DIST + 1) + third_dimension) - (size_t)(this->downloaded_vector.capacity() * part);
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

void piecewise_container_class::summ_for_concret_colloc(size_t index, now_type for_sum)
{
	this->downloaded_vector[index] += for_sum;
}

void piecewise_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = perem;
}