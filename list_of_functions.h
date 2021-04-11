#pragma once
#include "list_of_types.h"

class list_of_functions
{
public:
	static void print_info_about_sysyem() {
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		cout << "This system have " << statex.ullTotalPhys / 1024 << " KB of physical memory." << endl << endl;
		cout << "This system have " << statex.ullTotalPageFile / 1024 << " KB of paging file." << endl;
	}
};
