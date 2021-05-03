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
        int number_of_terms_in_one_db = 500 / 0.000045 / k / count_of_collocations; //20000=20 GB database size; 0.000045 is statistic multiplier(size of one cell)
        int number_of_full_db = round(count_of_collocations / number_of_terms_in_one_db); // how many full 20 GB databases text includes without tail

        int tail = count_of_collocations - number_of_terms_in_one_db * number_of_full_db;

        for (int i = 0; i < number_of_full_db; ++i)
        {
            sqlite3* db;

            int left_term = number_of_terms_in_one_db * i;
            int right_term = number_of_terms_in_one_db * (i + 1);

            //(tail / number_of_terms_in_one_db > 0.33) ? right_term = number_of_terms_in_one_db * (i + 1) : right_term = count_of_collocations;
            if ((i == number_of_full_db-1) && (tail / number_of_terms_in_one_db < 0.33))
                right_term = count_of_collocations;

            string textname = "text" + to_string(text_counter) + " terms " + to_string(left_term) + "-" + to_string(right_term) + ".db";
            sqlite3_open(textname.c_str(), &db);
            //text_counter++;

            this->_filenames.insert(make_pair(textname, make_pair(left_term, right_term)));

            string sql = "CREATE TABLE TEXT("  \
                "ID STRING PRIMARY KEY NOT NULL," \
                "CHISLO REAL NOT NULL);";

            int rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

            sqlite3_close(db);

            SQLite::Database db1(textname, SQLite::OPEN_READWRITE);
            string pragma = "PRAGMA synchronous=OFF";
            db1.exec(pragma);
            

            int x = clock();
            //string ins = "";
            string ins = "INSERT or IGNORE INTO TEXT(ID, CHISLO) VALUES ";

            std::cout << "Maximum size of a string is " << ins.max_size()
                << " (pointer size: " << CHAR_BIT * sizeof(void*)
                << " bits)\n";

            ins.reserve(1000000);
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));
            //ins.reserve((int)(100 * 100 * count_of_collocations * k));

            //std::cout << endl << "Size of a string is " << ins.capacity() << endl;
            int shit = 0;
            for (int i = left_term; i < right_term; ++i)
                for (int j = 0; j < count_of_collocations; ++j)
                    for (int l = 0; l < k; ++l)
                    {
                        if (ins.size() < 999950)
                            //ins += "UPDATE TEXT set CHISLO = 2.5 WHERE ID=" + to_string(i) + "!" + to_string(j) + "!" + to_string(l) + "\";";
                            ins += "(\"" + to_string(i) + "!" + to_string(j) + "!" + to_string(l) + "\", 7.3),";//last one will crash because of , ???
                        else
                        {
                            ins.back() = ';';
                            //shit = i;
                            //if (((shit + i) % 100) > 98) {//что то придумать
                            try {
                                SQLite::Transaction transaction(db1);
                                db1.exec(ins);
                                transaction.commit();
                            }
                            catch (std::exception& e)
                            {
                                std::cout << "exception: " << e.what() << std::endl;
                            }
                            ins.clear();
                            ins = "INSERT or IGNORE INTO TEXT(ID, CHISLO) VALUES (\"" + to_string(i) + "!" + to_string(j) + "!" + to_string(l) + "\", 7.3),";
                        }
                    }
            ins.back() = ';';
            SQLite::Transaction transaction(db1);
            db1.exec(ins);
            transaction.commit();
            ins.clear();

            int y = clock();
            auto time = y - x;

            fs::permissions(textname, fs::perms::owner_all | fs::perms::group_all, fs::perm_options::add); //mostly likely unneeded
        }
        if (tail / number_of_terms_in_one_db > 0.33)
        {
            sqlite3* db;
            string textname = "text" + to_string(text_counter) + " terms " + to_string(count_of_collocations-tail) + "-" + to_string(count_of_collocations) + ".db";
            sqlite3_open(textname.c_str(), &db);
            //text_counter++;

            this->_filenames.insert(make_pair(textname, make_pair(count_of_collocations - tail, count_of_collocations)));

            string sql = "CREATE TABLE TEXT("  \
                "ID STRING PRIMARY KEY NOT NULL," \
                "CHISLO REAL NOT NULL);";

            int rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

            sqlite3_close(db);

            SQLite::Database db1(textname, SQLite::OPEN_READWRITE);
            string ins = "INSERT or IGNORE INTO TEXT(ID, CHISLO) VALUES ";
            for (int i = number_of_terms_in_one_db * number_of_full_db; i < count_of_collocations; ++i)
                for (int j = 0; j < count_of_collocations; ++j)
                    for (int l = 0; l < k; ++l)
                    {
                        if (ins.size() < 999950)
                            ins += "(\"" + to_string(i) + "!" + to_string(j) + "!" + to_string(l) + "\", 7.3),";//last one will crash because of , ???
                        else
                        {
                            ins.back() = ';';
                            //shit = i;
                            //if (((shit + i) % 100) > 98) {//что то придумать
                            SQLite::Transaction transaction(db1);
                            db1.exec(ins);
                            transaction.commit();
                            ins.clear();
                            ins = "INSERT or IGNORE INTO TEXT(ID, CHISLO) VALUES ";
                        }
                    }
            ins.back() = ';';
            SQLite::Transaction transaction(db1);
            db1.exec(ins);
            transaction.commit();
            ins.clear();
        }


        text_counter++;
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


    // ”наследовано через container_class_interface
    virtual void increment(int first_dimension, int second_dimension, int third_dimension) override;
    virtual void decrement(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension) override;
    virtual now_type set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem) override;
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

    // ”наследовано через container_class_interface
    virtual void operator/=(now_type _num) override;
    unordered_map<string, pair<int, int>> _filenames;
};

