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
    MongoDB my_db;
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
   
    this->max_urls = config["max_urls"];    // get max urls for crawling from config file
    int needed_urls = this->max_urls * 10;   // multiply be 10 for case some of the urls not working
    this->m_url_queue = this->my_db.get_unvisited_queue(needed_urls); //get the urls
    std::cout<<"m_url_queue.size:  "<< this->m_url_queue.size();
    if(this->m_url_queue.size()==0){
        //get the URL from config file
        std::string url = config["start_url"];
        this->m_url_queue.push(url); 
    }
  
    
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
    // MongoDB my_db;   //create db object
    this->m_visited_urls_set = this->my_db.get_visited_urls();    
    int crawl_depth = 0;     
    while (this->max_urls != crawl_depth && this->m_url_queue.size() != 0)
    {
        std::string url = this->m_url_queue.front(); //get a url from the queue
        this->m_url_queue.pop();
        // if the URL has already been visited:
        if (this->m_visited_urls_set.find(url) == this->m_visited_urls_set.end()) {
            crawl_depth++;
            std::cout<<"crawl_depth:  " << crawl_depth<<"\n";
            std::cout<<"new url:  "<<url<<"\n";
            this->m_visited_urls_set.insert(url); // insert the url to the set of visited urls
            std::string html = get_html(url); // send the url to fanc that will return the html
            parser.extract_links(html);      // extract the urls from the curent html into the queue
            std::string str = parser.extract_words(html); //extract the text from the html
            auto map = parser.count_words(str);// count the words
            my_db.insert_data(url,map); //use the mongo class for inserting the data to db
            this-> m_visited_urls_set.insert(url);
        }
    }
    this->my_db.put_visited_urls(this-> m_visited_urls_set);
    // std::cout<<"queue size: "<<this-> m_url_queue.size();
    this->my_db.put_queue_urls(this-> m_url_queue);
 }



#endif


int main(){ 
    Crawler crawler;
    crawler.Crawling();
    
    return 0;
}

