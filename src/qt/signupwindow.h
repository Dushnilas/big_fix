#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QDate>
#include "mainwindow.h"

class SignUpWindow : public QWidget {
    Q_OBJECT

public:
    SignUpWindow(QWidget *parent = nullptr);
    ~SignUpWindow();

    private slots:
        void handleSignUp();
    void onBackButtonClicked();
    void updateAge(const QDate &date);

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *nameLineEdit;
    QDateEdit *dobDateEdit;
    QLabel *ageLabel;
    QLineEdit *emailLineEdit;
    QPushButton *signUpButton;
    QPushButton *backButton;
    int calculateAge(const QDate &date);
};

#endif // SIGNUPWINDOW_H
