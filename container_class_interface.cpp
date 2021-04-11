#pragma once
#include "container_class_interface.h"

short container_class_interface::get_k() const
{
	return this->k;
}

void container_class_interface::set_k(short k)
{
	this->k = k;
}

int container_class_interface::get_count_of_collocations() const
{
	return this->count_of_collocations;
}

void container_class_interface::set_count_of_collocations(int count_of_collocations)
{
	this->count_of_collocations = count_of_collocations;
}
