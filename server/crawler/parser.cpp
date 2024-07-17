#include "parser.h"


Parser::Parser(std::queue<std::string>* url_queue)
{
	m_url_queue = url_queue;
}


Parser::~Parser()
{
}

//gumbo func for filtering the text from the html
std::string Parser::cleantext(GumboNode* node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {
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
  } else {
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


//func for cut off the sub string "<a href="  
void Parser::cut_link(std::string* str)
{
	int index = str->find("=");
    std::string sub = str->substr(index+2);
	*str = sub;
}

void Parser::extract_urls(std::string* str)
{

	// Regular Expression to extract URLs from the string
	std::string regex_str = "<a href=\"((?:https?|ftp|file):"
					"\\/\\/[a-zA-Z0-9+&@#\\/%?=~_|!:,.;]*"
					"[a-zA-Z0-9+&@#\\/%=~_|])";

	// Compile the Regular Expression
	std::regex r(regex_str, std::regex_constants::icase);

	// Find the match between string and the regular expression
	std::sregex_iterator m(str->begin(), str->end(), r);
	std::sregex_iterator m_end;

	// find and store all the URLs in the queue
	while (m != m_end) {	
		std::string link = m->str();
		this->cut_link(&link); //cut the sub string "<a href=" from the link
		this->m_url_queue->push(link);
		m++;
	}
}

void Parser::extract_words(std::string html)
{
	// // html = "aba aba" ;
	// std::regex r(R"([^\W_]+(?:['_-][^\W_]+)*)");
	// for(std::sregex_iterator i = std::sregex_iterator(html.begin(), html.end(), r);
    //                      i != std::sregex_iterator();
    //                      ++i)
// {
//     std::smatch m = *i;
//     std::cout << m.str() << '\n';
// }
    std::cout << html << '\n';


}

