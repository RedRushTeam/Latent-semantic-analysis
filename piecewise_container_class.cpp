#pragma once 
#include "piecewise_container_class.h"

bool piecewise_container_class::is_slice_loaded(int first_dimension)
{
	if (number_of_downloaded_slice.find(first_dimension) != number_of_downloaded_slice.end())
		return true;
	return false;
}

void piecewise_container_class::load_slice(int first_dimension)
{
	if (is_slice_loaded(first_dimension))
		return;

	SQLite::Database db(this->_filename, SQLite::OPEN_READWRITE);
	for (int j = 0; j < this->get_count_of_collocations(); ++j)
		for (int l = 0; l < this->get_k(); ++l)
		{
			three_coordinate_structure _key{ first_dimension, j, l };
			downloaded_map.insert(make_pair(_key, this->get_count_of_concret_collocation(first_dimension, j, l)));
			
		}
	number_of_downloaded_slice.insert(first_dimension);
}

void piecewise_container_class::clear_map()
{
	for (auto i : number_of_downloaded_slice)
		for (int j = 0; j < this->get_count_of_collocations(); ++j)
			for (int l = 0; l < this->get_k(); ++l)
			{
				three_coordinate_structure _key{ i, j, l };
				//need to call db correct;
			}

}

void piecewise_container_class::increment(int first_dimension, int second_dimension, int third_dimension)
{
	if (is_slice_loaded(first_dimension))
	{ 
		three_coordinate_structure _key{ first_dimension, second_dimension, third_dimension };
		++(downloaded_map.find(_key)->second);
	}
	else {
		auto tmp = this->get_count_of_concret_collocation(first_dimension, second_dimension, third_dimension);
		++tmp;

		boost::regex rgx("[,]");
		string key = "\"" + to_string(first_dimension) + "!" + to_string(second_dimension) + "!" + to_string(third_dimension) + "\"";
		string sql = "UPDATE TEXT set CHISLO=" + boost::regex_replace(boost::lexical_cast<string>(tmp), rgx, ".") + " WHERE ID=" + key + ";";  //запятая вместо точки

		SQLite::Database db(this->_filename, SQLite::OPEN_READWRITE);
		SQLite::Transaction transaction(db);
		db.exec(sql);
		transaction.commit();
	}
}

void piecewise_container_class::decrement(int first_dimension, int second_dimension, int third_dimension)
{
	if (is_slice_loaded(first_dimension))
	{
		three_coordinate_structure _key{ first_dimension, second_dimension, third_dimension };
		--(downloaded_map.find(_key)->second);
	}
	else {
		auto tmp = this->get_count_of_concret_collocation(first_dimension, second_dimension, third_dimension);
		--tmp;

		boost::regex rgx("[,]");
		string key = "\"" + to_string(first_dimension) + "!" + to_string(second_dimension) + "!" + to_string(third_dimension) + "\"";
		string sql = "UPDATE TEXT set CHISLO=" + boost::regex_replace(boost::lexical_cast<string>(tmp), rgx, ".") + " WHERE ID=" + key + ";";  //запятая вместо точки

		SQLite::Database db(this->_filename, SQLite::OPEN_READWRITE);
		SQLite::Transaction transaction(db);
		db.exec(sql);
		transaction.commit();
	}
}

now_type piecewise_container_class::get_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension)
{
	if (is_slice_loaded(first_dimension))
	{
		three_coordinate_structure _key{ first_dimension, second_dimension, third_dimension };
		return downloaded_map.find(_key)->second;
	}
	else {
		string key = "\"" + to_string(first_dimension) + "!" + to_string(second_dimension) + "!" + to_string(third_dimension) + "\"";
		SQLite::Database db(this->_filename);
		SQLite::Statement query(db, "SELECT * FROM TEXT WHERE ID=" + key + ";");

		now_type ret_value = -1;

		while (query.executeStep())
		{
			auto value = query.getColumn(1).getDouble();
			ret_value = static_cast <now_type> (value);
		}

		return ret_value;
	}
}

shared_ptr<container_class_interface> piecewise_container_class::pow_all(int stepen)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::sqrt_all()
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator+(shared_ptr<container_class_interface> summed_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator+(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator-(shared_ptr<container_class_interface> summed_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator-(now_type _num)
{
	return shared_ptr<container_class_interface>();
}
void piecewise_container_class::operator+=(shared_ptr<container_class_interface> summed_class)
{
}

void piecewise_container_class::operator+=(now_type _num)
{
}

void piecewise_container_class::operator-=(shared_ptr<container_class_interface> summed_class)
{
}

void piecewise_container_class::operator-=(now_type _koef)
{
}

bool piecewise_container_class::operator==(shared_ptr<container_class_interface> compared_class)
{
	return false;
}

bool piecewise_container_class::operator!=(shared_ptr<container_class_interface> compared_class)
{
	return false;
}

shared_ptr<container_class_interface> piecewise_container_class::operator*(shared_ptr<container_class_interface> multipliable_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator*(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator/(shared_ptr<container_class_interface> dividor_class)
{
	return shared_ptr<container_class_interface>();
}

shared_ptr<container_class_interface> piecewise_container_class::operator/(now_type _koef)
{
	return shared_ptr<container_class_interface>();
}

void piecewise_container_class::operator/=(now_type _num)
{
}

now_type piecewise_container_class::set_count_of_concret_collocation(int first_dimension, int second_dimension, int third_dimension, now_type perem)
{
	return now_type();
}
