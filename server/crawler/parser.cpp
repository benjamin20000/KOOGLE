#include <iostream>
#include <regex>
#include<queue>

#ifndef Parser_cpp
#define Parser_cpp


class Parser
{
public:
    Parser(std::queue<std::string>* url_queue);
    ~Parser();
    void extract_urls(std::string str); //TODO pass a pointer of the string
private:
    std::queue<std::string>* m_url_queue;
	void cut_link(std::string* str);


};


Parser::Parser(std::queue<std::string>* url_queue)
{
	m_url_queue = url_queue;
}


Parser::~Parser()
{
}

//func for cut off the sub string "<a href="  
void Parser::cut_link(std::string* str)
{
	int index = str->find("=");
    std::string sub = str->substr(index+2);
	*str = sub;
}

void Parser::extract_urls(std::string str)
{

	// Regular Expression to extract URLs from the string
	std::string regex_str = "<a href=\"((?:https?|ftp|file):"
					"\\/\\/[a-zA-Z0-9+&@#\\/%?=~_|!:,.;]*"
					"[a-zA-Z0-9+&@#\\/%=~_|])";

	// Compile the Regular Expression
	std::regex r(regex_str, std::regex_constants::icase);

	// Find the match between string and the regular expression
	std::sregex_iterator m(str.begin(), str.end(), r);
	std::sregex_iterator m_end;

	// find and store all the URLs in the queue
	while (m != m_end) {
		std::string link = m->str();
		this->cut_link(&link); //cut the sub string "<a href=" from the link
		this->m_url_queue->push(link);
		m++;
	}
}
#endif
