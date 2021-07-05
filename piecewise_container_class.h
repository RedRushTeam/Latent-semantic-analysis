#pragma once
#include "container_class_interface.h"

class piecewise_container_class :
    public container_class_interface
{
public:
    //constr
    piecewise_container_class(short k, int count_of_collocations, pair<int, int> downloaded_range) :
        container_class_interface(k, count_of_collocations), downloaded_range(downloaded_range) {
        this->downloaded_vector = make_shared<tsl::robin_map<int, now_type>>();

        if (count_of_collocations < (SIZE_OF_PIECE))
            this->now_real_size = (size_t)count_of_collocations * count_of_collocations * (COLLOC_DIST + 1);
        if (count_of_collocations >= (SIZE_OF_PIECE))
            this->now_real_size = (size_t)SIZE_OF_PIECE * count_of_collocations * (COLLOC_DIST + 1);
    }

    piecewise_container_class(short k, int count_of_collocations) :
        container_class_interface(k, count_of_collocations) {
        this->downloaded_vector = make_shared<tsl::robin_map<int, now_type>>();

        if (count_of_collocations < (SIZE_OF_PIECE))
            this->now_real_size = (size_t)count_of_collocations * count_of_collocations * (COLLOC_DIST + 1);
        if (count_of_collocations >= (SIZE_OF_PIECE))
            this->now_real_size = (size_t)SIZE_OF_PIECE * count_of_collocations * (COLLOC_DIST + 1);
    }

    //destr
    ~piecewise_container_class() {
        //fs::remove("C:\\Users\\beerR2600\\YandexDisk\\SVDCollocationAnalyzer\\SVDCollocationAnalyzer\\text" + to_string(now_counter) + ".db");
    }

    //methods
    void clear_vec();
    bool is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension);
    size_t collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const;
    three_coordinate_structure split_three_coordinates_from_one(size_t index) const;
    void fill_vector(now_type number_for_fill);
    size_t get_vector_size();

    //setters&getters
    void set_downloaded_range(pair<int, int> downloaded_range);
    pair<int, int> get_downloaded_range() const;
    shared_ptr<tsl::robin_map<int, now_type>> get_vector_ptr() const;
    // Унаследовано через container_class_interface
    virtual void summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) override;
    virtual void minus_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) override;
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) override;
    virtual void pow_all(int stepen) override;
    virtual void sqrt_all() override;

    //operators
    // Унаследовано через container_class_interface
    virtual void operator+=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator+=(now_type _num) override;
    virtual void operator-=(shared_ptr<container_class_interface> deductible_class) override;
    virtual void operator/=(now_type _num) override;
    virtual void operator-=(now_type _koef) override;
    virtual void operator*=(now_type _num) override;

private:
    size_t now_real_size;
    shared_ptr<tsl::robin_map<int, now_type>> downloaded_vector;
    //boost::numeric::ublas::mapped_vector<now_type> downloaded_vector;
    pair<int, int> downloaded_range;
};