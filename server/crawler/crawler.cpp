#include <queue>
#include <string>

#include <fstream>



class Crawler
{
public:
    Crawler(/* args */);
    ~Crawler();
    int main();

private:
    std::queue<std::string> url_queue;
};

Crawler::Crawler()
{
    // TODO put this link to config file
    std::string url = "https://harrypotter.fandom.com/wiki/Main_Page";
    this->url_queue.push(url);
    
}

Crawler::~Crawler()
{
}

int Crawler::main(){
    while (!url_queue.empty())
    {
        /* continue to crawl */
    }
    


}

 
