#pragma once

//lib defines
#define BOOST_IOSTREAMS_NO_LIB
#define _CRT_SECURE_NO_WARNINGS
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#define BOOST_HAS_THREADS
//#define _ITERATOR_DEBUG_LEVEL 0
#define SQLITE_ENABLE_COLUMN_METADATA
#define SQLITE_OPEN_READWRITE

//C++ includes
#include <iostream>
#include <memory>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <math.h>
#include <ctime>
#include <thread>
#include <filesystem>
#include <random>
#include <regex>

//Multithreading include
#include "omp.h"

//lemmatizator include
#include <lemmatizator_engine.h>

//windows.h include
#include <windows.h>

//Eigen lib include
#include <Eigen>

//boost includes
#include <boost/any.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
/*#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/device/file.hpp> */

//SQLite include
#include <sqlite3.h> 

//SQLiteCpp SQL wrapper include
#include <SQLiteCpp/SQLiteCpp.h>

//namespaces
namespace fs = std::filesystem;
using namespace std;

//SVD namespace
using namespace Eigen;

//usings
using now_type = float;

//my defines
#define COLLOC_DIST 3
#define STOP_WORD 0 

#define LEMADR "C:\\RGD\\RussianGrammaticalDictionary\\bin-windows64\\lemmatizer.db"
#define TEXTS_PATH "A:\\rasp_puhl"
//#define TEXTS_PATH "G:\\rasp_puhl"