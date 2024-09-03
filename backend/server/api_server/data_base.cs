using MongoDB.Driver;
using MongoDB.Bson;
using System.Collections.Generic;
using System;

namespace YourNamespaceName.Data
{
    public static class DatabaseService
    {
        private static readonly MongoClient client = new MongoClient("mongodb://localhost:27017");
        private static readonly IMongoDatabase database = client.GetDatabase("KOOGLE_DB");
        private static readonly IMongoCollection<BsonDocument> collection = database.GetCollection<BsonDocument>("Words_db");

        // Function to get URLs for a specific word
        public static List<string> GetUrlsForWord(string word)
        {
            var filter = Builders<BsonDocument>.Filter.Eq("word", word);
            var document = collection.Find(filter).FirstOrDefault();
            List<string> urls = new List<string>();

            if (document != null && document.Contains("occurrences"))
            {
                var occurrences = document["occurrences"].AsBsonArray;
                foreach (var occurrence in occurrences)
                {
                    if (occurrence["url"] != null)
                    {
                        urls.Add(occurrence["url"].AsString);
                    }
                }
            }
            else
            {
                Console.WriteLine("No document found for the word: " + word);
            }

            return urls;
        }
    }
}
