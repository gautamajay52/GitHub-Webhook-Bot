// A Simple Telegram Bot, which will listen to GitHub Webhook and inform via Telegram

// Copyright (C) 2021-Present Gautam Kumar <https://github.com/gautamajay52>

#include <iostream>
#include <string>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>

#include <string>

#include "crow_all.h"
#include "helper.h"

using namespace std;

int main()
{
    crow::SimpleApp app;

    //just to handle normal get requests
    CROW_ROUTE(app, "/")
    ([]() { return "Hey! I am working !!"; });

    //just to handle /start message ⚡ (with webhook)
    CROW_ROUTE(app, "/telegram").methods("POST"_method)([](const crow::request &req) {
        auto x = crow::json::load(req.body);
        if (!x)
            return crow::response(400);
        int user = x["message"]["from"]["id"].i();
        string text = x["message"]["text"].s();
        if (text == "/start")
        {
            send_mess(rep_mess, user);
        }
        return crow::response(200);
    });

    //just to handle github post requests ⚡
    CROW_ROUTE(app, "/github").methods("POST"_method)([](const crow::request &req) {
        auto x = crow::json::load(req.body);

        if (!x)
            return crow::response(400);

        string events = req.get_header_value("X-GitHub-Event"); //to get event-name

        string message = parse_json(x, events);

        send_mess(message, usr_id);

        string ret_mess = "By gautamajay52";
        std::ostringstream os;
        os << ret_mess;
        return crow::response{os.str()};
    });

    app.port(PORT).multithreaded().run();
}
