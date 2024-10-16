#include "mongo_db.hpp"
#include <nlohmann/json.hpp>
#include <fstream>   


void MongoDB::init_driver() {
    static mongocxx::instance inst{}; // Ensure this is initialized only once
}

MongoDB::MongoDB() {
    init_driver();

    // Check the status of the MongoDB service
    std::cout << "Checking MongoDB service status..." << std::endl;
    int status_result = std::system("sudo systemctl status mongod > /dev/null 2>&1");

    if (status_result != 0) {
        std::cout << "MongoDB service is not running. Attempting to start it..." << std::endl;
        int start_result = std::system("sudo systemctl start mongod");

        if (start_result != 0) {
            throw std::runtime_error("Failed to start MongoDB service.");
        } else {
            std::cout << "MongoDB service started successfully." << std::endl;
        }
    } else {
        std::cout << "MongoDB service is running." << std::endl;
    }
    //take the database and collectoin names from a config file

    std::ifstream config_file("config.json");
    nlohmann::json config;
    config_file >> config;

    db_name = config["database_name"];
    collection_name =  config["collection_name"];

    this->client = mongocxx::client{mongocxx::uri{}};
    this->db = client[db_name];
    this->collection = db[collection_name];
}


void MongoDB::insert_word(const std::string& url, const std::string& word, int count) {
    // Create a filter to find the document with the specified word
    auto filter = bsoncxx::builder::stream::document{}
        << "word" << word
        << bsoncxx::builder::stream::finalize;

    // Create the update operation
    auto update = bsoncxx::builder::stream::document{}
        << "$addToSet" << bsoncxx::builder::stream::open_document
        << "occurrences" << bsoncxx::builder::stream::open_document
        << "url" << url
        << "count" << count
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    // Perform the update operation with upsert option
    collection.update_one(
        filter.view(),
        update.view(),
        mongocxx::options::update{}.upsert(true)
    );
}







//this func get arguments of url and dictionary like this: {word:3,animal:43,...} 
void MongoDB::insert_data(const std::string& url, const std::map<std::string, int>& dic) {
    for (const auto& [word, count] : dic) {
        insert_word(url, word, count);
        // std::cout<<word << ": " <<count <<", ";
    }
  
}

// //find the word in the db and return dictionary
// std::map<std::string, int> MongoDB::get_urls(const std::string word) {
//     std::map<std::string, int> url_count_map;

//     // Build the query to find the document by word
//     auto query = bsoncxx::builder::stream::document{} << "word" << word << bsoncxx::builder::stream::finalize;

//     // Find the document in the collection
//     auto maybe_result = collection.find_one(query.view());

//     // Check if the document was found
//     if (maybe_result) {
//         auto doc = maybe_result->view();

//         // Check if the document has the 'occurrences' field and it's an array
//         if (doc["occurrences"] && doc["occurrences"].type() == bsoncxx::type::k_array) {
//             auto occurrences = doc["occurrences"].get_array().value;

//             // Iterate over the array of occurrences
//             for (auto&& occurrence : occurrences) {
//                 if (occurrence["url"] && occurrence["url"].type() == bsoncxx::type::k_utf8 &&
//                     occurrence["count"] && occurrence["count"].type() == bsoncxx::type::k_int32) {
                    
//                     // Extract the URL and count
//                     std::string url = occurrence["url"].get_string().value.to_string();
//                     int count = occurrence["count"].get_int32().value;

//                     // Insert them into the map
//                     url_count_map[url] = count;
//                 }
//             }
//         }
//     }

//     return url_count_map;
// }

