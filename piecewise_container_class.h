#pragma once
#include "container_class_interface.h"

struct three_coordinate_structure {
    int number_of_first_collocation;
    int number_of_second_collocation;
    short k_;
};

namespace std
{
    template<> struct hash<three_coordinate_structure>
    {
        typedef three_coordinate_structure argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            std::size_t seed = 0;
            boost::hash_combine(seed, std::hash<int>{}(s.number_of_first_collocation));
            boost::hash_combine(seed, std::hash<int>{}(s.number_of_second_collocation));
            boost::hash_combine(seed, std::hash<short>{}(s.k_));
            return seed;
        }
    };
}

class piecewise_container_class :
    public container_class_interface
{
public:
    //constr
    piecewise_container_class(short k, int count_of_collocations) : container_class_interface(k, count_of_collocations) {}

    //methods


    // ”наследовано через container_class_interface
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual shared_ptr<container_class_interface> pow_all(int stepen) override;
    virtual shared_ptr<container_class_interface> sqrt_all() override;

    //operators
    // ”наследовано через container_class_interface
    virtual shared_ptr <container_class_interface> operator+(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator+(now_type _koef) override;
    virtual shared_ptr<container_class_interface> operator-(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator-(now_type _num) override;
    virtual void operator+=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator+=(now_type _num) override;
    virtual void operator-=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator-=(now_type _koef) override;
    virtual bool operator==(shared_ptr<container_class_interface> compared_class) override;
    virtual bool operator!=(shared_ptr<container_class_interface> compared_class) override;
    virtual shared_ptr<container_class_interface> operator*(shared_ptr<container_class_interface> multipliable_class) override;
    virtual shared_ptr<container_class_interface> operator*(now_type _koef) override;
    virtual shared_ptr<container_class_interface> operator/(shared_ptr<container_class_interface> dividor_class) override;
    virtual shared_ptr<container_class_interface> operator/(now_type _koef) override;

private:
    unordered_map<three_coordinate_structure, now_type> downloaded_map;
    set<int> number_of_downloaded_slice;
    //путь к бд кокретно этого файла
};

