#include "list_of_functions.h"

void list_of_functions::print_info_about_sysyem()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "This system have " << statex.ullTotalPhys / 1024 / 1024 << " MB of physical memory." << endl << endl;
	cout << "This system have " << statex.ullTotalPageFile / 1024 / 1024 << " MB of paging file." << endl;
}

shared_ptr<vector<fs::path>> list_of_functions::get_input_texts()
{
	auto input_path = TEXTS_PATH/*fs::current_path()/"inputfiles"*/;
	fs::recursive_directory_iterator begin(input_path);
	fs::recursive_directory_iterator end;
	shared_ptr<std::vector<fs::path>> txtFiles = make_shared<std::vector<fs::path>>();
	std::copy_if(begin, end, std::back_inserter(*txtFiles), [](const fs::path& path) {
		return fs::is_regular_file(path) && (path.extension() == ".txt" || path.extension() == ".TXT"); });
	return txtFiles;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

void list_of_functions::test_of_sqlite()
{
	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	const char* sql;
	const char* data = "Callback function called";

	rc = sqlite3_open("test.db", &db);

	if (rc)
		cout << "Can't open database: %s\n" << sqlite3_errmsg(db) << endl;
	else 
		cout << "Opened database successfully\n" << endl;

	/* Create SQL statement */
	sql = "CREATE TABLE SHIT("  \
		"ID INT PRIMARY KEY     NOT NULL," \
		"CHISLO           REAL    NOT NULL );";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Table created successfully\n");
	}

	std::mt19937 gen(time(NULL));
	std::uniform_real_distribution<float> uid1(0.0, 1.0);

	/* Create SQL statement */
	for (int i = 0; i < 100000; ++i) {
		string shit = "INSERT INTO SHIT (ID,CHISLO) "  \
			"VALUES (" + to_string(i) + ", " + to_string(uid1(gen)) + " ); ";

		sql = shit.c_str();

		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	}

	/* Create SQL statement */
	sql = "SELECT * from SHIT";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}

	/* Create merged SQL statement */
	sql = "UPDATE SHIT set CHISLO = 25000.00 where ID=1; " \
		"SELECT * from SHIT";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}

	/* Create merged SQL statement */
	sql = "DELETE from SHIT where ID=1; " \
		"SELECT * from SHIT";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		fprintf(stdout, "Operation done successfully\n");
	}

	sqlite3_close(db);
}

void list_of_functions::test_of_bit7z(std::string file)
{
	
	Bit7zLibrary lib{ L"7z.dll" };
	BitCompressor compressor{ lib, BitFormat::Tar };

	std::vector< std::wstring > files = { L"G:/TEST_db/text0.db"};

	compressor.compress(files, L"G:/output_archive1.zip"); //creating a simple zip archive

	//Bit7zLibrary lib{ L"7za.dll" };
	//BitExtractor extractor{ lib, BitFormat::Zip };

	//extractor.extract(L"G:/output_archive1.zip", L"G:/1"); //extracting a simple archive
	//extractor.extract(L"G:/output_archive1.zip"); //extracting a simple archive

	//extractor.extract(L"path/to/another/archive.7z", L"out/dir/");
}
