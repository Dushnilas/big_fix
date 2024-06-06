#ifndef USERPROFILEWINDOW_H
#define USERPROFILEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

class UserProfileWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserProfileWindow(QWidget *parent = nullptr);
    ~UserProfileWindow();

private:
    QLabel *photoLabel;
    QLabel *userIdLabel;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *mailLabel;
    QPushButton *editButton1;
    QPushButton *editButton2;
    QScrollArea *collectionsArea;
    QWidget *collectionsContainer;
};

#endif // USERPROFILEWINDOW_H
