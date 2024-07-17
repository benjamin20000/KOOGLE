#include <iostream>
#include<queue>
#include "gumbo.h"


#ifndef Parser_cpp
#define Parser_cpp


class Parser
{
public:
    Parser(std::queue<std::string>* url_queue);
    ~Parser();
    std::string extract_words(std::string html);
    void extract_links(std::string html);
private:
    std::queue<std::string>* m_url_queue;
    std::string cleantext(GumboNode* node); //gumbo func for filter the text from the html
    void search_for_links(GumboNode* node); //gumbo func for extract the links from the html



};
#endif