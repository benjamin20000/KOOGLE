#ifndef MongoDB_hpp
#define MongoDB_hpp

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <map>
#include <string>

class MongoDB 
{
public:
    MongoDB();
    void insert_data(const std::string& url, const std::map<std::string, int>& dic); // Insert the word occurrences for one URL
    std::map<std::string, int> get_urls(std::string word);

private:
    std::string db_name;
    std::string collection_name;

    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection collection;    

    static void init_driver();  // Static method for initializing the MongoDB driver
    void insert_word(const std::string& url, const std::string& word, int count);
};

#endif // MongoDB_hpp
