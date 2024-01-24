#include "LinkedList.h"
#include <fstream>
#include <QTextStream>
#include <cstring>
#include <QDebug>
#include <iostream>
using namespace std;

//Constructor
LinkedList::LinkedList()
    : first(0), mySize(0)
{}

//Copy constructor
LinkedList::LinkedList(const LinkedList& origList)
    : first(0), mySize(origList.mySize)
{
    mySize = origList.mySize;
    first = 0;
    if (mySize == 0) return;
    NodePointer ptr, nextPtr;
    first = new Node(origList.first->key, origList.first->value, origList.first->Date, origList.first->Done);
    ptr = first;
    nextPtr = origList.first->next;

    while (nextPtr != NULL)
    {
        ptr->next = new Node(nextPtr->key, nextPtr->value, origList.first->Date, origList.first->Done);
        nextPtr = nextPtr->next;
        ptr = ptr->next;
    }
}

//DESTRUCTOR UPDATED
LinkedList::~LinkedList()
{
    NodePointer curr = first, temp;
    while (curr != nullptr)
    {
        temp = curr;
        curr = curr->next;
        delete temp;
    }
}

//EMPTY UPDATED
bool LinkedList::Empty()
{
    return mySize == 0;
}

//-- Definition of the assignment operator
const LinkedList& LinkedList::operator=(const LinkedList& rhsLinkedlist)
{
    mySize = rhsLinkedlist.mySize;
    first = 0;
    if (mySize == 0) return *this;
    if (this != &rhsLinkedlist)
    {
        this->~LinkedList();
        NodePointer ptr, nextPtr;
        first = new Node(rhsLinkedlist.first->key, rhsLinkedlist.first->value, rhsLinkedlist.first->Date, rhsLinkedlist.first->Done);
        ptr = first;
        nextPtr = rhsLinkedlist.first->next;

        while (nextPtr != NULL)
        {
            ptr->next = new Node(nextPtr->key, nextPtr->value, rhsLinkedlist.first->Date, rhsLinkedlist.first->Done);
            nextPtr = nextPtr->next;
            ptr = ptr->next;
        }
    }
    return *this;
}

//INSERT UPDATED
void LinkedList::insert(string dataKey, int dataVal, string dateValue, bool statusValue)
{
    NodePointer currentPtr = first;
    Node* newNodeptr = new Node(dataKey, dataVal, dateValue, statusValue);

    if (first == nullptr || first->value < dataVal)
    {
        newNodeptr->next = first;
        first = newNodeptr;
    }
    else
    {
        while (currentPtr->next != NULL && (currentPtr->value > dataVal || currentPtr->value == dataVal))
        {
            currentPtr = currentPtr->next;
        }
        currentPtr->next = newNodeptr;
    }
    mySize++;
}

//REMOVE UPDATED
void LinkedList::Remove(string dataKey, int dataVal, string date)
{
    if (!Empty())
    {
        Node* current = first;
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->key == dataKey && current->value == dataVal && current->Date == date) {
                if (previous == nullptr) {
                    first = current->next;
                }
                else {
                    previous->next = current->next;
                }
                delete current;
                return;
            }
            previous = current;
            current = current->next;
        }

        cout << "Item was not found :(" << endl;
    }
    else {
        cout << "List is empty." << endl;
    }
    mySize--;
}

string tolowercase(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
    return str;
}

QStringList LinkedList::Search(string dataKey)
{
    QStringList list;
    bool isFound = false;
    NodePointer CurrentPtr = first;

    string searchKeyLower = tolowercase(dataKey);

    while (CurrentPtr != nullptr)
    {
        string currentKeyLower = tolowercase(CurrentPtr->key);

        if (currentKeyLower == searchKeyLower)
        {
            list.append(QString::fromStdString(CurrentPtr->key));
            isFound = true;
        }
        CurrentPtr = CurrentPtr->next;
    }

    if (!isFound)
    {
        cout << "Item not found." << endl;
    }
    return list;
}

void LinkedList::displayOneNode(ostream& out, string keyTo_display, int valueTo_display) {

    NodePointer current = first;
    if (current == nullptr)  // if key is not found in the calculated index
    {
        out << "Not found " << keyTo_display << endl;
        return;

    }
    if (current->next == nullptr) {  //if there is only one node
        out << current->key << ", Priority: " << current->value << endl;
    }
    else {
        while (current != nullptr) //more than one node in one bucket
        {
            if (current->key == keyTo_display && current->value == valueTo_display)
            {
                out << current->key << ", Priority: " << current->value << endl;
                return;
            }
            current = current->next;
        }
    }
}

// DISPLAY UPDATED
void LinkedList::Display(int prioritylevel, QFile& File) const
{
    QTextStream out(&File);
    NodePointer Currentptr = first;
    while (Currentptr != nullptr)
    {
        if (Currentptr->value == prioritylevel)
        {
            qDebug() << "actual writing reached";
            out << QString::fromStdString(Currentptr->key) << ","
                << Currentptr->value << ","
                << QString::fromStdString(Currentptr->Date) << ","
                << (Currentptr->Done ? "Done" : "Not Done") << "\n";
        }
        Currentptr = Currentptr->next;
    }
}


bool LinkedList::Exists(const string& key, const int& value, const string& date)
{
    Node* CurrentPtr = first;

    while (CurrentPtr != NULL)
    {
        if (CurrentPtr->key == key && CurrentPtr->value == value && CurrentPtr->Date == date)
        {
            return true;
        }
        CurrentPtr = CurrentPtr->next;
    }
    return false;
}

void LinkedList::markAsDone(const string& key, const int& value)
{
    NodePointer CurrentPtr = first;
    while (CurrentPtr != nullptr)
    {
        if (CurrentPtr->key == key && CurrentPtr->value == value)
        {
            CurrentPtr->Done = true;
        }
        CurrentPtr = CurrentPtr->next;
    }
}
void LinkedList::uncheck(const string& key, const int& value)
{
    NodePointer CurrentPtr = first;
    while (CurrentPtr != nullptr)
    {
        if (CurrentPtr->key == key && CurrentPtr->value == value)
        {
            CurrentPtr->Done = false;
        }
        CurrentPtr = CurrentPtr->next;
    }
}
void LinkedList::clearMarkedAsDone() {
    NodePointer CurrentPtr = first, nextNode;
    while (CurrentPtr != nullptr)
    {
        if (CurrentPtr->Done == true)
        {
            nextNode = CurrentPtr->next;  // Save the next node before removing
            Remove(CurrentPtr->key, CurrentPtr->value, CurrentPtr->Date);
            CurrentPtr = nextNode;  // Update CurrentPtr to the saved next node
        }
        else {
            CurrentPtr = CurrentPtr->next;
        }
    }
}
