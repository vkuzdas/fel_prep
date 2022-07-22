#include <string>
#include <iostream>
#include <regex>
using namespace std;

//
// Created by kuzdavoj@fel.cvut.cz on 7/15/2022.
//

const regex firstLineMatch ("^([a-zA-Z]*)(: )([a-zA-Z]*)$");
const regex depToDepMatch1 (R"(^([\w.]*)(: )([\w.]*)( )([\w.]*))");
const regex depToDepMatch2 (R"(^([\w.]*): ([\w.]*) ([\w.]*))");
// TODO: if there are more than two files after ":", make two regexes
const regex codeMatch ("^([a-zA-Z0-9.]*):$");

const int FRESH = 0;
const int OPEN = 1;
const int CLOSED = 2;

// TODO: consider unordered_map<vertex_type,unordered_set<vertex_type>>


int main() {
    string line;
    smatch matches;
    vector<vector<int>> adj;   // edges and neighbours as ints
    deque<string> dict;        // string to num conversion
    int depLinesNr = 0;        // each line contains "root" dep
    int depOrder = 0;

    while (getline(cin, line)) {
        if (regex_match(line, matches, depToDepMatch2)) {
            for (unsigned int i = 1; i < matches.size(); ++i) {
                string dep = matches[i].str();
                // add to dict if missing
                if( !(std::find(dict.begin(), dict.end(), dep) != dict.end()) ){
                    dict.insert(dict.begin() + depOrder, dep);
                    depOrder = depOrder + 1;
                }
                // insert into adj
                adj[depLinesNr].push_back(depOrder);
            }
        }
    }
    return 0;
}



