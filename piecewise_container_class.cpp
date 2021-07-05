#pragma once 
#include "piecewise_container_class.h"

void piecewise_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;
	auto iter = this->downloaded_vector->find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));

	if (iter == this->downloaded_vector->end())
		(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = 1;
	else
		(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = iter.value() + 1;
}

void piecewise_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;
	auto iter = this->downloaded_vector->find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));

	if (iter == this->downloaded_vector->end())
		(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = -1;
	else
		(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = iter.value() - 1;
}

now_type piecewise_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return -1.;

	auto iter = this->downloaded_vector->find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));
	if(iter == this->downloaded_vector->end())
		return 0;
	else
		return (*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)];;
}

void piecewise_container_class::pow_all(int stepen)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = pow(obj.second, stepen);
}

void piecewise_container_class::sqrt_all()
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = sqrt(obj.second);
}

void piecewise_container_class::operator+=(now_type _num)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = obj.second + _num;
}

void piecewise_container_class::operator-=(shared_ptr<container_class_interface> summed_class)
{
	if (summed_class->get_count_of_collocations() != this->get_count_of_collocations()) {	//TODO add check downloaded range!
		exit(-228);
		return;
	}

	auto summed_class_like_piecewise = dynamic_pointer_cast<piecewise_container_class>(summed_class);

	for (auto& obj : *summed_class_like_piecewise->get_vector_ptr()) {
		auto three_coord = this->split_three_coordinates_from_one(obj.first);

		this->minus_for_concret_colloc(three_coord.first_coord, three_coord.second_coord, three_coord.k, obj.second);
	}
} 

void piecewise_container_class::operator-=(now_type _koef)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = obj.second - _koef;
}

void piecewise_container_class::operator/=(now_type _num)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = obj.second / _num;
}

void piecewise_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
	if (summed_class->get_count_of_collocations() != this->get_count_of_collocations()) {
		exit(-228);
		return;
	}

	auto summed_class_like_piecewise = dynamic_pointer_cast<piecewise_container_class>(summed_class);

	for (auto& obj : *summed_class_like_piecewise->get_vector_ptr()) {
		auto three_coord = this->split_three_coordinates_from_one(obj.first);

		this->summ_for_concret_colloc(three_coord.first_coord, three_coord.second_coord, three_coord.k, obj.second);
	}
}

void piecewise_container_class::operator*=(now_type _num)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = obj.second * _num;
}

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector->clear();
}

bool piecewise_container_class::is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension)
{
	if (this->downloaded_range.first <= first_dimension && (this->downloaded_range.second >= first_dimension)) //CHECK SECOND CONDITION
		return true;

	return false;
}

size_t piecewise_container_class::collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const
{
	return ((size_t)first_dimension * (size_t)this->get_count_of_collocations() * (size_t)(global_var::COLLOC_DIST + 1) + (size_t)second_dimension * (global_var::COLLOC_DIST + 1) + third_dimension);
}

three_coordinate_structure piecewise_container_class::split_three_coordinates_from_one(size_t index) const
{
	three_coordinate_structure ret;
	for (int i = 0; i <= 3; ++i)
		if (!((index - i) % 4))
		{
			index = (index - i) / 4;
			ret.k = i;
			break;
		}

	size_t low = index / (size_t)this->get_count_of_collocations();

	for (size_t i = low; i < this->get_count_of_collocations(); ++i) {
		size_t minus = i * (size_t)this->get_count_of_collocations();
		size_t cond = index - minus;
		bool c1 = cond < this->get_count_of_collocations();
		bool c2 = cond >= 0;
		if (c1 && c2)
		{
			ret.first_coord = i;
			ret.second_coord = index - (size_t)this->get_count_of_collocations() * i;
			break;
		}
	}

	return ret;
}

void piecewise_container_class::fill_vector(now_type number_for_fill)
{
	for (auto& obj : *this->downloaded_vector)
		(*this->downloaded_vector)[obj.first] = number_for_fill;
}

size_t piecewise_container_class::get_vector_size()
{
	return this->downloaded_vector->size();
}

void piecewise_container_class::set_downloaded_range(pair<int, int> downloaded_range)
{
	this->downloaded_range.first = downloaded_range.first;

	if (downloaded_range.second < this->get_count_of_collocations())
		this->downloaded_range.second = downloaded_range.second;
	else
		this->downloaded_range.second = this->get_count_of_collocations();
}

pair<int, int> piecewise_container_class::get_downloaded_range() const
{
	return this->downloaded_range;
}

shared_ptr<tsl::robin_map<int, now_type>> piecewise_container_class::get_vector_ptr() const
{
	return this->downloaded_vector;
}

void piecewise_container_class::summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] += _num;
}

void piecewise_container_class::minus_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] -= _num;
}

void piecewise_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;

	(*this->downloaded_vector)[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = perem;
}