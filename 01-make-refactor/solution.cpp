#include <string>
#include <iostream>
#include <regex>
using namespace std;

//
// Created by kuzdavoj@fel.cvut.cz on 7/15/2022.
//

const regex SECOND_LINE (R"(^([\w]*):)");
const regex THIRD_LINE ("^\tg\\+\\+ ([\\w.]*) ([\\w.]*) -o [\\w.]*$");
const regex DEP_TO_DEP (R"(^([\w.]*): ([\w.]*) ([\w.]*))");
// TODO: if there are more than two files after ":", make two regexes

const regex codeMatch ("^([a-zA-Z0-9.]*):$");

const int FRESH = 0;
const int OPEN = 1;
const int CLOSED = 2;

// TODO: consider unordered_map<vertex_type,unordered_set<vertex_type>>


void graphFromInput(vector<vector<int>> &adj, deque<string> &dict);
void dfsCheckConnectivity(vector<vector<int>> &adj);
template<typename Container, typename Primitive>
int contains(Container& c, Primitive e);

int main() {
    vector<vector<int>> adj;   // edges and neighbours as ints
    deque<string> dict;        // string to num conversion

    graphFromInput(adj, dict);
    dfsCheckConnectivity(adj);

    return 0;
}

template<typename Container, typename Primitive>
int contains(Container& c, Primitive e) {
    if(c.empty()) {
        return -1;
    }
    auto iter = std::find(c.begin(), c.end(), e);
    if(iter != c.end()) {
        return int(iter-c.begin());
    }
    else {
        return -1;
    }
}

void dfsCheckConnectivity(vector<vector<int>> &adj) {
    vector<int> visited;
    stack<int> toVisit;
    toVisit.push(0);
    while (!toVisit.empty()) {
        int curr = toVisit.top();
        toVisit.pop();
        cout << curr;
        if(contains(visited, curr) == -1) {
            visited.push_back(curr);
            for (int neighbor : adj[curr]) {
                toVisit.push(neighbor);
            }
        }
    }
}


void graphFromInput(vector<vector<int>> &adj, deque<string> &dict) {
    int rootAdjIndex = 0;
    int depDictIndex = 0;
    string line;
    smatch matches;

    while (getline(cin, line)) { // stream file
        if (regex_match(line, matches, SECOND_LINE)) {
            // node is root and is not in dict
            string firstTarget = matches[1].str();
            adj.emplace_back();
            dict.insert(dict.begin() + depDictIndex, firstTarget);
            depDictIndex++;

            // read third line
            getline(cin, line);
            regex_match(line, matches, THIRD_LINE);
            for (int i = 1; i < matches.size(); ++i) {
                // subnode not in dict
                string dep = matches[i].str();
                adj.emplace_back();
                dict.insert(dict.begin() + depDictIndex, dep);
                adj[rootAdjIndex].push_back(depDictIndex);
                depDictIndex++;
            }
        }
        else if (regex_match(line, matches, DEP_TO_DEP)) { // input is of format [dep1: dep2 dep3]
            for (int i = 1; i < matches.size(); ++i) {   // submatch loop (first match is the whole match)
                string dep = matches[i].str();
                auto depDictFoundPosition = contains(dict, dep);

                if (i==1) { // the node is root
                    if (depDictFoundPosition != -1) { // root node in dict
                        rootAdjIndex = depDictFoundPosition;
                    }
                    else { // root node not in dict
                        adj.emplace_back();
                        dict.insert(dict.begin() + depDictIndex, dep);
                        rootAdjIndex = depDictIndex;
                        depDictIndex++;
                    }
                }
                else {
                    if( depDictFoundPosition != -1 ) { // subnode in dict
                        adj[rootAdjIndex].push_back(depDictFoundPosition);
                        // no increment because the node was already added to dict
                    } else { // subnode not in dict
                        adj.emplace_back();
                        dict.insert(dict.begin() + depDictIndex, dep);
                        adj[rootAdjIndex].push_back(depDictIndex);
                        depDictIndex++;
                    }
                }

            }
        }
    }
}



