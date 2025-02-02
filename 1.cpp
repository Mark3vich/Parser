#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

#define FILENAME "index.txt"
#define NGRAM 3

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

    string *getNameAdress()
    {
        return &this->name;
    }

    void incrementCount(int incrementValue = 1)
    {
        count += incrementValue;
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

    void checkingForAnotherTag(const string &storage, int &index)
    {
        for (int i = index + 1; i < storage.size(); i++)
        {
            if (storage[i] == '>')
            {
                index = i;
            }

            if (storage[i] == '<' && storage[i + 1] == '/')
            {
                return;
            }
        }
    }

    bool checkingForTag(const string &storage)
    {
        bool isFlag = false;
        int numberOfTagsThatOpen = 0;
        int numberOfTagsThatClose = 0;
        bool wasThereClosingCharacter = false;
        for (size_t i = 0; i < storage.size(); i++)
        {
            if (storage[i] == '<')
            {
                numberOfTagsThatOpen += 1;
            }
            if (storage[i] == '>')
            {
                numberOfTagsThatClose += 1;
            }
            if (storage[i] == '/')
            {
                wasThereClosingCharacter = true;
            }
        }
        if (numberOfTagsThatOpen == numberOfTagsThatClose &&
            numberOfTagsThatOpen >= 2 && numberOfTagsThatClose >= 2 &&
            wasThereClosingCharacter)
        {
            isFlag = true;
        }
        return isFlag;
    }

public:
    map<string, value> parserFile(const vector<string> &storage)
    {
        bool found = false;
        for (int i = 0; i < storage.size(); i++)
        {
            for (int j = 0; j < storage[i].size(); j++)
            {
                // Проверка на наличие тега (фильтр тегов)
                if (!checkingForTag(storage[i]))
                {
                    break;
                }

                // Ищем открытие тега (нужен, чтобы избежать все лишние возле тега. Например - пробел)
                if (storage[i][j] == '<')
                {
                    // Имеется хотя бы одно слово
                    found = true;

                    // Ищем последний вложенный тег
                    checkingForAnotherTag(storage[i], j);

                    for (int z = j + 1; z < storage[i].size(); z++)
                    {
                        // Собираем информацию, пока не найдем открывающую скобку у закрывающего тега
                        if (storage[i][z] != '<')
                        {
                            key += storage[i][z];
                        }
                        // Если ключ не пуст и при этом мы нашли закрывающею скобку
                        if (key != "" && storage[i][z] == '<')
                        {
                            // Если результируйщий map был не пуст по переданному ключу, то мы прибавляем 1 к уже имеющемуся count
                            if (result[key].getName() != "")
                            {
                                count += 1;
                            }
                            else
                            {
                                count = 1;
                            }
                            // Сохраням результат через конструктор value и очищаем ключ
                            result[key] = value(count, key);
                            key = "";
                            break;
                        }
                    }
                    break;
                }
            }
        }
        if (!found)
        {
            cerr << "No tag found!" << endl;
        }

        return result;
    }
};

class MapProcessing
{
private:
    map<string, value> result;
    map<string, list<string>> story;
    vector<string> words;

    void printMap(map<string, value>::iterator it)
    {
        cout << "Key: " << it->first << "\n"
             << "Count: " << it->second.getCount() << "\n"
             << "Adress: " << it->second.getNameAdress() << "\n"
             << endl;
    }

    void convertKeysInList()
    {
        for (map<string, value>::iterator it = result.begin(); it != result.end(); ++it)
        {
            words.push_back(it->second.getName());
        }
    }

    void printList()
    {
        for (auto it = story.begin(); it != story.end(); it++)
        {
            cout << it->first << " -> ";
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                cout << *it2 << " ";
            }
            cout << endl;
        }
    }

    void addListElement(string word)
    {
        list<string> newList;
        newList.push_back(word);
        story[word] = newList;
    }

public:
    MapProcessing(map<string, value> stories)
    {
        this->result = stories;
        createNGram();
    }

    void getMap()
    {
        for (map<string, value>::iterator it = result.begin(); it != result.end(); ++it)
        {
            printMap(it);
        }
    }

    void search(string word)
    {
        bool isFlag = false;
        for (auto it = story.begin(); it != story.end(); it++)
        {
            if (it->first == word)
            {
                isFlag = true;
                cout << "Result: " << it->first << endl;
            }
        }
        if (!isFlag)
        {
            cout << "Not found" << endl;
        }
    }

    void createNGram()
    {
        convertKeysInList();
        for (auto word : words)
        {
            // Добавляем само слово в словарь
            addListElement(word);
            // Для слов, состоящих из одного или двух букв
            if (word.size() == 1 || word.size() == 2)
            {
                addListElement(word);
            }
            else
            {
                for (int i = 0; i <= word.size() - NGRAM; i++)
                {
                    string ngram = word.substr(i, NGRAM);
                    if (story.find(ngram) == story.end())
                    {
                        addListElement(word);
                    }
                    else
                    {
                        story[ngram].push_back(word);
                    }
                }
            }
        }
    }

    void mergingPairs(string key1, string key2)
    {
        if (result.find(key1) != result.end() && result.find(key2) != result.end())
        {
            result[key1].incrementCount(result[key2].getCount());
            result.erase(key2);
            cout << "Pair merged" << endl;
            return;
        }

        cout << "Keys not found!" << endl;
    }
};

int main()
{
    vector<string> storage;
    FileReader reader(FILENAME, storage);

    Parser parser = Parser();

    MapProcessing processor = MapProcessing(parser.parserFile(storage));

    processor.getMap();

    processor.search("Сидоров");
    processor.search("9");
    processor.search("Сло");

    processor.mergingPairs("Сидоров", "Слово 2");
    processor.mergingPairs("Слово 2", "Слово 3");

    return 0;
}
