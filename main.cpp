#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

// These are my global variables in which I will store the information that I read in from the main function.
vector<pair<string, float>> vec;
map<string, vector<pair<string, float>>> graphMap; // <to, <from,1/outdegree(from)>> adjacency list
map<string, float> map1;                           // from and outdegree
map<string, float> resMap;                         // from and correspoding page rank

string from;
string to;

// This function is used in the main function and it reads in the webpages that point to other webpages
// and adds in how many outdegrees that webpage has
void map1Func(string &from, map<string, float> &map1)
{
    map1[from] += 1;
}

// This function is called after the main function to add in the outdegrees from the first map.
void graphMapFunc(map<string, float> &map1, map<string, vector<pair<string, float>>> &graphMap)
{

    // this for loop iterates through the graph map
    for (const auto &it : graphMap)
    {
        // this global vector equals the value of graphMap.
        vec = it.second;
        // This for loop iterates through map1 for every key of graphMap.
        for (auto member : map1)
        {
            // Inside map1 there is another for loop in order to iterate through the vector to find every pairs outdegree value in map 1.
            for (int i = 0; i < vec.size(); i++)
            {
                if (vec[i].first == member.first)
                {
                    vec[i].second = (float)(1 / (float)(member.second));
                }
                graphMap[it.first] = vec;
            }
        }
    }
}

// This function grabs the rank from resMap if the following key is found.
float getRank(string matcher, map<string, float> &resMap)
{
    for (auto itr = resMap.find(matcher); itr != resMap.end(); itr++)
    {
        return resMap[matcher];
    }
}

// This function calculates the page ranks of the web pages.
map<string, float> pageRankFunc(map<string, vector<pair<string, float>>> &graphMap, map<string, float> &resMap)
{

    // a copy map of resMap so it doesn't alter the values for every power iteration.
    map<string, float> copyResMap = resMap;
    float tempRank;

    // iterate through resMap
    for (pair<string, float> mem : resMap)
    {
        tempRank = 0;
        // create a temp vector to grab the vector from the adjacencylist that corresponds to resMap.
        vector<pair<string, float>> tempVec = graphMap[mem.first];
        // iterate through the vector.
        for (int i = 0; i < tempVec.size(); ++i)
        {
            // create another temporary rank and call the get rank function on the ith pair of the vector.
            float initRank = getRank(tempVec.at(i).first, copyResMap);
            // calculate the rank by multiply the vector pair's second value by the pair's first value rank
            // and then add forever how many pairs are in that vector.
            tempRank += (tempVec.at(i).second) * initRank;
        }

        // After the vector is being done looped through, add the rank in resMap.
        resMap[mem.first] = (float)tempRank;
    }
    // return the map.
    return resMap;
}

// This function prints the webpages and their corresponding page ranks.
void printPageRanks(int p)
{
    // If there is only 1 power iteration then print the initial values
    // which is one over the map's size.
    if (p == 1)
    {
        for (auto member : graphMap)
        {
            resMap[member.first] += (1 / (float)graphMap.size());
        }
    }
    // else, then add in those initial values.
    else
    {
        for (auto member : graphMap)
        {
            resMap[member.first] += (1 / (float)graphMap.size());
        }
        // Then forever how many power iterations, call pageRankFunc to calculate those values.
        for (int i = 0; i < p - 1; i++)
        {
            pageRankFunc(graphMap, resMap);
        }
    }
    // finally, print out the resulting map with the page ranks.
    for (auto member : resMap)
    {
        cout << member.first << " ";
        cout << fixed << showpoint << std::setprecision(2) << member.second << "\n";
    }
}

int main()
{

    // read first number of lines (n) from cin
    int n;
    cin >> n;

    // read the number of power iterations (p) from cin
    int p;
    cin >> p;

    // for each of the next n lines, read in the vertices from and to
    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);
        istringstream in(line);

        // read in the source vertex
        string fromTemp;
        cin >> fromTemp;

        from = fromTemp;

        // read in the destination vertex
        string toTemp;
        cin >> toTemp;

        to = toTemp;

        map1Func(from, map1);

        // read in the values into the adjacency list map.
        graphMap[to].push_back(make_pair(from, 1 / float((map1[from]))));
        if (graphMap.find(from) == graphMap.end())
        {
            graphMap[from] = {};
        }
    }

    // call graphMapFunc
    graphMapFunc(map1, graphMap);

    // commented out to see the adjacency list's values.
    /*
        for (const auto &it: graphMap) {
            cout << it.first << " ";
            for (const auto &vec: it.second) {
                cout << "{" << vec.first << ", " << vec.second << "}" << ", ";
            }
            cout << endl;
        }
    */

    // call printPagerANKS
    printPageRanks(p);
}