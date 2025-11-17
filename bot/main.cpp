

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>
#define auto __auto_type

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

    return json.substr(proj_pos, end - proj_pos);
}

int main(int argc, char **argv)
{
    std::string BEARER = get_bearer();
    std::cout<< "BEARER = "<< BEARER << std::endl << std::endl << std::endl << std::endl ;

    std::string payload = "curl -s -H \"Authorization: Bearer "+BEARER+ "\" \
  https://api.intra.42.fr/v2/users/"+argv[1]+"/projects_users \
  | jq '.[] | {project: .project.name, occurrence: .occurrence, mark: .final_mark, status: .status}' > Occu.json";

    system(payload.c_str());
        
    std::map<std::string, int> tab;
    std::ifstream file("Occu.json");
    std::string line;
    std::string json;

    while (std::getline(file, line))
        json += line;
    std::string key = "occurrence";

    int i =1;
    auto it = json.find(key);
    // std::cout << "json = " << json << std::endl<< std::endl<< std::endl<< std::endl;
    while (it != std::string::npos)
    {
        std::string project = get_project(json, it);
        if (project.empty())
            break ;
        std::cout << project << " | " << "failed attempt = " << json[it + key.size() + 3] << std::endl;
        it = json.find(key, it + key.size());
        i++;
    }
}
