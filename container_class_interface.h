#pragma once
#include "list_of_functions.h"
class container_class_interface
{
public:
	//constr
	container_class_interface() {}
	container_class_interface(short k, int count_of_collocations) : k(k), count_of_collocations(count_of_collocations) {}

	//functions
	virtual void summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) = 0;
	virtual void increment(int first_dimension, int second_dimension, int third_dimension) = 0;
	virtual void decrement(int first_dimension, int second_dimension, int third_dimension) = 0;
	virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) = 0;
	virtual void set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) = 0;
	virtual shared_ptr<container_class_interface> pow_all(int stepen) = 0;
	virtual shared_ptr<container_class_interface> sqrt_all() = 0;

	//operators
	virtual shared_ptr<container_class_interface> operator+(shared_ptr<container_class_interface> summed_class) = 0;
	virtual shared_ptr<container_class_interface> operator+(now_type _koef) = 0;
	virtual shared_ptr<container_class_interface> operator-(shared_ptr<container_class_interface> summed_class) = 0;
	virtual shared_ptr<container_class_interface> operator-(now_type _num) = 0;
	virtual void operator+=(shared_ptr<container_class_interface> summed_class) = 0;
	virtual void operator/=(now_type _num) = 0;
	virtual void operator+=(now_type _num) = 0;
	virtual void operator-=(shared_ptr<container_class_interface> summed_class) = 0;
	virtual void operator-=(now_type _koef) = 0;
	virtual bool operator==(shared_ptr<container_class_interface> compared_class) = 0;
	virtual bool operator!=(shared_ptr<container_class_interface> compared_class) = 0;
	virtual shared_ptr<container_class_interface> operator*(shared_ptr<container_class_interface> multipliable_class) = 0;
	virtual shared_ptr<container_class_interface> operator*(now_type _koef) = 0;
	virtual shared_ptr<container_class_interface> operator/(shared_ptr<container_class_interface> dividor_class) = 0;
	virtual shared_ptr<container_class_interface> operator/(now_type _koef) = 0;

	//getters&&setters
	short get_k() const;
	void set_k(short k);
	int get_count_of_collocations() const;
	void set_count_of_collocations(int count_of_collocations);

private:
	short k;
	int count_of_collocations;
};

