#ifndef API_HPP
#define API_HPP

#include <iostream>
#include <string>
#include "curl/curl.h"       // curl se request bhejne ke liye
#include "nlohmann/json.hpp" // JSON library -kyunki websocket mein json data aayega

using namespace std;
using json = nlohmann::json;

// Class that will handle the API requests in derbit
class API {
public:
    API(string apikey, string apisecret)
        : apikey(apikey), apisecret(apisecret), accesstoken("") {}

    // The Derbian takes the json and connects to the web socket
    static size_t writecallback(void* contents, size_t size, size_t nmemb, void* userp) {
        string* s = static_cast<string*>(userp); // userp ko string pointer mein cast karo
        s->append(static_cast<char*>(contents), size * nmemb); // Data append karo
        return size * nmemb; // Total size return karo
    }

    string mkRequest(string url, string payload) {
        CURL* curl;
        CURLcode res;
        string response;

        curl = curl_easy_init(); // Curl session initialize karo
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // URL set karo
            curl_easy_setopt(curl, CURLOPT_POST, 1L); // POST request bano
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str()); // POST data set karo
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &API::writecallback); // Write callback set karo
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); // Response ke liye string pass karo

            res = curl_easy_perform(curl); // Request perform karo
            if (res != CURLE_OK) { // Agar fail ho gaya
                cout << "curl fail ho gya: " << curl_easy_strerror(res) << endl; // Error print karo
            }
            curl_easy_cleanup(curl); // Curl session cleanup karo
        }
        return response; // Response return karo
    }

    void authenticate() {
        string url = "https://test.deribit.com/api/v2/public/auth";
        json payload = { // JSON payload prepare karo
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "public/auth"},
            {"params",
                {
                    {"grant_type", "client_credentials"},
                    {"client_id", apikey},
                    {"client_secret", apisecret}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo aur response lo
        json j = json::parse(response); // JSON response parse karo

        if (j.contains("result") && j["result"].contains("access_token"))
            accesstoken = j["result"]["access_token"]; // Agar access_token mil gaya to save karo
        else
            throw runtime_error("Authentication failed"); // Error throw karo agar fail ho gaya
    }

    /*
        Core Requirements:

        Order Management Functions
        1. Place order
        2. Cancel order
        3. Modify order
        4. Get orderbook
        5. View current positions
        6. Real-time market data streaming via WebSocket
            - Implement WebSocket server functionality
            - Allow clients to subscribe to symbols
            - Stream continuous orderbook updates for subscribed symbols
    */
    void Place_Order(string name, double price, double quantity) {
        string url = "https://test.deribit.com/api/v2/private/buy";
        json payload = { // Payload prepare karo
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/buy"},
            {"params",
                {
                    {"instrument_name", name},
                    {"amount", quantity},
                    {"price", price},
                    {"type", "limit"},
                    {"access_token", accesstoken}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo
        json j = json::parse(response); // JSON response parse karo
        cout << "Order Placed Response:" << j.dump(4) << endl; // Response print karo
    }

    void Cancel_Order(string order_id) {
        string url = "https://test.deribit.com/api/v2/private/cancel";
        json payload = { // Cancel order ka payload
            {"jsonrpc", "2.0"},
            {"id", 4214},
            {"method", "private/cancel"},
            {"params",
                {
                    {"order_id", order_id},
                    {"access_token", accesstoken}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo
        json j = json::parse(response); // JSON response parse karo
        cout << "Order Cancel Response:" << j.dump(4) << endl; // Response print karo
    }

    void Modify_Order(string order_id, double price, double amount) {
        string url = "https://test.deribit.com/api/v2/private/edit";
        json payload = { // Modify order ka payload
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/edit"},
            {"params",
                {
                    {"order_id", order_id},
                    {"amount", amount},
                    {"price", price},
                    {"access_token", accesstoken}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo
        json j = json::parse(response); // JSON response parse karo
        cout << "Order Modified Response:" << j.dump(4) << endl; // Response print karo
    }

    void Get_Order(string order_id) {
        string url = "https://test.deribit.com/api/v2/private/get_order_state";
        json payload = { // Order state fetch karne ka payload
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/get_order_state"},
            {"params",
                {
                    {"order_id", order_id},
                    {"access_token", accesstoken}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo
        json j = json::parse(response); // JSON response parse karo
        cout << "Order State Response:" << j.dump(4) << endl; // Response print karo
    }

    void ViewPositions(string currency, string kind) {
        string url = "https://test.deribit.com/api/v2/private/get_positions";
        json payload = { // Positions dekhne ka payload
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/get_positions"},
            {"params",
                {
                    {"currency", currency},
                    {"kind", kind},
                    {"access_token", accesstoken}
                }
            }
        };
        string response = mkRequest(url, payload.dump()); // Request bhejo
        json j = json::parse(response); // JSON response parse karo
        cout << "Positions Response:" << j.dump(4) << endl; // Response print karo
    }

private:
    string apikey;       // API key
    string apisecret;    // API secret
    string accesstoken;  // Access token
};

#endif
