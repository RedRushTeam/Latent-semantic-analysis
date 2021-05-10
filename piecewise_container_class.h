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

inline bool operator== (three_coordinate_structure lefty, three_coordinate_structure righty)
{
    if ((lefty.number_of_first_collocation == righty.number_of_first_collocation) && (lefty.number_of_second_collocation == righty.number_of_second_collocation) && (lefty.k_ == righty.k_))
        return true;
    return false;
};

static int text_counter = 0;


class piecewise_container_class :
    public container_class_interface
{
public:
    //constr
    piecewise_container_class(short k, int count_of_collocations) {};
    piecewise_container_class(short k, int count_of_collocations, bool random_number) : container_class_interface(k, count_of_collocations)
    {
        int zapisey_in_one_db = 4000000;
        int _commiter = 10000;
        int number_of_full_db = count_of_collocations * count_of_collocations * k / zapisey_in_one_db; //4000000 now fastest
        int terms_in_one_db = zapisey_in_one_db / count_of_collocations / k;
        int tails = number_of_full_db * zapisey_in_one_db;
        int rc;

        MDBX_env* env = NULL;
        MDBX_dbi dbi = 0;
        MDBX_val index, number;
        MDBX_txn* txn = NULL;
        MDBX_cursor* cursor = NULL;
        //char sval[32];
        int found = 0;
        int i, j, l;

        string _index;
        float _value;
        int counter = 0;

        std::mt19937 gen(time(NULL));
        std::uniform_real_distribution<> uid(0, 1);
        
        string _for_size = "99999!99999!4";

        for (int t = 0; t < number_of_full_db; t++)
        {
            //auto start_size = 0.5 * 1024 * 1024 * 1024; // 0.01 has been got by handle calculation
            int start_size = zapisey_in_one_db * (sizeof(float) + sizeof(_for_size));
            //cout << "str: " << sizeof(_for_size) << " fl: " << sizeof(float);

            // this variables should be calculated automatically by database size
            int left_term;
            left_term = terms_in_one_db * t;
            int right_term = left_term+terms_in_one_db;

            cout << left_term;
            //
            string textnameparam;
            random_number ? textnameparam = "randoms" : textnameparam = "concrets";
            //string textname = "./" + to_string(k*count_of_collocations*count_of_collocations) + " zapisey by " + textnameparam; // ВМЕСТО ./ МОЖНО ПОДСТАВИТЬ ЛЮБОЙ АДРЕС
            string textname = "./text" + to_string(text_counter) + " terms " + to_string(left_term) + "-" + to_string(right_term);

            if (fs::exists(textname))
                fs::remove(textname);//debug

            this->_filenames.insert(make_pair(textname, make_pair(left_term, right_term)));
            ////////// СОЗДАНИЕ ФАЙЛА БД
            rc = mdbx_env_create(&env);
            if (rc != MDBX_SUCCESS) {
                fprintf(stderr, "mdbx_env_create: (%d) %s\n", rc, mdbx_strerror(rc));
                goto bailout;
            }


            /////////////ВЫДЕЛЕНИЕ ПАМЯТИ
            //mdbx_env_set_mapsize(env, count_of_collocations * count_of_collocations * k); ///deprecated
            rc = mdbx_env_set_geometry(env, start_size, -1,5*start_size, -1, -1, -1); //место для поиска скорости

            rc = mdbx_env_open(env, textname.c_str(),
                MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM, 0664); //0664 - what is it ?
            if (rc != MDBX_SUCCESS) {
                fprintf(stderr, "mdbx_env_open: (%d) %s\n", rc, mdbx_strerror(rc));
                goto bailout;
            }

            //mdbx_dbi_close(env, dbi);
            ////// ФЛАГИ, ПОКАЗЫВАЮЩИЕ ОШИБКИ
            /*
            MDBX_EINVAL;
            MDBX_EPERM;
            MDBX_EACCESS;
            MDBX_MAP_FULL;
            MDBX_TOO_LARGE;

            */



            //////////// СОЗДАНИЕ ДАТАСЕТА ВНУТРИ ФАЙЛА БД

            for (i = left_term; i < right_term; ++i)
                for (j = 0; j < count_of_collocations; ++j)
                    for (l = 0; l < k; ++l) {
                        if (counter == 0) {
                            rc = mdbx_txn_begin(env, NULL, MDBX_TXN_READWRITE, &txn);

                            if (rc != MDBX_SUCCESS) {
                                fprintf(stderr, "mdbx_txn_begin: (%d) %s\n", rc, mdbx_strerror(rc));
                                goto bailout;
                            }

                            rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
                            if (rc != MDBX_SUCCESS) {
                                fprintf(stderr, "mdbx_dbi_open: (%d) %s\n", rc, mdbx_strerror(rc));
                                goto bailout;
                            }
                        }
                        _index = to_string(i) + "!" + to_string(j) + "!" + to_string(l);
                        random_number ? uid(gen) : 0.0;
                        index.iov_len = sizeof(_index);
                        index.iov_base = &_index;
                        number.iov_len = sizeof(float);
                        number.iov_base = &_value;

                        //sprintf(sval, "%03x %d foo bar", 32, 3141592);


                        ////////ДОБАВЛЕНИЕ ДАННЫХ В БД
                        rc = mdbx_put(txn, dbi, &index, &number, MDBX_UPSERT);
                        if (rc != MDBX_SUCCESS) {
                            fprintf(stderr, "mdbx_put: (%d) %s\n", rc, mdbx_strerror(rc));
                            goto bailout;
                        }

                        ++counter;
                        if (counter == _commiter) {
                            rc = mdbx_txn_commit(txn);
                            if (rc) {
                                fprintf(stderr, "mdbx_txn_commit: (%d) %s\n", rc, mdbx_strerror(rc));
                                goto bailout;
                            }
                            counter = 0;
                            txn = NULL;
                        }

                    }

            if (txn != NULL) {
                rc = mdbx_txn_commit(txn);
                if (rc) {
                    fprintf(stderr, "mdbx_txn_commit: (%d) %s\n", rc, mdbx_strerror(rc));
                    goto bailout;
                }
                txn = NULL;
            }
        }

        
        for (i = tails; i < count_of_collocations; ++i)
            for (j = 0; j < count_of_collocations; ++j)
                for (l = 0; l < k; ++l) {
                    if (counter == 0) {
                        rc = mdbx_txn_begin(env, NULL, MDBX_TXN_READWRITE, &txn);

                        if (rc != MDBX_SUCCESS) {
                            fprintf(stderr, "mdbx_txn_begin: (%d) %s\n", rc, mdbx_strerror(rc));
                            goto bailout;
                        }

                        rc = mdbx_dbi_open(txn, NULL, MDBX_DB_DEFAULTS, &dbi);
                        if (rc != MDBX_SUCCESS) {
                            fprintf(stderr, "mdbx_dbi_open: (%d) %s\n", rc, mdbx_strerror(rc));
                            goto bailout;
                        }
                    }
                    _index = to_string(i) + "!" + to_string(j) + "!" + to_string(l);
                    random_number ? uid(gen) : 0.0;
                    index.iov_len = sizeof(_index);
                    index.iov_base = &_index;
                    number.iov_len = sizeof(float);
                    number.iov_base = &_value;

                    //sprintf(sval, "%03x %d foo bar", 32, 3141592);


                    ////////ДОБАВЛЕНИЕ ДАННЫХ В БД
                    rc = mdbx_put(txn, dbi, &index, &number, MDBX_UPSERT);
                    if (rc != MDBX_SUCCESS) {
                        fprintf(stderr, "mdbx_put: (%d) %s\n", rc, mdbx_strerror(rc));
                        goto bailout;
                    }

                    ++counter;
                    if (counter == _commiter) {
                        rc = mdbx_txn_commit(txn);
                        if (rc) {
                            fprintf(stderr, "mdbx_txn_commit: (%d) %s\n", rc, mdbx_strerror(rc));
                            goto bailout;
                        }
                        counter = 0;
                        txn = NULL;
                    }

                }

        if (txn != NULL) {
            rc = mdbx_txn_commit(txn);
            if (rc) {
                fprintf(stderr, "mdbx_txn_commit: (%d) %s\n", rc, mdbx_strerror(rc));
                goto bailout;
            }
            txn = NULL;
        }
        //////////////////////////// ТРАНЗАКЦИЯ ПОЛУЧЕНИЯ ДАННЫХ ИЗ БАЗЫ
        /*rc = mdbx_txn_begin(env, NULL, MDBX_TXN_RDONLY, &txn);
        if (rc != MDBX_SUCCESS) {
            fprintf(stderr, "mdbx_txn_begin: (%d) %s\n", rc, mdbx_strerror(rc));
            goto bailout;
        }
        rc = mdbx_cursor_open(txn, dbi, &cursor);
        if (rc != MDBX_SUCCESS) {
            fprintf(stderr, "mdbx_cursor_open: (%d) %s\n", rc, mdbx_strerror(rc));
            goto bailout;
        }

        while ((rc = mdbx_cursor_get(cursor, &index, &number, MDBX_NEXT)) == 0) {
            
            cout << "key: " << *(string*)index.iov_base << " value: " << *(float*)number.iov_base;
            found += 1;
        }
        if (rc != MDBX_NOTFOUND || found == 0) {
            fprintf(stderr, "mdbx_cursor_get: (%d) %s\n", rc, mdbx_strerror(rc));
            goto bailout;
        }
        else {
            rc = MDBX_SUCCESS;
        }*/
        ///////////////// ЗАВЕРШЕНИЕ ТРАНЗАКЦИИ; ЗАКРЫТИЕ ДАТАСЕТА И БД
        //system("pause");
        /*if (fs::exists(textname))
            fs::remove(textname);//debug*/

    bailout:
        if (cursor)
            mdbx_cursor_close(cursor);
        if (txn)
            mdbx_txn_abort(txn);
        if (dbi)
            mdbx_dbi_close(env, dbi);
        if (env)
            mdbx_env_close(env);
        cout << (rc != MDBX_SUCCESS) ? EXIT_FAILURE : EXIT_SUCCESS;

        ++text_counter;

        
    }
    

    int now_counter;

    //destr
    ~piecewise_container_class() {
        //fs::remove("C:\\Users\\beerR2600\\YandexDisk\\SVDCollocationAnalyzer\\SVDCollocationAnalyzer\\text" + to_string(now_counter) + ".db");
    }

    //methods
    void load_slice(int first_dimesion);
    void clear_map();
    bool is_slice_loaded(int first_dimension);


    // Унаследовано через container_class_interface
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) override;
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

    // Унаследовано через container_class_interface
    virtual void operator/=(now_type _num) override;
    unordered_map<string, pair<int, int>> _filenames;
};

