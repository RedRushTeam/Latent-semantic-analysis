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
}

void list_of_functions::test_of_libmdbx()
{
	int rc;
	MDBX_env* env = NULL;
	MDBX_txn* txn = NULL;
	MDBX_cursor* cursor = NULL;
	char sval[32];
}

void list_of_functions::test_of_libmdbx(int kolichestvo_zapisey, bool random_number)
{
	auto s = clock();
	/*НЕОБХОДИМЫЕ ПЕРЕМЕННЫЕ*/
	int rc;

	MDBX_env* env = NULL;
	MDBX_dbi dbi = 0;
	MDBX_val index, number;
	MDBX_txn* txn = NULL;
	MDBX_cursor* cursor = NULL;

	int _commit = 10000;
	int found = 0;
	int i, j, l;
	string _index;
	float _value;
	int counter = 0;
	string _for_size = "99999!99999!4";

	std::mt19937 gen(time(NULL));
	std::uniform_real_distribution<> uid(0, 1);
	std::uniform_int_distribution<> uid_key(0, 100000);
	std::uniform_int_distribution<> uid_k(0, 3);

	/*РАСЧЁТ ИСПОЛЬЗУЕМЫХ ЗНАЧЕНИЙ*/
	auto start_size = (sizeof(_for_size) + sizeof(float)) * kolichestvo_zapisey; //(длина ключа + длина числа) * на количество записей
	string textnameparam;
	random_number ? textnameparam = "randoms" : textnameparam = "concrets";
	string textname = "./" + to_string(kolichestvo_zapisey) + " zapisey by " + textnameparam; // ВМЕСТО ./ МОЖНО ПОДСТАВИТЬ ЛЮБОЙ АДРЕС

	/*СОЗДАНИЕ ФАЙЛА БД*/
	rc = mdbx_env_create(&env);
	if (rc != MDBX_SUCCESS) {
		fprintf(stderr, "mdbx_env_create: (%d) %s\n", rc, mdbx_strerror(rc));
		goto bailout;
	}

	/*ВЫДЕЛЕНИЕ ПАМЯТИ*/
	//mdbx_env_set_mapsize(env, count_of_collocations * count_of_collocations * k); ///deprecated
	rc = mdbx_env_set_geometry(env, start_size, -1, start_size * 4, -1, -1, -1); //место для поиска скорости
	if (rc != MDBX_SUCCESS) {
		fprintf(stderr, "mdbx_env_open: (%d) %s\n", rc, mdbx_strerror(rc));
		goto bailout;
	}

	rc = mdbx_env_open(env, textname.c_str(),
		MDBX_NOSUBDIR | MDBX_COALESCE | MDBX_LIFORECLAIM, 0664); //0664 - what is it ?
	if (rc != MDBX_SUCCESS) {
		fprintf(stderr, "mdbx_env_open: (%d) %s\n", rc, mdbx_strerror(rc));
		goto bailout;
	}

	
	for (l = 0; l < kolichestvo_zapisey; ++l) {
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
		_index = to_string(uid_key(gen)) + "!" + to_string(uid_key(gen)) + "!" + to_string(uid_k(gen));
		//_index = "999!999!2";
		random_number ? _value = uid(gen) : _value = 0.0;
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
		if (counter == _commit) {
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

bailout:
	if (cursor)
		mdbx_cursor_close(cursor);
	if (txn)
		mdbx_txn_abort(txn);
	if (dbi)
		mdbx_dbi_close(env, dbi);
	if (env)
		mdbx_env_close(env);
	
	auto f = clock();
	cout << kolichestvo_zapisey << '\t' << textnameparam << '\t' << f - s << '\n';
	system("pause");
	if (fs::exists(textname))
		fs::remove(textname);
}
