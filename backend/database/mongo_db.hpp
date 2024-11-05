#ifndef MongoDB_hpp
#define MongoDB_hpp

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <map>
#include <string>
#include <set>
#include <queue>

class MongoDB 
{
public:
    MongoDB();
    void insert_data(const std::string& url, const std::map<std::string, int>& dic); // Insert the word occurrences for one URL
    std::map<std::string, int> get_urls(std::string word);
    std::set<std::string> get_visited_urls();
    std::queue<std::string> get_unvisited_queue(int& urls_needed);
    void put_visited_urls(std::set<std::string> & urls);
    void put_queue_urls(std::queue<std::string>  & queue);

private:
    std::string db_name;
    std::string collection_words_name;
    std::string visited_urls_name;
    std::string unvisited_urls_queue_name;


    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection words_collection;    
    mongocxx::collection visited_urls_collection; 
    mongocxx::collection urls_queue_collection;  //urls not visited yet   

    static void init_driver();  // Static method for initializing the MongoDB driver
    void insert_word(const std::string& url, const std::string& word, int count);
};

#endif
