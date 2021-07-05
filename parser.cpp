#include "parser.h"

shared_ptr<list<string>> parser::parse() {
	auto terms = this->delete_trash();
	for (auto& word : *terms)
		if (parser::stop_words.find(word) != parser::stop_words.end())
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
	ifstream _input(this->_filename);

	std::stringstream ss;
	std::string untext;
	if (_input.is_open()) {
		ss << _input.rdbuf();
		untext = ss.str();
		_input.close();
	}
	else
		cout << "it doesn't work";

	std::string wrapped_pattern_for_symbols = "[^Р-пр-џ ]+";
	RE2::Options opt_for_symbols;
	opt_for_symbols.set_log_errors(false);
	opt_for_symbols.set_case_sensitive(false);
	opt_for_symbols.set_encoding(re2::RE2::Options::Encoding::EncodingLatin1);
	RE2 re2_for_symbols(wrapped_pattern_for_symbols, opt_for_symbols);

	std::string wrapped_pattern_for_spaces = "( {2,})+";
	RE2::Options opt_for_spaces;
	opt_for_spaces.set_log_errors(false);
	opt_for_spaces.set_case_sensitive(false);
	opt_for_spaces.set_encoding(re2::RE2::Options::Encoding::EncodingLatin1);
	RE2 re2_for_spaces(wrapped_pattern_for_spaces, opt_for_spaces);

	RE2::GlobalReplace(&untext, re2_for_symbols, " ");
	RE2::GlobalReplace(&untext, re2_for_spaces, " ");

	if (untext[0] == ' ')
		untext.erase(0, 1);
	untext.pop_back();
	if (untext[untext.size() - 1] != ' ')
		untext.push_back(' ');

	shared_ptr<list<string>> terms = make_shared<list<string>>();

	boost::algorithm::split(*terms, untext, boost::is_any_of(" "));

	return terms;
}