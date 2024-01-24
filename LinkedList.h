#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <string>
#include <fstream>
#include <QFile>
#include <QTextStream>
using namespace std;

class LinkedList
{

public:
    LinkedList();

    LinkedList(const LinkedList& origList);

    ~LinkedList();

    const LinkedList& operator=(const LinkedList& rightSide);

    bool Empty();

    void insert(string dataKey, int dataVal, string dateValue, bool statusVal = false);

    void Remove(string dataKey, int dataVal, string date);

    void Display(int prioritylevel, QFile& File) const;

    void displayOneNode(ostream& out, string keyTo_display, int valueTo_display);

    bool Exists(const string& key, const int& value, const string& date);

    void sortByPriority();

    QStringList Search(string dataKey);

    void markAsDone(const string& key, const int& value);
    void uncheck(const string& key, const int& value);

    void clearMarkedAsDone();


private:
    class Node {
    public:
        int value;
        string key;
        bool Done;
        string Date;
        Node* next;

        Node()
            : value(), key(), Done(), Date(), next(nullptr)
        {}

        Node(string keyValue, int dataValue, string dateValue)
            : value(dataValue), key(keyValue), Done(false), Date(dateValue), next(nullptr)
        {}

        Node(string keyValue, int dataValue, string dateValue, bool dataDone)
            : value(dataValue), key(keyValue), Done(dataDone), Date(dateValue), next(nullptr)
        {}
    };
    fstream File;
    typedef Node* NodePointer;
    NodePointer first;
    int mySize;
};


#endif // LINKEDLIST_H
