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

void piecewise_container_class::clear_vec()
{
	this->downloaded_vector.clear();
}

void piecewise_container_class::upload_vec()
{
	//method for Dima
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
