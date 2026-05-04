int currentId = 1;
#define ASIO_STANDALONE

#include "crow/app.h"
#include "crow/json.h"

#include <string>

#include <algorithm>

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
int main()
{
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

        std::string shortURL = encodeBase62(currentId++);

        crow::json::wvalue response;

        response["original_url"] = originalURL;
        response["short_url"] = shortURL;

        return crow::response(response);
    });

    app.port(18080).multithreaded().run();
}