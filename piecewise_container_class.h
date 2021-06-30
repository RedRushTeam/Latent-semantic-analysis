#pragma once
#include "container_class_interface.h"

static int text_counter = 0;


class piecewise_container_class :
    public container_class_interface
{
public:
    //constr
    piecewise_container_class(short k, int count_of_collocations, pair<int, int> downloaded_range) :
        container_class_interface(k, count_of_collocations), downloaded_range(downloaded_range) {
        if (count_of_collocations < (SIZE_OF_PIECE / 2))
            this->downloaded_vector.resize((size_t)count_of_collocations * count_of_collocations * (COLLOC_DIST + 1), NULL);
        if (count_of_collocations >= (SIZE_OF_PIECE / 2))
            this->downloaded_vector.resize((size_t)SIZE_OF_PIECE / 2 * count_of_collocations * (COLLOC_DIST + 1), NULL);
    }

    piecewise_container_class(short k, int count_of_collocations) :
        container_class_interface(k, count_of_collocations) {
        if (count_of_collocations < (SIZE_OF_PIECE / 2))
            this->downloaded_vector.resize((size_t)count_of_collocations * count_of_collocations * (COLLOC_DIST + 1), NULL);
        if (count_of_collocations >= (SIZE_OF_PIECE / 2))
            this->downloaded_vector.resize((size_t)SIZE_OF_PIECE / 2 * count_of_collocations * (COLLOC_DIST + 1), NULL);
    }

    //destr
    ~piecewise_container_class() {
        //fs::remove("C:\\Users\\beerR2600\\YandexDisk\\SVDCollocationAnalyzer\\SVDCollocationAnalyzer\\text" + to_string(now_counter) + ".db");
    }

    //methods
    void clear_vec();
    bool is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension);
    size_t collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const;
    void fill_vector(now_type number_for_fill);
    now_type get_count_of_concret_collocation_with_one_coordinate(size_t _i);
    void set_count_of_concret_collocation_with_one_coordinate(size_t _i, now_type _perem);
    size_t get_vector_size();

    //setters&getters
    void set_downloaded_range(pair<int, int> downloaded_range);
    pair<int, int> get_downloaded_range() const;
    // Унаследовано через container_class_interface
    virtual void summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) override;
    void summ_for_concret_colloc(size_t index, now_type for_sum);
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
    vector<now_type> downloaded_vector;
    pair<int, int> downloaded_range;
};