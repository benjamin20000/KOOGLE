#include "parser.h"
#include <sstream> 

Parser::Parser(std::queue<std::string>* url_queue)
{
	m_url_queue = url_queue;
}


Parser::~Parser()
{
}

std::string Parser::extract_words(std::string html){
	GumboOutput* output = gumbo_parse(html.c_str());
	std::string text = cleantext(output->root);
	return text;
}

void Parser::extract_links(std::string html){
	GumboOutput* output = gumbo_parse(html.c_str());
    search_for_links(output->root);
}


//gumbo func for filtering the text from the html
std::string Parser::cleantext(GumboNode* node) {
  	if (node->type == GUMBO_NODE_TEXT) {
    	return std::string(node->v.text.text);
  	}
	else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) 
			{
    			std::string contents = "";
    			GumboVector* children = &node->v.element.children;
    			for (unsigned int i = 0; i < children->length; ++i) {
      				const std::string text = cleantext((GumboNode*) children->data[i]);
      				if (i != 0 && !text.empty()) {
        				contents.append(" ");
      				}
      				contents.append(text);
    			}
    			return contents;
  		} 		
		else {
    	return "";
  	}
}


//gumbo func for extract the links from html
void Parser::search_for_links(GumboNode* node) {
	if (node->type != GUMBO_NODE_ELEMENT){
    	return;
  	}
  	GumboAttribute* href;
  	if (node->v.element.tag == GUMBO_TAG_A &&
    	(href = gumbo_get_attribute(&node->v.element.attributes, "href")))
	{
   		this->m_url_queue->push(href->value); //TODO make sure its return only working urls
  	}
  	GumboVector* children = &node->v.element.children;
  	for (unsigned int i = 0; i < children->length; ++i){
    	search_for_links(static_cast<GumboNode*>(children->data[i]));
  	}
}


std::map<std::string, int>  Parser::count_words(std::string str) {
	std::map<std::string,int> words_dic; //declare a dic for words
	std::stringstream ss(str); 
    std::string word;

    // access each word of the string 
    // and enter it to dic
    while (ss >> word) { 
        if(!words_dic.count(word)){
			words_dic.insert({word, 1});
		} 
		else{
			++words_dic[word]; //add 1 to the count of the word
		}
    } 
	return words_dic;
}
