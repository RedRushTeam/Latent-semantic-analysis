#include "parser.h"



shared_ptr<list<string>> parser::parse() {
	auto terms = this->delete_trash();
	for (auto& word : *terms)
		if (stop_words.find(word) != stop_words.end())
			word = "a";
	return terms;
}

void parser::set_filename(fs::path filename)
{
	this->_filename = filename;
}

fs::path parser::get_filename() const
{
	return this->_filename;
}

void parser::print_all_words(string text)
{
	cout << endl << text << endl;
}

shared_ptr<list<string>> parser::delete_trash()
{
	setlocale(LC_ALL, "Russian");
	std::ifstream _input(_filename, std::ios::binary);

	boost::regex no_letters("[^à-ÿÀ-ß ]+");
	boost::regex spaces("( {2,})+");
	std::ios::sync_with_stdio(false);

	string replacement = " ";

	boost::iostreams::mapped_file mmap(_filename.string());

	string untext(mmap.data(), mmap.size());

	/*std::stringstream ss;
	std::string untext;
	if (_input.is_open()) {
		ss << _input.rdbuf();
		untext = ss.str();
		_input.close();
	}
	else
		cout << "it doesn't work";*/


	re2::RE2::GlobalReplace(&untext, u8"[^à-ÿÀ-ß ]+", " ");
	re2::RE2::GlobalReplace(&untext, u8"( {2,})+", " ");
	//string temp = boost::regex_replace(untext, no_letters, replacement);
	//string text = boost::regex_replace(temp, spaces, replacement);

	transform(untext.begin(), untext.end(), untext.begin(), ::tolower);

	if (untext[0] == ' ')
		untext.erase(0, 1);
	untext.pop_back();
	if (untext[untext.size() - 1] != ' ')
		untext.push_back(' ');

	shared_ptr<list<string>> terms = make_shared<list<string>>();

	boost::algorithm::split(*terms, untext, boost::is_any_of(" "));

	return terms;
}