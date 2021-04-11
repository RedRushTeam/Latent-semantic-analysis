#pragma once

//lib defines
#define H5_BUILT_AS_DYNAMIC_LIB
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#define BOOST_HAS_THREADS

//C++ includes
#include <iostream>
#include <memory>
#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <lemmatizator_engine.h>
#include <windows.h>
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
#include <regex>

//Eigen lib include
#include <Eigen>

//boost includes
#include <boost/any.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

//HDF5 includes
#include "H5cpp.h"

//namespaces
namespace fs = std::filesystem;
using namespace std;

//SVD namespace
using namespace Eigen;

//HDF namespace
using namespace H5;

//usings
using now_type = float;

//my defines
#define GAP 3
#define STOP_WORD 0 

#define LEMADR "C:\\RGD\\RussianGrammaticalDictionary\\bin-windows64\\lemmatizer.db"
#define TEXTS_PATH "C:\\txts"