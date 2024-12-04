#include "auxiliary_functions.h"
#include <ctime>
#include <memory>
#include <filesystem>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::string getUniqueFilename(void)
{
    std::string filename;
    time_t now(0);
    std::unique_ptr<tm> time(localtime(&now));

    filename = std::to_string(time->tm_hour) + std::to_string(time->tm_min) +
                std::to_string(time->tm_sec) + "_" + std::to_string(time->tm_mday) + 
                std::to_string(time->tm_mon) + std::to_string(time->tm_year + 1900);

    std::vector<std::string> filenames;
    for(auto& entry : std::filesystem::directory_iterator(recordsPath))
    {
        filenames.push_back(entry.path().filename());
    }

    bool isOK = false;
    while(!isOK)
    {
        auto filenamesItr = std::find(filenames.begin(), filenames.end(), filename);
        if(filenamesItr != filenames.end())
        {
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> dist;
            filename += std::to_string(dist(rng)); /*add random number to the end*/
        }
        else
        {
            isOK = true;
        }
    }

    return filename;
}

void readConfLine(std::fstream& file, std::string& str)
{
    std::string tmp;
    int pos;

    std::getline(file, tmp);
    pos = tmp.find('=') + 1;
    str = tmp.substr(pos, tmp.length() - pos);
}

void readConfLine(std::fstream& file, int& val)
{
    std::string tmp;
    int pos;

    std::getline(file, tmp);
    pos = tmp.find('=') + 1;    
    tmp = tmp.substr(pos, tmp.length() - pos);
    val = std::stoi(tmp);
}