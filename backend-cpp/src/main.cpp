int currentId = 1;
#define ASIO_STANDALONE

#include "crow/app.h"
#include "crow/json.h"
#include <regex>
#include <string>

#include <algorithm>

#include <unordered_map>
#include <fstream>
std::unordered_map<std::string, int> clickCount;
std::unordered_map<std::string, std::string> urlMap;
std::string encodeBase62(int num)
{
    std::string chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::string shortURL;

    while (num > 0)
    {
        shortURL += chars[num % 62];
        num /= 62;
    }

    std::reverse(shortURL.begin(), shortURL.end());

    return shortURL;
}
bool isValidURL(const std::string& url)
{
    std::regex pattern("(http|https)://(www\\.)?.+");
    return std::regex_match(url, pattern);
}
void saveToFile(const std::string& code, const std::string& url)
{
    std::ofstream file("data.txt", std::ios::app);
    file << code << " " << url << std::endl;
}
void loadFromFile()
{
    std::ifstream file("data.txt");
    std::string code, url;

    while (file >> code >> url)
    {
        urlMap[code] = url;
    }
}
int main()
{
    loadFromFile(); 
    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")
    ([]() {
        return "Shortify AI Backend Running";
    });

    CROW_ROUTE(app, "/shorten")
    .methods("POST"_method)
    ([](const crow::request& req)
    {
        auto body = crow::json::load(req.body);

        if (!body)
        {
            return crow::response(400, "Invalid JSON");
        }

        std::string originalURL = body["url"].s();
 std::string shortURL;

if (body.has("custom_code"))
{
    shortURL = body["custom_code"].s();

    if (urlMap.find(shortURL) != urlMap.end())
    {
        return crow::response(400, "Custom code already exists");
    }
}
else
{
    shortURL = encodeBase62(currentId++);
}       
if (!isValidURL(originalURL))
{
    return crow::response(400, "Invalid URL format");
}
        urlMap[shortURL] = originalURL;
        saveToFile(shortURL, originalURL);

        crow::json::wvalue response;

        response["original_url"] = originalURL;
        response["short_url"] = shortURL;

        return crow::response(response);
    });
CROW_ROUTE(app, "/<string>")
([](std::string code) {

    if (urlMap.find(code) == urlMap.end())
    {
        return crow::response(404, "URL not found");
    }
clickCount[code]++;
    crow::response res;
    res.code = 302;
    res.set_header("Location", urlMap[code]);

    return res;
});
CROW_ROUTE(app, "/stats/<string>")
([](std::string code) {

    if (urlMap.find(code) == urlMap.end())
    {
        return crow::response(404, "URL not found");
    }

    crow::json::wvalue res;

    res["url"] = urlMap[code];
    res["clicks"] = clickCount.count(code) ? clickCount[code] : 0;

    return crow::response(res);
});
    app.port(18080).multithreaded().run();
}