#pragma once

//lib defines
//#define _ITERATOR_DEBUG_LEVEL 0
//#define SQLITE_ENABLE_COLUMN_METADATA
//#define SQLITE_OPEN_READWRITE
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
//#include <hopscotch_map.h>
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

//SVD namespace
using namespace Eigen;

//7z namespace
using namespace bit7z;

//usings
using now_type = float;

//my defines
constexpr auto SIZE_OF_PIECE = 10000;
constexpr auto COLLOC_DIST = 3;
constexpr auto STOP_WORD = 0 ;
constexpr auto CUTOFF = 6;
constexpr auto CUTOFF_FR_IN_TEXTS = 6;
constexpr auto DELETE_THRESHOLD = 0.065;
constexpr auto KOEF_FOR_COLLOC_COS_DELETE = 0.065;
constexpr auto BIT_COMPRESSION_LEVEL = BitCompressionLevel::FASTEST;
constexpr int FLAGS = LEME_FASTEST;

constexpr auto LEMADR = "C:\\RGD\\RussianGrammaticalDictionary\\bin-windows64\\lemmatizer.db";
//constexpr auto TEXTS_PATH = "A:\\rasp_puhl1";
constexpr auto TEXTS_PATH = "G:\\rasp_puhl";
//constexpr auto TEXTS_PATH = "C:\\txts";
constexpr auto DB_PATH = "C:\\databases";