#pragma once

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <sstream>
#include <iostream>
#include <vector>

bool contain(const char *s, const char *p);
int parseInt(char* s);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
void trim(std::string &s);
std::vector<std::string> split(const std::string& s, char seperator);
