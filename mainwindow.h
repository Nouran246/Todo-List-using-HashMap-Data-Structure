#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include "HashMap.h"
#include <iostream>
#include <QFile>
using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readFromFile(QFile& file);
    void clearLayout(QLayout *layout);
    QWidget* originalWidget;



private slots:
    void on_pushButton_clicked();
    void handleDeleteButton();
    void handleEditButton();
    void handleCheckBoxChanged(int state);
    //void on_pushButton_2_clicked();
    void on_searchButton_clicked();
    void on_deletee_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void resetFormVisibility();
    void on_resetButton_clicked();

private:
    void displaySearchResults(const QString &searchText);

    // Add a private variable to store the original layout
    QLayout *originalLayout;
    Ui::MainWindow *ui;
    HashMap todoList;
    QFile file;

};
#endif // MAINWINDOW_H
