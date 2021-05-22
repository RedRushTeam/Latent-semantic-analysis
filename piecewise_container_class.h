#pragma once
#include "container_class_interface.h"

static int text_counter = 0;


class piecewise_container_class :
    public container_class_interface
{
public:
    //constr
    piecewise_container_class(short k, int count_of_collocations, fs::path path_to_db, pair<int, int> downloaded_range) : 
        container_class_interface(k, count_of_collocations), path_to_db(path_to_db), downloaded_range(downloaded_range){
        this->downloaded_vector.resize((size_t)SIZE_OF_PIECE * count_of_collocations * COLLOC_DIST, NULL);
    }

    piecewise_container_class(short k, int count_of_collocations, bool random_number) : container_class_interface(k, count_of_collocations)
    {
        this->downloaded_vector.resize((size_t)SIZE_OF_PIECE * count_of_collocations * COLLOC_DIST, NULL);
    }
    

    int now_counter;

    //destr
    ~piecewise_container_class() {
        //fs::remove("C:\\Users\\beerR2600\\YandexDisk\\SVDCollocationAnalyzer\\SVDCollocationAnalyzer\\text" + to_string(now_counter) + ".db");
    }

    //methods
    void clear_vec();
    void upload_vec();
    bool is_data_for_this_colloc_downloaded(int first_dimension, int second_dimension, int third_dimension);
    int collect_one_coordinate_from_three(int first_dimension, int second_dimension, int third_dimension) const;

    //setters&getters
    void set_downloaded_range(pair<int, int> downloaded_range);
    pair<int, int> get_downloaded_range() const;
    // Унаследовано через container_class_interface
    virtual void summ_for_concret_colloc(int first_dimension, int second_dimension, int third_dimension, now_type _num) override;
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) override;
    virtual shared_ptr<container_class_interface> pow_all(int stepen) override;
    virtual shared_ptr<container_class_interface> sqrt_all() override;

    //operators
    // Унаследовано через container_class_interface
    virtual shared_ptr <container_class_interface> operator+(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator+(now_type _koef) override;
    virtual shared_ptr<container_class_interface> operator-(shared_ptr<container_class_interface> summed_class) override;
    virtual shared_ptr<container_class_interface> operator-(now_type _num) override;
    virtual void operator+=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator+=(now_type _num) override;
    virtual void operator-=(shared_ptr<container_class_interface> summed_class) override;
    virtual void operator/=(now_type _num) override;
    virtual void operator-=(now_type _koef) override;
    virtual bool operator==(shared_ptr<container_class_interface> compared_class) override;
    virtual bool operator!=(shared_ptr<container_class_interface> compared_class) override;
    virtual shared_ptr<container_class_interface> operator*(shared_ptr<container_class_interface> multipliable_class) override;
    virtual shared_ptr<container_class_interface> operator*(now_type _koef) override;
    virtual shared_ptr<container_class_interface> operator/(shared_ptr<container_class_interface> dividor_class) override;
    virtual shared_ptr<container_class_interface> operator/(now_type _koef) override;

private:
    vector<now_type> downloaded_vector;
    pair<int, int> downloaded_range;
    fs::path path_to_db;
    unordered_map<string, pair<int, int>> _filenames;
};

