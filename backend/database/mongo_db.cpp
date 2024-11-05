#include "mongo_db.hpp"
#include <nlohmann/json.hpp>
#include <fstream>  
#include <mongocxx/exception/exception.hpp>




void MongoDB::init_driver() {
    static mongocxx::instance inst{}; // This should be done only once
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
    }

    //connect to a config file
    std::ifstream config_file("config.json");
    nlohmann::json config;
    config_file >> config;

    db_name = config["database_name"];  //take the databas name from the config file
    collection_words_name =  config["collection_words_name"]; //take the collection_words name from the config file
    visited_urls_name =  config["visited_urls_name"]; //take the  collection visited_urls name from the config file
    unvisited_urls_queue_name =  config["unvisited_urls_queue_name"];

    this->client = mongocxx::client{mongocxx::uri{}}; //connect to database by uri(defult:27017)
    this->db = client[db_name]; //access a database by its name
    this->words_collection = db[collection_words_name]; //access to the words collection 
    this->visited_urls_collection = db[visited_urls_name]; //access to the visited_urls collection
    this->urls_queue_collection = db[unvisited_urls_queue_name]; //access to the visited_urls collection  
}


void MongoDB::put_visited_urls(std::set<std::string> & urls) {
     for (const auto& url : urls) {
        try {
            // Check if the URL already exists in the collection
            bsoncxx::builder::stream::document filter{};
            filter << "url" << url;

            auto cursor = this->visited_urls_collection.find(filter.view());

            if (cursor.begin() != cursor.end()) {
                std::cout << "URL already exists in visited: " << url << std::endl;
                continue; // Skip inserting the URL if it already exists
            }
            

            // Get the current time using std::chrono
            auto now = std::chrono::system_clock::now();
            bsoncxx::types::b_date bson_date{std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch())};

            // Create a new BSON document to insert
            bsoncxx::builder::stream::document document{};
            document << "url" << url
                     << "timestamp" << bson_date;

            // Insert the document into the collection
            this->visited_urls_collection.insert_one(document.view());

        } catch (const mongocxx::exception& e) { 
            std::cerr << "Error inserting URL: " << e.what() << std::endl;
        } catch (const std::exception& e) { 
            std::cerr << "General error: " << e.what() << std::endl;
        }
    }
}


void MongoDB::put_queue_urls(std::queue<std::string>& queue) {
    while (!queue.empty()) {
        auto& url = queue.front();

        try {
            // Check if the URL already exists in the collection
            bsoncxx::builder::stream::document filter{};
            filter << "url" << url;

            auto cursor = this->urls_queue_collection.find(filter.view());

            if (cursor.begin() != cursor.end()) {
                std::cout << "URL already in queue: " << url << std::endl;
                queue.pop(); // Ensure the URL is removed from the queue
                continue; // Skip inserting the URL if it already exists
            }

            // Create a new BSON document to insert
            bsoncxx::builder::stream::document document{};
            document << "url" << url;

            // Insert the document into the collection
            this->urls_queue_collection.insert_one(document.view());

        } catch (const mongocxx::exception& e) {
            std::cerr << "Error inserting URL: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "General error: " << e.what() << std::endl;
        }

        queue.pop(); // Remove URL after processing
    }
}



std::set<std::string> MongoDB::get_visited_urls() {
    std::set<std::string> visited_urls; // Set to store unique URLs

    try {
        auto cursor = this->visited_urls_collection.find({}); // Fetch all documents from the collection

        for (const auto& doc : cursor) {
            // Directly access the URL and convert it to std::string
            visited_urls.insert(doc["url"].get_string().value.to_string()); // Extract URL and insert into the set
        }

    } catch (const mongocxx::exception& e) {
        std::cerr << "Error retrieving URLs: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }

    return visited_urls; // Return the set of visited URLs
}



std::queue<std::string> MongoDB::get_unvisited_queue(int& urls_needed) {
    std::queue<std::string> queue_urls; // Set queue to store unvisited URLs

    try {
        auto cursor = this->urls_queue_collection.find({}); // Fetch all documents from the collection
        int url_count = 0;
        for (const auto& doc : cursor) {
            if(url_count == urls_needed){
                 break; 
            }
            url_count++;
            // Directly access the URL and convert it to std::string
            queue_urls.push(doc["url"].get_string().value.to_string()); // Extract URL and insert into the queue
        }

    } catch (const mongocxx::exception& e) {
        std::cerr << "Error retrieving URLs: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General error: " << e.what() << std::endl;
    }

    return queue_urls; // Return the set of visited URLs
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
    words_collection.update_one(
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
