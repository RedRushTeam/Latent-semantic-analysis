#include "sparce_container_class.h"

size_t sparce_container_class::collect_one_coordinate_from_three(size_t first_dimension, size_t second_dimension, size_t third_dimension)
{
    if (first_dimension != second_dimension) {
        nullptr;
        exit(-3);
    }        

    return first_dimension;
}

void sparce_container_class::calculate_and_sum_parametr_to_one_term(shared_ptr<container_class_interface> _parametr)
{
    if (!dynamic_cast<piecewise_container_class*>(_parametr.get()))
        return;

    //многопоток?
    for (auto q = dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().first; q < dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().second; ++q)	//check it
        for (auto j = dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().first; j < dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().second; ++j)
            for (int p = 0; p <= global_var::COLLOC_DIST; ++p)
                this->set_count_of_concret_collocation(q, q, p, this->get_count_of_concret_collocation(q, q, p) + _parametr->get_count_of_concret_collocation(q, j, p));
}

void sparce_container_class::summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num)
{
}

void sparce_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
}

void sparce_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
}

now_type sparce_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
    return this->sparce_vec.at(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension));
}

void sparce_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
    this->sparce_vec.at(this->collect_one_coordinate_from_three(first_dimension, second_dimension, third_dimension)) = perem;
}

void sparce_container_class::pow_all(int stepen)
{

}

void sparce_container_class::sqrt_all()
{
}

void sparce_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
}

void sparce_container_class::operator/=(now_type _num)
{
}

void sparce_container_class::operator+=(now_type _num)
{
}

void sparce_container_class::operator-=(shared_ptr<container_class_interface> deductible_class)
{
}

void sparce_container_class::operator-=(now_type _koef)
{
}

void sparce_container_class::operator*=(now_type _num)
{
}

void sparce_container_class::erase_concret_colloc(int first_dimension, int second_dimension, int third_dimension)
{
}

void sparce_container_class::minus_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num)
{
}
