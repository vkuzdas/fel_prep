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
const regex FILE_LINE (R"(^([\w]*\.[\w]*):)");
const regex CODE_LINE ("^\techo.+");
// TODO: if there are more than two files after ":", make two regexes

const regex codeMatch ("^([a-zA-Z0-9.]*):$");


// TODO: consider unordered_map<vertex_type,unordered_set<vertex_type>>


string graphFromInput(vector<vector<int>> &adj, vector<string> &dict);
vector<int> dfsCheckConnectivity(vector<vector<int>> &adj);
template<typename Container, typename Primitive>
int contains(Container& c, Primitive e);

int main() {
    vector<vector<int>> adj;   // edges and neighbours as ints
    vector<string> dict;        // string to num conversion

    string lastLine = graphFromInput(adj, dict); // returns last readable line (after all deps)
    vector<int> visitable = dfsCheckConnectivity(adj);

    smatch matches;

    bool eraseFollowingLine = false;
    while(true) {
        if(regex_match(lastLine, matches, FILE_LINE)) {
            // is line visitable?
            string file = matches[1].str();
            auto index = int(distance(dict.begin(), find(dict.begin(), dict.end(), file)));
            if(contains(visitable, index) == -1) { // if file cannot be visited comment it out
                cout << "#" << lastLine << "\n";
                eraseFollowingLine = true;
            }
            else {
                cout << lastLine << "\n";
                eraseFollowingLine = false;
            }
        }
        else if(regex_match(lastLine, matches, CODE_LINE) && eraseFollowingLine) {
            cout << "#" << lastLine << "\n";
        }
        else if(regex_match(lastLine, matches, CODE_LINE) && !eraseFollowingLine) {
            cout << lastLine << "\n";
        }

        if(!getline(cin, lastLine)) break;
    }

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

vector<int> dfsCheckConnectivity(vector<vector<int>> &adj) {
    vector<int> visited;
    stack<int> toVisit;
    toVisit.push(0);
    while (!toVisit.empty()) {
        int curr = toVisit.top();
        toVisit.pop();
        if(contains(visited, curr) == -1) {
            visited.push_back(curr);
            for (int neighbor : adj[curr]) {
                toVisit.push(neighbor);
            }
        }
    }
    return visited;
}


string graphFromInput(vector<vector<int>> &adj, vector<string> &dict) {
    int rootAdjIndex = 0;
    int depDictIndex = 0;
    string line;
    smatch matches;

    while (getline(cin, line)) { // stream file
        cout << line << "\n";
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
        else if (regex_match(line, matches, FILE_LINE)) {
            return line;
        }
    }
    return ""; // pretty much guaranteed it returns "dep.cpp"
}



