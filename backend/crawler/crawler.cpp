#include <queue>
#include <string>
#include <set>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <sstream> 
#include "parser.cpp"
#include "../database/mongo_db.hpp"
#include <nlohmann/json.hpp>
#include <fstream>   




#ifndef Crawler_cpp
#define Crawler_cpp


class Crawler
{
public:
    Crawler();
    ~Crawler();
    void Crawling();
    std::queue<std::string> m_url_queue;
    std::set<std::string> m_visited_urls_set;

private:
    int max_urls;
    std::string get_html(std::string url);
    void create_db();
    void update_set();

};

Crawler::Crawler()
{
    std::ifstream config_file("config.json");
    nlohmann::json config;
    config_file >> config;

    // get the URL from config file
    std::string url = config["start_url"];
    this->m_url_queue.push(url); 

    // get max urls for craeling from config file
    this->max_urls = config["max_urls"];
}


Crawler::~Crawler()
{
}


std::string Crawler::get_html(std::string url){
    try
	{
        curlpp::Easy request;
        std::stringstream response;
        request.setOpt<curlpp::options::Url>(url);
        request.setOpt(new curlpp::options::WriteStream(&response));
        
        request.perform();  // Perform the request
        return response.str();
        
    }
    catch(curlpp::RuntimeError & e)
	{
		std::cout << e.what() << std::endl;
	}

	catch(curlpp::LogicError & e)
	{
		std::cout << e.what() << std::endl;
	}
    return "";
}


void Crawler::Crawling(){
    Parser parser(&this->m_url_queue); //create parser object
    MongoDB my_db;   //create db object
 
    int i = 0;          
    while (i != this->max_urls)
    {
        i++;
        std::string url = this->m_url_queue.front(); //get a url from the queue
        this->m_url_queue.pop();
        // Check if the URL has already been visited
        if (this->m_visited_urls_set.find(url) == this->m_visited_urls_set.end()) {
            this->m_visited_urls_set.insert(url); // insert the url to the set of visited urls
            std::string html = get_html(url); // send the url to fanc that will return the html
            parser.extract_links(html);      // extract the urls from the curent html into the queue
            std::string str = parser.extract_words(html); //extract the text from the html
            auto map = parser.count_words(str);// count the words
            my_db.insert_data(url,map); //use the mongo class for inserting the data to db

        }
    }
}



#endif


int main(){ 
    Crawler crawler;
    crawler.Crawling();
    
    return 0;
}

