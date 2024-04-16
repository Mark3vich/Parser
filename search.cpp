#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>

using namespace std;

#define NGRAM 3 

void CreateNGram(vector<string>& v, map<string, list<string>>& story) {
    for(auto word : v) {
        for(size_t i = 0; i <= word.size() - NGRAM; i++) {
            string ngram = word.substr(i, NGRAM);
            if(story.find(ngram) == story.end()) {
                list<string> newList;
                newList.push_back(word);
                story[ngram] = newList;
            } else {
                story[ngram].push_back(word);
            }
        }
    }    
}

void SearchNGram(string word, map<string, list<string>>& story) {
    if(story.find(word) == story.end()) {
        cout << "Not found" << endl;
    } else {
        for(auto it = story[word].begin(); it != story[word].end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    }
}

void print(map<string, list<string>>& story) { 
    for(auto it = story.begin(); it != story.end(); it++) {
        cout << it->first << " -> ";
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            cout << *it2 << " ";
        }
        cout << endl;
    }
}

int main(int argc, char const *argv[])
{
    vector<string> v = {"hello", "world", "hel", "9"};

    map<string, list<string>> story;
    CreateNGram(v, story);
    SearchNGram("9", story);

    return 0;
}
