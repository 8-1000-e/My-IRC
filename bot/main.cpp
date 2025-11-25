/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emile <emile@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 14:02:11 by edubois-          #+#    #+#             */
/*   Updated: 2025/11/24 17:09:48 by emile            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

std::string get_project( std::string json, size_t occ_pos)
{
    // 1) Remonter au début du bloc JSON
    size_t start = json.rfind("{", occ_pos);
    if (start == std::string::npos)
        return "";

    // 2) Chercher le champ "project" *dans ce bloc*
    const std::string key = "\"project\": \"";
    size_t proj_pos = json.find(key, start);
    if (proj_pos == std::string::npos)
        return "";

    proj_pos += key.size(); // se placer juste après le dernier guillemet

    // 3) Trouver la fin du nom du projet
    size_t end = json.find("\"", proj_pos);
    if (end == std::string::npos)
        return "";

    return json.substr(proj_pos, end - proj_pos);
}

std::string get_bearer()
{
    system("curl -X POST \
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

void get_time(std::map<std::string, Info> &tab, const std::string &project, const std::string &name)
{
    std::string BEARER = get_bearer();


    std::string payload = "curl -s -H \"Authorization: Bearer " + BEARER + "\" \
  'https://api.intra.42.fr/v2/users/" + name + "/projects_users' \
  | jq '.[] | select(.project.name == \"" + project + "\") | {project_name: .project.name, marked: .marked, marked_at: .marked_at, created_at: .created_at}' > Time.json";

    system(payload.c_str());

    std::ifstream file("Time.json");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;
        
    std::string key = "\"marked\"";
    auto it = json.find(key);
    tab[project].marked = (bool)(json[it + key.size() + 2] == 't');

    key = "created_at";
    it = json.find(key);
    std::string iso = json.substr(it + key.size() + 4, 10);
    int year  = atoi(iso.substr(0, 4).c_str());
    int month = atoi(iso.substr(5, 2).c_str());
    int day   = atoi(iso.substr(8, 2).c_str());

    struct tm t;
    memset(&t, 0, sizeof(t));
    t.tm_year = year - 1900; // tm_year = années depuis 1900
    t.tm_mon  = month - 1;   // tm_mon = 0–11
    t.tm_mday = day;
    tab[project].created_at = mktime(&t);

    key = "marked_at";
    it = json.find(key);
    iso = json.substr(it + key.size() + 4, 10);
    year  = atoi(iso.substr(0, 4).c_str());
    month = atoi(iso.substr(5, 2).c_str());
    day   = atoi(iso.substr(8, 2).c_str());

    memset(&t, 0, sizeof(t));
    t.tm_year = year - 1900; // tm_year = années depuis 1900
    t.tm_mon  = month - 1;   // tm_mon = 0–11
    t.tm_mday = day;
    tab[project].marked_at = mktime(&t);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "Usage ./a.out <login>" << std::endl;
        return 0;
    }
    std::string BEARER = get_bearer();
    // std::cout<< "BEARER = "<< BEARER << std::endl << std::endl << std::endl << std::endl ;

    std::string payload = "curl -s -H \"Authorization: Bearer "+BEARER+ "\" \
  https://api.intra.42.fr/v2/users/"+argv[1]+"/projects_users \
  | jq '.[] | {project: .project.name, occurrence: .occurrence, mark: .final_mark, status: .status}' > Occu.json";

    system(payload.c_str());

    std::ifstream file("Occu.json");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;
    std::string key = "occurrence";


    std::map<std::string, Info> tab;
    auto it = json.find(key);
    // std::cout << "json = " << json << std::endl<< std::endl<< std::endl<< std::endl;
    Info info;
    while (it != std::string::npos)
    {
        std::string project = get_project(json, it);
        if (project.empty())
            break ;
        int nb = json[it + key.size() + 3] - '0';
        if (isdigit(json[it + key.size() + 4]))
            nb = nb * 10 + json[it + key.size() + 4] - '0';
        info.nb = ++nb;
        tab[project] = info;        
        it = json.find(key, it + key.size());
    }
    remove("Occu.json");

    for (std::map<std::string ,Info>::iterator it = tab.begin(); it != tab.end(); it++)
        get_time(tab, it->first, argv[1]);
        remove("Time.json");
        
    for (std::map<std::string ,Info>::iterator it = tab.begin(); it != tab.end(); it++)
    {
    std::cout << "-----------------------------\n";
    std::cout << "Project:     " << it->first << "\n";
    std::cout << "try:          " << it->second.nb << "\n";
    std::cout << "marked:      " << (it->second.marked ? "true" : "false") << "\n";
    std::cout << "created_at:  " << it->second.created_at << "\n";
    std::cout << "marked_at:   " << it->second.marked_at << "\n";
    std::cout << "-----------------------------\n\n";
    }
}



