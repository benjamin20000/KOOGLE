#include <queue>
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include "parser.cpp"

#ifndef Crawler_cpp
#define Crawler_cpp


class Crawler
{
public:
    Crawler();
    ~Crawler();
    void Crawling();
    std::queue<std::string> m_url_queue;

private:
    std::string get_html(std::string url);

};

Crawler::Crawler()
{
    // TODO put this link to config file
    std::string url = "https://harrypotter.fandom.com/wiki/Main_Page";
    this->m_url_queue.push(url);
    
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
    Parser parser(&this->m_url_queue);
    std::string url = this->m_url_queue.front();
    this->m_url_queue.pop();
    std::string html = get_html(url);
    if(html != "")
    parser.extract_urls(html);
    
    // while (!m_url_queue.empty())
    // {
    //     /* continue to crawl */
    //     get_html();
    // }
    
}

#endif

int main(){
    int m = 0;
    Crawler crawler;
    crawler.Crawling();
    std::cout << crawler.m_url_queue.size();
    while(m<30){
        m++;
        std::cout << crawler.m_url_queue.front()<<'\n';
        crawler.m_url_queue.pop();
    }
    
    return 0;
}