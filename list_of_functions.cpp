#include "list_of_functions.h"

void list_of_functions::print_info_about_sysyem()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	cout << "This system have " << statex.ullTotalPhys / 1024 / 1024 << " MB of physical memory." << endl;
	cout << "This system have " << statex.ullTotalPageFile / 1024 / 1024 << " MB of paging file." << endl;

	if ((statex.ullTotalPhys / 1024 / 1024) < ((SIZE_OF_PIECE * SIZE_OF_PIECE * COLLOC_DIST * 4 * 8) / 1024 / 1024)) {
		cout << "You do not have enough RAM. Reduce SIZE_OF_PIECE!" << endl;
		exit(1);
	}
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

bool list_of_functions::compress_file_for_path(fs::path path_to_compress)	//сжимает конкретный файл, если пришла ссылка на файл, или все файлы в директории в свой архив, если пришел путь к директории
{
	if (!fs::exists(path_to_compress))
		return false;

	shared_ptr<std::vector<fs::path>> files_for_ar = make_shared<std::vector<fs::path>>();
	if (fs::is_directory(path_to_compress)) {	//это директория
		fs::recursive_directory_iterator begin(path_to_compress);
		fs::recursive_directory_iterator end;
		std::copy_if(begin, end, std::back_inserter(*files_for_ar), [](const fs::path& path) {	return fs::is_regular_file(path); });
	}
	else
		if (fs::is_regular_file(path_to_compress))
			files_for_ar->push_back(path_to_compress);

	if (files_for_ar->empty())	//нам передали пустю дирректорию/или путь не к архиву
		return false;


	Bit7zLibrary lib{ L"7z.dll" };
	BitCompressor compressor{ lib, BitFormat::SevenZip };
	compressor.setCompressionMethod(BitCompressionMethod::Lzma2);
	compressor.setCompressionLevel(BIT_COMPRESSION_LEVEL);

	std::vector<std::wstring> files;

	for (int i = 0; i < files_for_ar->size(); ++i) {
		files.push_back((*files_for_ar)[i].generic_wstring());
		compressor.compress(files, (*files_for_ar)[i].generic_wstring() + L".7z");
		files.clear();
	}

	return true;
}

bool list_of_functions::decompress_file_for_path(fs::path path_to_decompress)
{
	if (!fs::exists(path_to_decompress))
		return false;

	fs::path path_to_output;

	shared_ptr<std::vector<fs::path>> files_for_ar = make_shared<std::vector<fs::path>>();
	if (fs::is_directory(path_to_decompress)) {	//это директория
		fs::recursive_directory_iterator begin(path_to_decompress);
		fs::recursive_directory_iterator end;
		std::copy_if(begin, end, std::back_inserter(*files_for_ar), [](const fs::path& path) {	return fs::is_regular_file(path) && path.extension() == ".7z"; });
		path_to_output = path_to_decompress;
	}
	else
		if (path_to_decompress.extension() == ".7z" && fs::is_regular_file(path_to_decompress)) {
			path_to_output = path_to_decompress.parent_path();
			files_for_ar->push_back(path_to_decompress);
		}

	if (files_for_ar->empty())	//нам передали пустю дирректорию/или путь не к архиву
		return false;

	Bit7zLibrary lib{ L"7z.dll" };
	BitExtractor decompressor{ lib, BitFormat::SevenZip };


	for (int i = 0; i < files_for_ar->size(); ++i)
		decompressor.extract((*files_for_ar)[i].generic_wstring(), path_to_output.generic_wstring());

	return true;
}

bool list_of_functions::delete_file_for_path(fs::path path_to_delete)
{
	if (!fs::exists(path_to_delete))
		return false;
	
	if (fs::is_directory(path_to_delete)) {	//это директория
		fs::recursive_directory_iterator begin(path_to_delete);
		fs::recursive_directory_iterator end;
		shared_ptr<std::vector<fs::path>> files_for_delete = make_shared<std::vector<fs::path>>();
		std::copy_if(begin, end, std::back_inserter(*files_for_delete), [](const fs::path& path) {	return fs::is_regular_file(path); });

		for(auto obj : *files_for_delete)
			fs::remove(obj);
	}
	else
		fs::remove(path_to_delete);

	return true;
}