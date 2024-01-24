#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "HashMap.h"
#include <QFile>
#include <QTextStream>
#include <QPropertyAnimation>
#include <string>
#include <QString>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDateTime>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QSpinBox>
#include <QDialogButtonBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), todoList(100), file((QApplication::applicationDirPath()) + "/To-Do List.txt")
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->centralwidget);
    todoList.readFromFile(file);
    ui->dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
    originalWidget = ui->scrollArea;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearLayout(QLayout *layout)
{
    if (!layout)
        return;

    while (QLayoutItem *item = layout->takeAt(0)) {
        QLayout *childLayout = item->layout();
        if (childLayout)
            clearLayout(childLayout);

        QWidget *childWidget = item->widget();
        if (childWidget)
            delete childWidget;

        delete item;
    }
}

void MainWindow::readFromFile(QFile& file)
{
    clearLayout(ui->formLayout);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    QTextStream in(&file);
    QString line, item, date;
    bool status = true;
    QStringList stringList;
    int priority;

    while (!in.atEnd())
    {
        line = in.readLine();
        stringList = line.split(',');

        if (stringList.size() >= 2)
        {
            item = stringList.at(0);
            priority = stringList.at(1).toInt();
            date = stringList.at(2);


            QDateTime dateTime = QDateTime::fromString(date, "d/M/yyyy h:mm AP");

            if (stringList.at(3).contains("Not"))
            {
                status = false;
            }
            else
            {
                status=true;
            }

            qDebug() << "Inserting item:" << item << " priority:" << priority << " date:" << dateTime.toString("dd/M/yyyy h:mm AP");

            QHBoxLayout* containerLayout = new QHBoxLayout();
            QCheckBox* checkbox = new QCheckBox();
            checkbox->setChecked(status);
            qDebug() << "Setting checkbox state to:" << status;
            connect(checkbox, &QCheckBox::stateChanged, this, &MainWindow::handleCheckBoxChanged);


            QLineEdit* lineedit = new QLineEdit(item);
            lineedit->setReadOnly(true);
            QPushButton* editButton = new QPushButton();
            QPushButton* deleteButton = new QPushButton();

            QString imagePath2 = ":/pics/edit.png";
            QIcon icon = QIcon(imagePath2);
            editButton->setIcon(icon);

            QString imagePath = ":/pics/trash.png";
            QIcon icon1 = QIcon::fromTheme(imagePath);
            deleteButton->setIcon(icon1);

            QLabel* label = new QLabel(dateTime.toString("d/M/yyyy h:mm AP"));

            containerLayout->addWidget(checkbox);
            containerLayout->addWidget(lineedit);
            containerLayout->addWidget(label);
            containerLayout->addWidget(editButton);
            containerLayout->addWidget(deleteButton);

            QWidget* containerWidget = new QWidget();
            containerWidget->setLayout(containerLayout);
            containerWidget->setProperty("key", QVariant(item));
            containerWidget->setProperty("value", QVariant(priority));

            connect(deleteButton, SIGNAL(clicked()), this, SLOT(handleDeleteButton()));
            connect(editButton, SIGNAL(clicked()), this, SLOT(handleEditButton()));

            ui->formLayout->addWidget(containerWidget);
        }
    }

    file.close();
}


void MainWindow::on_pushButton_clicked()
{
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    QString newItemText = ui->lineEdit->text();
    if (newItemText.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Item text cannot be empty."));
        return;
    }

    int priority = ui->comboBox->currentText().toInt();
    QString key = newItemText;
    QString date = ui->dateTimeEdit->dateTime().toString("d/M/yyyy h:mm AP");

    if (todoList.Exists(key.toStdString(), priority, date.toStdString()))
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Duplicate Item",
                                      "An item with the same name, priority, and date exists, are you sure you want to proceed?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No)
        {
            return;
        }
    }
    todoList.insert(ui->lineEdit->text().toStdString(), ui->comboBox->currentText().toInt(), ui->dateTimeEdit->dateTime().toString("d/M/yyyy h:mm AP").toStdString());
    readFromFile(file);
    disconnect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    ui->lineEdit->setText("");
    ui->comboBox->setCurrentText("1");
    ui->dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());
}

void MainWindow::handleDeleteButton()
{
    QPushButton* deleteButton = qobject_cast<QPushButton*>(sender());

    if (deleteButton)
    {
        QWidget* containerWidget = deleteButton->parentWidget();
        QString key = containerWidget->property("key").toString();
        int value = containerWidget->property("value").toInt();
        QString date = containerWidget->findChild<QLabel*>()->text();
        ui->formLayout->removeWidget(containerWidget);
        containerWidget->deleteLater();
        todoList.Remove(key.toStdString(), value, date.toStdString());
        todoList.writeToFile(file);
    }
}


void MainWindow::handleEditButton()
{
    QPushButton* editButton = qobject_cast<QPushButton*>(sender());

    if (editButton)
    {
        QWidget* containerWidget = editButton->parentWidget();
        QString key = containerWidget->property("key").toString();
        int value = containerWidget->property("value").toInt();
        QString date = containerWidget->findChild<QLabel*>()->text();
        QDialog editDialog(this);
        editDialog.setWindowTitle(tr("Edit Task"));
        QLineEdit* itemTextLineEdit = new QLineEdit(key);
        QDateTimeEdit* dateTimeEdit = new QDateTimeEdit(QDateTime::fromString(date, "d/M/yyyy h:mm AP"));
        dateTimeEdit->setCalendarPopup(true);
        dateTimeEdit->setMinimumDateTime(QDateTime::fromString(date,"d/M/yyyy h:mm AP"));
        QSpinBox* prioritySpinBox = new QSpinBox;
        prioritySpinBox->setRange(1, 5);
        prioritySpinBox->setValue(value);
        QFormLayout formLayout(&editDialog);
        formLayout.addRow(tr("Item Text:"), itemTextLineEdit);
        formLayout.addRow(tr("Date and Time:"), dateTimeEdit);
        formLayout.addRow(tr("Priority:"), prioritySpinBox);
        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &editDialog);
        formLayout.addRow(&buttonBox);
        connect(&buttonBox, &QDialogButtonBox::accepted, &editDialog, &QDialog::accept);
        connect(&buttonBox, &QDialogButtonBox::rejected, &editDialog, &QDialog::reject);
        if (editDialog.exec() == QDialog::Accepted)
        {
            QString newItemText = itemTextLineEdit->text();
            QDateTime newDateTime = dateTimeEdit->dateTime();
            int newPriority = prioritySpinBox->value();
            containerWidget->findChild<QLineEdit*>()->setText(newItemText);
            containerWidget->findChild<QLabel*>()->setText(newDateTime.toString("d/M/yyyy h:mm AP"));
            containerWidget->setProperty("value", QVariant(newPriority));
            todoList.Edit(key.toStdString(), value, date.toStdString(), newPriority, newItemText.toStdString(), newDateTime.toString("d/M/yyyy h:mm AP").toStdString());
            readFromFile(file);
        }
    }
}


void MainWindow::handleCheckBoxChanged(int state)
{
    QCheckBox* checkBox = qobject_cast<QCheckBox*>(sender());
    if (checkBox)
    {
        QWidget* containerWidget = checkBox->parentWidget();
        QString key = containerWidget->property("key").toString();
        int value = containerWidget->property("value").toInt();
        bool isChecked = (state == Qt::Checked);

        if (isChecked)
        {
            todoList.markAsDone(key.toStdString(), value);
        }
        else
        {
            todoList.uncheck(key.toStdString(), value);
        }

        //file.resize(0);
        todoList.writeToFile(file);

    }
}

void MainWindow::on_deletee_clicked()
{
    qDebug() << "Delete Entire List button clicked";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Entire List", "Are you sure you want to delete the entire list?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        qDebug() << "User clicked Yes";
        file.resize(0);
        todoList.deleteEntireList();
        clearLayout(ui->formLayout);
        qDebug() << "Entire list deleted.";
    } else {
        qDebug() << "User clicked No or closed the dialog";
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    bool hasCheckedItem = false;

    for (int i = 0; i < ui->formLayout->rowCount(); ++i)
    {
        QLayoutItem* item = ui->formLayout->itemAt(i);
        if (item)
        {
            QWidget* widget = item->widget();
            if (widget && widget->metaObject()->className() == QString("QWidget"))
            {
                bool isChecked = widget->findChild<QCheckBox*>()->isChecked();
                if (isChecked)
                {
                    hasCheckedItem = true;
                    QString key = widget->property("key").toString();
                    int value = widget->property("value").toInt();
                    QString date = widget->findChild<QLabel*>()->text();
                    ui->formLayout->removeWidget(widget);
                    widget->deleteLater();
                    // todoList.Remove(key.toStdString(), value, date.toStdString());
                    --i;
                }
            }
        }
    }
    if (!hasCheckedItem)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No items are marked as done."), QMessageBox::Ok);
    }
    todoList.clearMarkedAsDone();
}

void MainWindow::on_pushButton_5_clicked()
{
    for (int i = 0; i < ui->formLayout->rowCount(); ++i)
    {
        QLayoutItem* item = ui->formLayout->itemAt(i);

        if (item)
        {
            QWidget* widget = item->widget();

            if (widget && widget->metaObject()->className() == QString("QWidget"))
            {
                QCheckBox* checkBox = widget->findChild<QCheckBox*>();

                if (checkBox)
                {
                    checkBox->setChecked(true);
                    handleCheckBoxChanged(Qt::Checked);
                }
            }
        }

    }

}

void MainWindow::on_searchButton_clicked()
{
    QString searchText = ui->lineEdit_4->text();
    string searchStdString = searchText.toStdString();
    qDebug() << "search button clicked! Searching for:" << searchText;
    if (searchText.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Search text cannot be empty."));
        return;
    }
    resetFormVisibility();
    QStringList matchingResults = todoList.Search(searchStdString);
    for (int i = 0; i < ui->formLayout->count(); ++i) {
        QLayoutItem* item = ui->formLayout->itemAt(i);
        QWidget* containerWidget = item->widget();
        if (containerWidget) {
            QLineEdit* lineEdit = containerWidget->findChild<QLineEdit*>();
            if (lineEdit) {
                QString widgetText = lineEdit->text();

                if (matchingResults.contains(widgetText, Qt::CaseInsensitive)) {
                    containerWidget->setVisible(true);
                } else {
                    containerWidget->setVisible(false);
                }
            }
        }
    }

    for (int i = 0; i < ui->formLayout->count(); ++i) {
        QLayoutItem* item = ui->formLayout->itemAt(i);
        QWidget* containerWidget = item->widget();
        if (containerWidget) {
            QPushButton* editPushButton = containerWidget->findChild<QPushButton*>("editButton");
            if (editPushButton) {
                connect(editPushButton, &QPushButton::clicked, this, &MainWindow::handleEditButton);
            }

            QPushButton* deletePushButton = containerWidget->findChild<QPushButton*>("deleteButton");
            if (deletePushButton) {
                connect(deletePushButton, &QPushButton::clicked, this, &MainWindow::handleDeleteButton);
            }
        }
    }
}

void MainWindow::resetFormVisibility()
{
    for (int i = 0; i < ui->formLayout->count(); ++i) {
        QLayoutItem* item = ui->formLayout->itemAt(i);
        QWidget* containerWidget = item->widget();
        if (containerWidget) {
            containerWidget->setVisible(true);
        }
    }
}

void MainWindow::on_resetButton_clicked()
{
    resetFormVisibility();
}


