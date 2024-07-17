#include <iostream>
#include <regex>
#include<queue>
#include "gumbo.h"


#ifndef Parser_cpp
#define Parser_cpp


class Parser
{
public:
    Parser(std::queue<std::string>* url_queue);
    ~Parser();
    void extract_urls(std::string* str); 
	void extract_words(std::string str); //TODO pass pointer insted value

private:
    std::queue<std::string>* m_url_queue;
	void cut_link(std::string* str);
    std::string cleantext(GumboNode* node); //gumbo func for filter the text from the html


};
#endif