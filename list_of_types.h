#pragma once

//lib defines
#define BOOST_IOSTREAMS_NO_LIB
#define _CRT_SECURE_NO_WARNINGS
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#define BOOST_HAS_THREADS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define DATABASE ":memory:"

//C++ includes
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <unordered_set>
#include <math.h>
#include <ctime>
#include <filesystem>
#include <random>


//custom maps include
#include <robin_map.h>
#include <robin_set.h>

//Multithreading include
#include "omp.h"

//lemmatizator include
#include <lemmatizator_engine.h>

#ifdef _WIN32
//windows.h include
#include <windows.h>
#elif __linux__
// special linux includes here
#else
// special code for other OS
#endif

//LAPACKE for SVD
#include "mkl_lapacke.h"


//Eigen lib include
#include <Eigen>

//boost includes
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <boost/unordered_map.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

//SQLite include
//#include <sqlite3.h> 

//SQLiteCpp SQL wrapper include
//#include <SQLiteCpp/SQLiteCpp.h>

//RE2 include
#include <re2.h>

//7zip Wrapper include
#include "bitextractor.hpp"
#include "bitcompressor.hpp"

//libmdbx include
#include "mdbx.h++"

//libmdbx namespace
//using namespace mdbx;	(use mdbx::)!!!

//c++ namespaces
namespace fs = std::filesystem;
using namespace std;
//using namespace boost::numeric::ublas;

//SVD namespace
using namespace Eigen;

//7z namespace
using namespace bit7z;

//usings
using now_type = float;
//using now_type = half;

//my defines
constexpr auto SIZE_OF_PIECE = 500000;
constexpr auto COLLOC_DIST = 3;
constexpr auto STOP_WORD = 0 ;
constexpr auto CUTOFF = 6;
constexpr auto CUTOFF_FR_IN_TEXTS = 6;
constexpr auto DELETE_THRESHOLD = 0.065;
constexpr auto KOEF_FOR_COLLOC_COS_DELETE = 0.065;
constexpr auto SVD_PIECE = 40000;
constexpr auto BIT_COMPRESSION_LEVEL = BitCompressionLevel::FASTEST;
constexpr int FLAGS = LEME_FASTEST;

constexpr auto LEMADR = "C:\\RGD\\RussianGrammaticalDictionary\\bin-windows64\\lemmatizer.db";
//constexpr auto TEXTS_PATH = "A:\\rasp_puhl1";
constexpr auto TEXTS_PATH = "G:\\rasp_puhl";
//constexpr auto TEXTS_PATH = "C:\\txts";
constexpr auto DB_PATH = "C:\\databases";

//my structs
struct word_and_number_of_appearances_structure {
	string word;
	int number_of_appearances_of_this_word;
	int number_of_texts_in_which_term_occurs;
};

struct three_coordinate_structure {
	int first_coord;
	int second_coord;
	short k;
};

//my std hash
namespace std
{
	template<> struct hash<word_and_number_of_appearances_structure>
	{
		typedef word_and_number_of_appearances_structure argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<string>{}(s.word));
			return seed;
		}
	};
}

namespace std
{
	template<> struct hash<three_coordinate_structure>
	{
		typedef three_coordinate_structure argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<int>{}(s.first_coord));
			boost::hash_combine(seed, std::hash<int>{}(s.second_coord));
			boost::hash_combine(seed, std::hash<short>{}(s.k));
			return seed;
		}
	};
}

namespace std
{
	template<> struct hash<pair<int, int>>
	{
		typedef pair<int, int> argument_type;
		typedef std::size_t result_type;
		result_type operator()(argument_type const& s) const noexcept
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, std::hash<int>{}(s.first));
			boost::hash_combine(seed, std::hash<int>{}(s.second));
			return seed;
		}
	};
}

//my operators for trees
inline bool operator== (three_coordinate_structure lefty, three_coordinate_structure righty)
{
	if (lefty.first_coord == righty.first_coord && (lefty.second_coord == righty.second_coord) && (lefty.k == righty.k))
		return true;
	return false;
};

inline bool operator== (word_and_number_of_appearances_structure lefty, word_and_number_of_appearances_structure righty)
{
	if (lefty.word == righty.word)
		return true;
	return false;
};

//methods for boost up robin trees
struct hash_word_and_number_of_appearances_structure {
	std::size_t operator()(const word_and_number_of_appearances_structure& e) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, std::hash<string>{}(e.word));
		return seed;
	}

	std::size_t operator()(int id) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, std::hash<int>{}(id));
		return seed;
	}
};

struct equal_word_and_number_of_appearances_structure {
	using is_transparent = void;

	bool operator()(const word_and_number_of_appearances_structure& e1, const word_and_number_of_appearances_structure& e2) const {
		return e1.word == e2.word;
	}
};