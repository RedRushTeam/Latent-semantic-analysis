﻿#pragma once 
#include "piecewise_container_class.h"

void piecewise_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;
	auto iter = this->downloaded_vector.find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));

	if (iter == this->downloaded_vector.end())
		this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = 1;
	else
		this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = iter.value() + 1;
}

void piecewise_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return;
	auto iter = this->downloaded_vector.find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));

	if (iter == this->downloaded_vector.end())
		this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = -1;
	else
		this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)] = iter.value() - 1;
}

now_type piecewise_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
	if (!is_data_for_this_colloc_downloaded(first_dimension, second_dimension, third_dimension))
		return -1.;

	auto iter = this->downloaded_vector.find(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));
	if(iter == this->downloaded_vector.end())
		return 0;
	else
		return this->downloaded_vector[this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)];;
}

void piecewise_container_class::pow_all(int stepen)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = pow(obj.second/*.perem*/, stepen);
}

void piecewise_container_class::sqrt_all()
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = sqrt(obj.second/*.perem*/);
}

void piecewise_container_class::operator+=(now_type _num)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = obj.second + _num;
}

void piecewise_container_class::operator-=(shared_ptr<container_class_interface> summed_class)
{
	if (summed_class->get_count_of_collocations() != this->get_count_of_collocations()) {	//TODO add check downloaded range!
		exit(-228);
		return;
	}

	for (size_t i = this->get_downloaded_range().first; i < this->get_downloaded_range().second; ++i)
		for (size_t j = 0; j < this->get_count_of_collocations(); ++j)
			for (size_t k = 0; k <= COLLOC_DIST; ++k) {
				auto shit = summed_class->get_count_of_concret_collocation(i, j, k);
				if (!shit/*.perem*/)
					continue;
				this->set_count_of_concret_collocation(i, j, k, this->get_count_of_concret_collocation(i, j, k) - shit);
			}
} 

void piecewise_container_class::operator-=(now_type _koef)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = obj.second - _koef;
}

void piecewise_container_class::operator/=(now_type _num)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = obj.second / _num;
}

void piecewise_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
	for (size_t i = this->get_downloaded_range().first; i < this->get_downloaded_range().second; ++i)
		for (size_t j = 0; j < this->get_count_of_collocations(); ++j)
			for (size_t k = 0; k <= COLLOC_DIST; ++k) {
				auto shit = summed_class->get_count_of_concret_collocation(i, j, k);
				if (!shit/*.perem*/)
					continue;
				this->set_count_of_concret_collocation(i, j, k, this->get_count_of_concret_collocation(i, j, k) + shit);
			}
}

void piecewise_container_class::operator*=(now_type _num)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = obj.second * _num;
}

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector.clear();
}

bool piecewise_container_class::is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension)
{
	if (this->downloaded_range.first <= first_dimension && (this->downloaded_range.second >= first_dimension)) //CHECK SECOND CONDITION
		return true;

	return false;
}

size_t piecewise_container_class::collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const
{
	size_t part = first_dimension / (SIZE_OF_PIECE);
	size_t ret = ((size_t)first_dimension * (size_t)this->get_count_of_collocations() * (size_t)(COLLOC_DIST + 1) + (size_t)second_dimension * (COLLOC_DIST + 1) + third_dimension) - (size_t)(this->downloaded_vector.size() * part);
	return ret;
}

void piecewise_container_class::fill_vector(now_type number_for_fill)
{
	for (auto& obj : this->downloaded_vector)
		this->downloaded_vector[obj.first] = number_for_fill;
}

size_t piecewise_container_class::get_vector_size()
{
	return this->downloaded_vector.size();
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