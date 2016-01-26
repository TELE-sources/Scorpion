//
// Created by bnunnall on 8/19/2015.
//

#include "filestream.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;


bool FileStream::exists(const char *file)
{
    std::ifstream infile(file);
       return infile.good();
}

bool is_whitespace(char c)
{
    return ((c ==' ') || (c =='\n') ||
        (c =='\r') || (c =='\t') ||
        (c =='\b') || (c =='\v') ||
        (c =='\f')) ;
}

bool FileStream::empty(const char *file)
{
    string data = getfile(file);
    for(long i = 0; i < data.size(); i++)
    {
        if(!is_whitespace(data.at(i)))
          return false;
    }
    
    return true;
}

bool FileStream::txtempty(string data)
{
    for(long i = 0; i < data.size(); i++)
    {
        if(!is_whitespace(data.at(i)))
          return false;
    }
    
    return true;
}

int FileStream::out(const char *file, string data)
{
    try {
        ofstream f (file);
         if (f.is_open())
        {
           f << data;
           f.close();
           return 0;
        }
         else
           return -1;
    }
    catch(std::bad_alloc& ba){
         return -1;
    }
}

string FileStream::getfile(const char *file)
{
    stringstream f;
    f << "";
    
    try {
        string tmp;
        ifstream input(file);
    
        while(!input.eof()) {
            tmp = "";
            getline(input, tmp);
            f << tmp;
            f << "\n";
        }
    }
    catch(std::bad_alloc& ba){
        return f.str();  
    }
    catch(std::exception& e){
        return f.str();
    }
    
    return f.str();
}

bool FileStream::endswith(string ext, string file)
{
    int ext_t = ext.length();
    stringstream extension;
    for(int i = file.length() - ext_t; i < file.length(); i++){
        extension << file.at(i);
    }
    
    if(extension.str() == ext)
      return true;
    return false;
}