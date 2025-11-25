/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BOT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edubois- <edubois-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:00:00 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/25 15:00:00 by edubois-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <map>

static std::string get_project(std::string json, size_t occ_pos)
{
    size_t start = json.rfind("{", occ_pos);
    if (start == std::string::npos)
        return "";

    const std::string key = "\"project\": \"";
    size_t proj_pos = json.find(key, start);
    if (proj_pos == std::string::npos)
        return "";

    proj_pos += key.size();
    size_t end = json.find("\"", proj_pos);
    if (end == std::string::npos)
        return "";

    return json.substr(proj_pos, end - proj_pos);
}

static std::string get_bearer()
{
    system("curl -s -X POST \
        --data \"grant_type=client_credentials&client_id=u-s4t2ud-8de6e2a010f1aec1b3125cc5e1009367c1f54f31dc284b249b097cc1557824ab&client_secret=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\" \
        https://api.intra.42.fr/oauth/token > \"Auth token\"");

    std::ifstream file("Auth token");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;

    const std::string key = "\"access_token\"";
    size_t proj_pos = json.find(key);
    if (proj_pos == std::string::npos)
        return "";
    proj_pos += key.size() + 2;
    size_t end = json.find("\"", proj_pos);
    if (end == std::string::npos)
        return "";

    remove("Auth token");
    return json.substr(proj_pos, end - proj_pos);
}

static void fill_data(std::map<std::string, Info> &tab, const std::string &project)
{
    std::ifstream file("Time.json");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;

    auto project_it = json.find(project);
    json = json.substr(project_it, json.length());
    json += project_it;
    std::string key = "\"marked\"";
    auto it = json.find(key);
    tab[project].marked = (bool)(json[it + key.size() + 2] == 't');

    key = "created_at";
    it = json.find(key);
    std::string iso = json.substr(it + key.size() + 4, 10);
    int year  = atoi(iso.substr(0, 4).c_str());
    int month = atoi(iso.substr(5, 2).c_str());
    int day   = atoi(iso.substr(8, 2).c_str());

    key = "\"mark\"";
    it = json.find(key);
    tab[project].validated = (bool)(json[it + key.size() + 2] != '0');

    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    tab[project].created_at = mktime(&t);

    key = "marked_at";
    it = json.find(key);
    iso = json.substr(it + key.size() + 4, 10);
    year  = atoi(iso.substr(0, 4).c_str());
    month = atoi(iso.substr(5, 2).c_str());
    day   = atoi(iso.substr(8, 2).c_str());

    memset(&t, 0, sizeof(t));
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    tab[project].marked_at = mktime(&t);
}

static int days_since(time_t ts)
{
    time_t now = time(NULL);
    double diff = difftime(now, ts);
    return (int)(diff / 86400);
}

static std::string human_duration(time_t duration)
{
    int days  = duration / 86400;
    int months = days / 30;
    int years  = days / 365;

    if (years >= 1)
    {
        char buf[64];
        sprintf(buf, "%d year%s", years, (years > 1 ? "s" : ""));
        return std::string(buf);
    }

    if (months >= 1)
    {
        char buf[64];
        sprintf(buf, "%d month%s", months, (months > 1 ? "s" : ""));
        return std::string(buf);
    }

    char buf[64];
    sprintf(buf, "%d day%s", days, (days > 1 ? "s" : ""));
    return std::string(buf);
}

static std::string int_to_string(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

static void roast_user(std::map<std::string, Info> tab, int fd, Server *server)
{
    std::string bestKey;
    int maxNb = -1;

    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (it->second.nb > maxNb)
        {
            maxNb = it->second.nb;
            bestKey = it->first;
        }
    }

    std::string msg = "LOL, that guy really tried " + bestKey + " " + int_to_string(maxNb) + " times, is that guy even real??";
    server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);

    std::string s_bestKey;
    maxNb = -1;
    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (it->second.nb > maxNb && it->first != bestKey)
        {
            maxNb = it->second.nb;
            s_bestKey = it->first;
        }
    }

    msg = "AIN'T NO WAY IT TOOK BRO " + int_to_string(maxNb) + " tries to validate " + s_bestKey + ", who's that guy ??";
    server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);

    bool registered = false;
    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (!it->second.marked)
        {
            msg = "Bro still hasn't done " + it->first + " in " + int_to_string(days_since(it->second.created_at)) + " days, what the hell is he doing ??";
            server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);
            registered = true;
        }
    }
    if (!registered)
    {
        msg = "Bro is not registered to any project ?! What the hell is he doing ??";
        server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);
    }

    time_t max_time = -1;
    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (it->second.validated &&  it->second.marked && it->second.marked_at - it->second.created_at > max_time)
        {
            max_time = it->second.marked_at - it->second.created_at;
            bestKey = it->first;
        }
    }

    msg = "How did it took bro " + human_duration(max_time) + " to validate " + bestKey + "?! How did bro struggle this much !?!";
    server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);

    max_time = -1;
    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (it->second.validated && it->second.marked && it->second.marked_at - it->second.created_at > max_time && it->first != bestKey)
        {
            max_time = it->second.marked_at - it->second.created_at;
            s_bestKey = it->first;
        }
    }

    msg = "Seriously ??? " + human_duration(max_time) + " to make " + s_bestKey + " how is it possible??";
    server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);

    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); ++it)
    {
        if (!it->second.validated && it->second.marked)
        {
            msg = "How did bro failed " + it->first + " ?!";
            server->_sendResponse(":Bot!bot@localhost PRIVMSG " + server->GetServerClient(fd)->GetNickName() + " :" + msg + "\r\n", fd);
        }
    }
}

void Server::BOT(std::string cmd, int fd)
{
    std::vector<std::string> splited = splitCmd(cmd);

    if (splited.size() < 2)
    {
        _sendResponse(":Bot!bot@localhost PRIVMSG " + GetServerClient(fd)->GetNickName() + " :Usage: BOT <42-login>\r\n", fd);
        return;
    }

    std::string login = splited[1];
    std::string BEARER = get_bearer();

    if (BEARER.empty())
    {
        _sendResponse(":Bot!bot@localhost PRIVMSG " + GetServerClient(fd)->GetNickName() + " :Error: Could not authenticate with 42 API\r\n", fd);
        return;
    }

    std::string payload = "curl -s -H \"Authorization: Bearer "+BEARER+ "\" \
  https://api.intra.42.fr/v2/users/"+login+"/projects_users \
  | jq '.[] | {project: .project.name, occurrence: .occurrence, mark: .final_mark, status: .status}' > Occu.json";

    system(payload.c_str());

    std::ifstream file("Occu.json");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;

    if (json.empty())
    {
        _sendResponse(":Bot!bot@localhost PRIVMSG " + GetServerClient(fd)->GetNickName() + " :Error: Could not find user '" + login + "'\r\n", fd);
        remove("Occu.json");
        return;
    }

    std::string key = "occurrence";
    std::map<std::string, Info> tab;
    auto it = json.find(key);
    Info info;

    while (it != std::string::npos)
    {
        std::string project = get_project(json, it);
        if (project.empty())
            break;
        int nb = json[it + key.size() + 3] - '0';
        if (isdigit(json[it + key.size() + 4]))
            nb = nb * 10 + json[it + key.size() + 4] - '0';
        info.nb = ++nb;
        tab[project] = info;
        it = json.find(key, it + key.size());
    }

    BEARER = get_bearer();
    payload = "curl -s -H \"Authorization: Bearer " + BEARER + "\" \
    'https://api.intra.42.fr/v2/users/" + login + "/projects_users' \
    | jq '.[]| {project_name: .project.name, marked: .marked, mark: .final_mark, marked_at: .marked_at, created_at: .created_at}' > Time.json";

    system(payload.c_str());

    for (std::map<std::string, Info>::iterator it = tab.begin(); it != tab.end(); it++)
        fill_data(tab, it->first);

    remove("Time.json");
    remove("Occu.json");

    roast_user(tab, fd, this);
}
