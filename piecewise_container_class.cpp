#pragma once 
#include "piecewise_container_class.h"

void piecewise_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
}

void piecewise_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
}

now_type piecewise_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
	return now_type();
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

void piecewise_container_class::operator/=(now_type _num)
{
}
