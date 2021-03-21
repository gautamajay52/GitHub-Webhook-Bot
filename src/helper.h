// A Simple Telegram Bot, which will listen to GitHub Webhook and inform via Telegram

// Copyright (C) 2021-Present Gautam Kumar <https://github.com/gautamajay52>

#include <iostream>
#include <string>

using namespace std;

long usr_id = atol(getenv("USER_ID"));
string usr_bot = getenv("TOKEN");

char *port = getenv("PORT");
uint16_t PORT = static_cast<uint16_t>(port != NULL ? atoi(port) : 18080);

string rep_mess = "Hey! I am working ⚡"; // Reply message after sending /start to your bot (only works if webhook is set)

void send_mess(string message, long usr)
{
    string gcmd = string("curl ") + "-X " + "POST " + "-H " + "'Content-Type=application/json' " + "-d " + "'chat_id=" + to_string(usr) + "&text=" + message + "&disable_notification=false&parse_mode=markdown&disable_web_page_preview=true' " + "https://api.telegram.org/bot" + usr_bot + "/sendMessage";
    system(gcmd.c_str());
}

string parse_json(crow::json::rvalue &x, string &events)
{

    string mess;

    //available in all json
    string repo_name = x["repository"]["name"].s();
    string repo_url = x["repository"]["html_url"].s();
    string usr_name = x["sender"]["login"].s();
    string usr_url = x["sender"]["html_url"].s();

    int forks = x["repository"]["forks_count"].i();
    int stars = x["repository"]["stargazers_count"].i();

    //parsing others
    // if(events=="watch") {
    //     //Do-Nothing :)
    // }

    if (events == "fork")
    {
        mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *forked* " + "[" + repo_name + "]" + "(" + repo_url + ")👌" + "\n\n" + "*Forks*:: " + to_string(forks) + "🍴 " + "*Stars*:: " + to_string(stars) + " ⭐\n\n🚧 #forkee";
    }

    else if (events == "star" || events == "issues")
    {
        string c_d = x["action"].s();
        if (c_d == "created")
        {
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *starred* " + "[" + repo_name + "]" + "(" + repo_url + ")🔥" + "\n\n" + "*Forks*:: " + to_string(forks) + "🍴 " + "*Stars*:: " + to_string(stars) + " ⭐\n\n🚧 #starred";
        }
        else if (c_d == "deleted")
        {
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *unstarred* " + "[" + repo_name + "]" + "(" + repo_url + ")🤬" + "\n\n" + "*Forks*:: " + to_string(forks) + "🍴 " + "*Stars*:: " + to_string(stars) + " ⭐\n\n🚧 #unstarred";
        }
        else if (c_d == "opened")
        {
            string isu_title = x["issue"]["title"].s();
            string isu_url = x["issue"]["html_url"].s();
            int isu_num = x["issue"]["number"].i();
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *created an issue* " + "[#" + to_string(isu_num) + "]" + "(" + isu_url + ") 🥺" + "\n\n" + "👉 " + isu_title + "\n\n🚧 #issue";
        }
        else if (c_d == "closed")
        {
            string isu_title = x["issue"]["title"].s();
            string isu_url = x["issue"]["html_url"].s();
            int isu_num = x["issue"]["number"].i();
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *closed an issue* " + "[#" + to_string(isu_num) + "]" + "(" + isu_url + ") 🥺" + "\n\n🚧 #issue";
        }
    }
    else if (events == "issue_comment")
    {
        string c_d = x["action"].s();
        string isu_title = x["issue"]["title"].s();
        string isu_url = x["issue"]["html_url"].s();
        string isu_comm = x["comment"]["body"].s();
        int isu_num = x["issue"]["number"].i();
        if (c_d == "created")
        {
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *commented at* " + "[#" + to_string(isu_num) + "]" + "(" + isu_url + ") 👀" + "\n\n" + "👉 " + isu_comm + "\n\n🚧 #issue";
        }
        if (c_d == "edited")
        {
            string bef_comm = x["changes"]["body"]["from"].s();
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *edited at* " + "[#" + to_string(isu_num) + "]" + "(" + isu_url + ") 👀" + "\n\n" + "👉*From:* " + bef_comm + "\n👉*To:* " + isu_comm + "\n\n🚧 #issue";
        }
        else if (c_d == "deleted")
        {
            mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *deleted at* " + "[#" + to_string(isu_num) + "]" + "(" + isu_url + ") 👀" + "\n\n" + "👉 " + isu_comm + "\n\n🚧 #issue";
        }
    }
    else if (events == "pull_request")
    {
        string c_d = x["action"].s();
        int pull_no = x["number"].i();
        string pull_url = x["pull_request"]["html_url"].s();
        string pull_title = x["pull_request"]["title"].s();
        mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ") " + "*" + c_d + "*" + " *at* " + "[#" + to_string(pull_no) + "]" + "(" + pull_url + ") 🏃" + "\n\n" + "👉 " + pull_title + "\n\n🚧 #PullRequest";
    }
    else if (events == "push")
    {
        string push_url = x["head_commit"]["url"].s();
        string push_id = x["head_commit"]["id"].s();
        mess = "🤖: [" + usr_name + "]" + "(" + usr_url + ")" + " *pushed at* " + "[#" + push_id.substr(0, 7) + "]" + "(" + push_url + ") 🌡️" + "\n\n🚧 #Pushed";
    }
    return mess;
}
