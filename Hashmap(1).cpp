#include <iostream>
#include "HashMap.h"
#include "LinkedList.h"
#include <fstream>
#include <cstring>
#include <string>
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QApplication>
using namespace std;

HashMap::HashMap(int maxSize)
    : mapCapacity(maxSize), mapSize(0), File((QApplication::applicationDirPath()) + "/To-Do List.txt")
{
    map = new LinkedList [mapCapacity];
    for (int i = 0; i < mapCapacity; i++)
    {
        map[i] = LinkedList();
    }
}
void HashMap::deleteEntireList()
{
    for (int i = 0; i < mapCapacity; i++) {
        map[i].~LinkedList();
    }
}


HashMap::~HashMap()
{
    for (int i = 0; i < mapCapacity; i++) {
        map[i].~LinkedList();
    }
    delete[] map;
}


const HashMap& HashMap::operator=(const HashMap& rhsMap)
{
    if (this != &rhsMap)
    {
        this->~HashMap();

        mapSize = rhsMap.mapSize;
        mapCapacity = rhsMap.mapCapacity;

        if (mapSize > 0)
        {
            map = new LinkedList [mapCapacity];

            for (size_t i = 0; i < mapCapacity; i++)
            {
                if (!rhsMap.map[i].Empty())
                {
                    map[i] = LinkedList(rhsMap.map[i]);
                }
            }
        }
    }
    return *this;
}

int HashMap::hashFunction(string key) const
{
    return (tolower(key.front()) + tolower(key.back())) % mapCapacity;
}

QStringList HashMap::Search(const string& key)
{
    int linkedListIndex = hashFunction(key);
    return map[linkedListIndex].Search(key);
}

bool HashMap::isEmpty() const
{
    return mapSize == 0;
}

void HashMap::insert(string key, int value, string date, bool status)
{
    int linkedListIndex = hashFunction(key);
    map[linkedListIndex].insert(key, value, date, status);
    mapSize++;
    qDebug()<<"inserted";
    writeToFile(File);
}

void HashMap::Remove(const string& key, const int& value, const string& date)
{
    int linkedListIndex = hashFunction(key);
    map[linkedListIndex].Remove(key, value, date);
    mapSize--;
    writeToFile(File);
}

void HashMap::Edit(const string& oldkey, const int& oldvalue, const string& oldDate, int& newValue, const string& newKey, const string& newdate)
{
    if (Exists(oldkey, oldvalue, oldDate))
    {
        Remove(oldkey, oldvalue, oldDate);
        insert(newKey, newValue, newdate);
        writeToFile(File);
    }
    else
    {
        cout << newKey << " with priority " << newValue << " does not exist\n";
        return;
    }
}


void HashMap::display(ostream& out) const
{
    string line;
    fstream File("To-Do List.txt", ios::in);
    if (!File.is_open())
    {
        cerr << "Error opening file." << endl;
    }
    while (getline(File, line))
    {
        out << line << endl;
    }
    File.close();
}

void HashMap::writeToFile(QFile& file) const
{

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Error opening file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    qDebug() << "else entered";
    for (int j = 5; j > 0; j--)
    {
        for (int i = 0; i < mapCapacity; i++)
        {
            map[i].Display(j, file);
        }
    }

    file.close();
}



void HashMap::readFromFile(QFile& file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line;
    string item, date;
    bool status = true;
    QStringList stringList;
    int priority;
    while (!in.atEnd())
    {
        line = in.readLine();
        stringList = line.split(',');
        if (stringList.size() >= 2)
        {
            item = stringList.at(0).toStdString();
            priority = stringList.at(1).toInt();
            date = stringList.at(2).toStdString();
            if(stringList.at(3).contains("Not"))
            {
                status = false;
            }
            insert(item, priority, date, status);
        }
    }
    file.close();
}

bool HashMap::Exists(const string& key, const int& value, const string& date)
{
    int linkedListIndex = hashFunction(key);
    return map[linkedListIndex].Exists(key, value, date);
}

void HashMap::markAsDone(const string& key, const int& value)
{
    int nodeIndex = hashFunction(key);
    map[nodeIndex].markAsDone(key, value);
}
void HashMap::uncheck(const string& key, const int& value)
{
    int nodeIndex = hashFunction(key);
    map[nodeIndex].uncheck(key, value);
}

void HashMap::clearMarkedAsDone() {
    LinkedList list;
    for (int i = 0; i < mapCapacity; i++) {
        map[i].clearMarkedAsDone();
    }
    writeToFile(File);
}
