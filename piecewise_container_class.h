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
    piecewise_container_class(short k, int count_of_collocations) : container_class_interface(k, count_of_collocations) 
    {
        /*sqlite3* db;

        string textname = "G:\\TEST_db\\text" + to_string(text_counter) + ".db";
        sqlite3_open(textname.c_str(), &db);
        text_counter++;

        this->_filename = textname;

        string sql = "CREATE TABLE TEXT("  \
            "ID STRING PRIMARY KEY NOT NULL," \
            "CHISLO REAL NOT NULL);";

        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

        sqlite3_close(db);

        SQLite::Database db1(this->_filename, SQLite::OPEN_READWRITE);

        string ins = "";

        std::cout << "Maximum size of a string is " << ins.max_size()
            << " (pointer size: " << CHAR_BIT * sizeof(void*)
            << " bits)\n";

        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));
        ins.reserve((int)(100 * 100 * count_of_collocations * k));

        std::cout << endl << "Size of a string is " << ins.capacity() << endl;
        int shit = 0;
        for (int i = 0; i < count_of_collocations; ++i)
            for (int j = 0; j < count_of_collocations; ++j)
                for (int l = 0; l < k; ++l)
                {
                    if (((shit + i) % 700) > 698) {
                        SQLite::Transaction transaction(db1);
                        db1.exec(ins);
                        transaction.commit();
                        ins.clear();
                        shit = i;
                    }

                    ins += "INSERT or IGNORE INTO TEXT(ID, CHISLO) VALUES (\"" + to_string(i) + "!" + to_string(j) + "!" + to_string(l) + "\", 7.3); ";
                }



        fs::permissions(this->_filename, fs::perms::owner_all | fs::perms::group_all, fs::perm_options::add);
        */
        //need to call db correct;
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
    string _filename;

    string bufer;
};

