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

void parser::print_all_words(string text)
{
	cout << endl << text << endl;
}

shared_ptr<list<string>> parser::delete_trash()
{
	std::ifstream _input(_filename);
	int count = 1;
	int helper = 1;
	int size = 0;

	boost::regex no_letters("[^а-€ј-я ]+");
	boost::regex spaces("( {2,})+");


	string replacement = " ";

	std::stringstream ss;
	std::string untext;
	if (_input.is_open()) {
		ss << _input.rdbuf();
		untext = ss.str();
		_input.close();
	}
	else
		cout << "it doesn't work";

	string temp = boost::regex_replace(untext, no_letters, replacement);
	string text = boost::regex_replace(temp, spaces, replacement);


	transform(text.begin(), text.end(), text.begin(), ::tolower);

	if (text[0] == ' ')
		text.erase(0, 1);
	text.pop_back();
	if (text[text.size() - 1] != ' ')
		text.push_back(' ');

	string substring_word;
	shared_ptr<list<string>> terms = make_shared<list<string>>();


	char* c_text = new char[text.size()];
	strcpy(c_text, text.c_str());
	char* pch = strtok(c_text, " ");
	while (pch != NULL)                         // пока есть лексемы
	{
		terms->push_back((string)pch);
		pch = strtok(NULL, " ");
	}

	delete[] c_text;
	delete[] pch;

	return terms;
}