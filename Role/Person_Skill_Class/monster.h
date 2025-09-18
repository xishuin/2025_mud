#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
#include "Person.h"

using json = nlohmann::json;
using namespace std;

map<int,Person> loadMonstersFromJson(const string& filename);
map<int ,Person> loadBossFromJson(const string& filename);
void saveMonstersToJson(const vector<Person>& monsters, const string& filename);
map<int, Person> loadEventMonstersMap(const string& eventFilename, const string& monsterFilename);