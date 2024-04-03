#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

#define FILENAME "index.txt"

class value
{
private:
    int count;
    string name;

public:
    value() : count(0), name("") {}

    value(int count, string name)
    {
        this->count = count;
        this->name = name;
    }

    int getCount()
    {
        return this->count;
    }

    string getName()
    {
        return this->name;
    }
};

class FileReader
{
protected:
    string line;
    ifstream in;

public:
    FileReader(const string &filename, vector<string> &storage)
    {
        in.open(filename);
        if (in.is_open())
        {
            while (getline(in, line))
            {
                storage.push_back(line);
                // cout << line << endl;
            }
        }
        in.close();
    }
};

class Parser
{
private:
    string key;
    map<string, value> result;
    int count = 0;

public:
    Parser(const vector<string> &storage)
    {
        bool found = false;
        for (int i = 0; i < storage.size(); i++)
        {
            for (int j = 0; j < storage[i].size(); j++)
            {
                if (storage[i].substr(j, 4) == "<h1>")
                {
                    found = true;
                    for (int z = j + 4; z < storage[i].size(); z++)
                    {
                        if (storage[i][z] != '<')
                        {
                            key += storage[i][z];
                        }
                        if (key != "" && storage[i][z] == '<')
                        {
                            if(result[key].getName() != "") {
                                count += 1;
                            } else { 
                                count = 1;
                            }
                            result[key] = value(count, key);
                            key = ""; // Сброс ключа после записи
                            break;
                        }
                    }
                }
            }
        }
        if (!found)
        {
            cerr << "No <h1> tag found!" << endl;
        }
    }

    void getMap()
    {
        for (map<string, value>::iterator it = result.begin(); it != result.end(); ++it)
        {
            cout << "Key: " << it->first << "\n"
                 << "Count: " << it->second.getCount() << "\n"
                 << "Name: " << it->second.getName() << "\n"
                 << endl;
        }
    }
};

int main()
{
    vector<string> storage;
    FileReader reader(FILENAME, storage);
    Parser parser(storage);
    parser.getMap();

    return 0;
}
