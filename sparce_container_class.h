#pragma once
#include "piecewise_container_class.h"
class sparce_container_class :
    public container_class_interface
{
public:
    //constr
    sparce_container_class(short k, int count_of_collocations) : container_class_interface(k, count_of_collocations) {
            this->sparce_vec.resize(count_of_collocations, NULL);
    }

    //methods
    size_t collect_one_coordinate_from_three(size_t first_dimension, size_t second_dimension, size_t third_dimension);
    void calculate_and_sum_parametr_to_one_term(shared_ptr<container_class_interface> _parametr);
    
    // Унаследовано через container_class_interface
    virtual void summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) override;
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) override;
    virtual void pow_all(int stepen) override;
    virtual void sqrt_all() override;

    //operators
    virtual shared_ptr<container_class_interface> operator+(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator+(now_type _koef) override;
    virtual shared_ptr<container_class_interface> operator-(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator-(now_type _num) override;
    virtual void operator+=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator/=(now_type _num) override;
    virtual void operator+=(now_type _num) override;
    virtual void operator-=(shared_ptr<container_class_interface> deductible_class) override;
    virtual void operator-=(now_type _koef) override;
    virtual bool operator==(shared_ptr<container_class_interface> compared_class) override;
    virtual bool operator!=(shared_ptr<container_class_interface> compared_class) override;
    virtual void operator*=(now_type _num) override;
    virtual shared_ptr<container_class_interface> operator*(shared_ptr<container_class_interface> multipliable_class) override;
    virtual shared_ptr<container_class_interface> operator*(now_type _koef) override;
    virtual void operator/(shared_ptr<container_class_interface> dividor_class) override;
    virtual void operator/(now_type _koef) override;

private:
    //sparce vec
    vector<now_type> sparce_vec;
};

