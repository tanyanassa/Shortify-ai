#define ASIO_STANDALONE

#include "crow/app.h"
#include "crow/json.h"

#include <string>

std::string generateShortURL()
{
    return "abc123";
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

        std::string shortURL = generateShortURL();

        crow::json::wvalue response;

        response["original_url"] = originalURL;
        response["short_url"] = shortURL;

        return crow::response(response);
    });

    app.port(18080).multithreaded().run();
}