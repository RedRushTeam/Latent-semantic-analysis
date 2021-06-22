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
    for (auto q = dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().first; q < dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().second; ++q)
        for (auto j = dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().first; j < dynamic_pointer_cast<piecewise_container_class>(_parametr)->get_downloaded_range().second; ++j)
            for (int p = 0; p <= COLLOC_DIST; ++p)
                this->set_count_of_concret_collocation(q, q, p, this->get_count_of_concret_collocation(q, q, p) + _parametr->get_count_of_concret_collocation(q, j, p));
}

now_type sparce_container_class::get_count_of_concret_collocation_for_one_coord(size_t coordinate) const
{
    return this->sparce_vec[coordinate];
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

shared_ptr<container_class_interface> sparce_container_class::pow_all(int stepen)
{
    return shared_ptr<container_class_interface>();
}

void sparce_container_class::sqrt_all()
{
    std::transform(this->sparce_vec.begin(), this->sparce_vec.end(), this->sparce_vec.begin(), [&](now_type& obj) {	return sqrt(obj); });
}

shared_ptr<container_class_interface> sparce_container_class::operator+(shared_ptr<container_class_interface> summed_class)
{
    return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> sparce_container_class::operator+(now_type _koef)
{
    return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> sparce_container_class::operator-(shared_ptr<container_class_interface> summed_class)
{
    return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> sparce_container_class::operator-(now_type _num)
{
    return shared_ptr<container_class_interface>();
}

void sparce_container_class::operator/=(now_type _num)
{
    for (size_t i = 0; i < this->sparce_vec.size(); ++i)
        this->sparce_vec[i] /= _num;
}

void sparce_container_class::operator+=(now_type _num)
{
}

void sparce_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
    if (!dynamic_cast<sparce_container_class*>(summed_class.get()))
        return;

    for (size_t i = 0; i < this->sparce_vec.size(); ++i)
        this->sparce_vec[i] += dynamic_pointer_cast<sparce_container_class>(summed_class)->get_count_of_concret_collocation_for_one_coord(i);
}

void sparce_container_class::operator-=(shared_ptr<container_class_interface> deductible_class)
{
    if (dynamic_pointer_cast<sparce_container_class>(deductible_class) == nullptr)
        return;

    for (size_t i = 0; i < this->sparce_vec.size(); ++i)
        this->sparce_vec[i] -= dynamic_pointer_cast<sparce_container_class>(deductible_class)->get_count_of_concret_collocation_for_one_coord(i);
}

void sparce_container_class::operator-=(now_type _koef)
{
    std::transform(this->sparce_vec.begin(), this->sparce_vec.end(), this->sparce_vec.begin(), [&](now_type& obj) {	return obj - _koef; });
}

bool sparce_container_class::operator==(shared_ptr<container_class_interface> compared_class)
{
    return false;
}

bool sparce_container_class::operator!=(shared_ptr<container_class_interface> compared_class)
{
    return false;
}

shared_ptr<container_class_interface> sparce_container_class::operator*(shared_ptr<container_class_interface> multipliable_class)
{
    return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> sparce_container_class::operator*(now_type _koef)
{
    return shared_ptr<container_class_interface>();
}

void sparce_container_class::operator/(shared_ptr<container_class_interface> dividor_class)
{

}

void sparce_container_class::operator/(now_type _koef)
{
}

void sparce_container_class::operator*=(now_type _num)
{
}
