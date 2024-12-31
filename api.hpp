#ifndef API_HPP
#define API_HPP
#include <iostream>
#include <string>
#include "curl/curl.h"       // curl se request bhejne ke liye
#include "nlohmann/json.hpp" // JSON library -kyunki websocket mein json data aayega

using namespace std;
using json = nlohmann::json;
// Class that will handle the API requests in derbit
class API
{
public:
    API(string apikey, string apisecret):apikey(apikey),apisecret(apisecret),accesstoken(""){}
    // The Derbian takes the json and connects to the web socket
    size_t writecallback(void* contents, size_t size, size_t nmemb, string *s)
    {
        s->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    string mkRequest(string url, string payload){
        CURL *curl;
        CURLcode res;
        string response;
        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1LL); // make a post request
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writecallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            
            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                cout<<"curl fail ho gya"<<curl_easy_strerror(res)<<endl;
            }
            curl_easy_cleanup(curl);
        }
        return response;
    }
    void authenticate()
    {
        string url = "https://test.deribit.com/api/v2/public/auth";
        json payload = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "public/auth"},
            {"params", 
            {
            {"grant_type", "client_credentials"}, 
            {"client_id", apikey}, 
            {"client_secret", apisecret}
            }}
            };
        string response = mkRequest(url, payload.dump());
        json j = json::parse(response);
        
        if(j.contains("result") and j["result"].contains("access_token"))accesstoken = j["result"]["access_token"];
        else throw runtime_error("Authentication failed");
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
    void Place_Order(string name, double price, double quantity){
        string url = "https://test.deribit.com/api/v2/private/buy";
        json payload = {
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
            }}
            };
        string response = mkRequest(url, payload.dump());
        json j = json::parse(response);
        cout<<"Order Placed Response:"<<j.dump(4)<<endl;
    }
    void Cancel_Order(string order_id){
        string url = "https://test.deribit.com/api/v2/private/cancel";
        json payload = {
            {"jsonrpc", "2.0"},
            {"id", 4214},
            {"method", "private/cancel"},
            {"params", 
            {
            {"order_id", order_id},
            {"access_token", accesstoken}
            }}
            };
        string response = mkRequest(url, payload.dump());
        json j = json::parse(response);
        cout<<"Order Cancel Response:"<<j.dump(4)<<endl;
    }
    void Modify_Order(string order_id, double price, double amount){
        string url ="https://test.deribit.com/api/v2/private/edit";
        json payload = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/edit"},
            {"params", 
            {
            {"order_id", "string"}, 
            {"amount", amount}, 
            {"price", price},
            {"access_token", accesstoken}
            }}
            };
    }
    void Get_Order(string name){
        string url = "https://test.deribit.com/api/v2/private/get_order_state";
        json payload = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/get_order_state"},
            {"params", 
            {
            {"order_id", name}, 
            {"access_token", accesstoken}
            }}
            };
        string response = mkRequest(url, payload.dump());
        json j = json::parse(response);
        cout<<"Order State Response:"<<j.dump(4)<<endl;
    }
    void ViewPositions(string currency, string kind){
        string url = "https://test.deribit.com/api/v2/private/get_positions";
        json payload = {
            {"jsonrpc", "2.0"},
            {"id", 1},
            {"method", "private/get_positions"},
            {"params", 
            {
            {"currency", currency}, 
            {"kind", kind},
            {"access_token", accesstoken}
            }}
            };
    }

private:
    string apikey;
    string apisecret;
    string accesstoken;
};
#endif