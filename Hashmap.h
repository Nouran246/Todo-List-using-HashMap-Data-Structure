#ifndef HASHMAP_H
#define HASHMAP_H
#include "LinkedList.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QFile>
using namespace std;

class MainWindow;

class HashMap {
public:

    HashMap(int maxSize = 50);

    HashMap(const HashMap& other);

    const HashMap& operator=(const HashMap& rightHandSide);

    ~HashMap();

    bool isEmpty() const;

    int hashFunction(string key) const;

    void insert(string key, int value, string date = "", bool status = false);

    void Remove(const string& key, const int& value, const string& date);

    void Edit(const string& oldkey, const int& oldvalue, const string& oldDate, int& newValue, const string& newKey, const string& newdate);

    QStringList Search(const string& key);

    bool Exists(const string& key, const int& value, const string& date = "");

    void display(ostream& out) const;

    void displayOneNode(ostream& out, string keyTo_display, int valueTo_display);

    void writeToFile(QFile& File) const;

    void readFromFile(QFile& file);

    void markAsDone(const string& key, const int& value);
    void uncheck(const string& key, const int& value);

    void clearMarkedAsDone();
    void deleteEntireList();
    QFile File;


private:
    LinkedList* map;
    int mapCapacity;
    int mapSize;
};
#endif // HASHMAP_H
