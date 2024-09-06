#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>  // Include JSON header

// Callback function to write the API response into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize cURL
    curl = curl_easy_init();
    if(curl) {
        // Set the URL for the request
        std::string pokemonName;
        std::cout << "Enter the name of the Pokémon: ";
        std::cin >> pokemonName;

        std::string url = "https://pokeapi.co/api/v2/pokemon/" + pokemonName;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set the callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            try {
                // Parse the response data into a JSON object
                nlohmann::json responseData = nlohmann::json::parse(readBuffer);

                // Extract relevant data from the JSON response
                std::string name = responseData["name"];
                int id = responseData["id"];
                std::string type = responseData["types"][0]["type"]["name"];
                int height = responseData["height"];
                int weight = responseData["weight"];

                // Output the formatted data
                std::cout << "Pokémon Info:" << std::endl;
                std::cout << "Name: " << name << std::endl;
                std::cout << "ID: " << id << std::endl;
                std::cout << "Type: " << type << std::endl;
                std::cout << "Height: " << height << " decimetres" << std::endl;
                std::cout << "Weight: " << weight << " hectograms" << std::endl;
            } catch (nlohmann::json::exception& e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }

        // Clean up cURL
        curl_easy_cleanup(curl);
    }
    return 0;
}