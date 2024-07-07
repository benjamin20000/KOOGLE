#include <queue>


class Crawler
{
public:
    Crawler(/* args */);
    ~Crawler();
    int main();

private:
    std::queue<int> url_queue;
};

Crawler::Crawler()
{
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

 
