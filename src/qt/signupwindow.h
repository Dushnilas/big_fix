#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "mainwindow.h"

class SignUpWindow : public QWidget {
    Q_OBJECT

public:
    SignUpWindow(QWidget *parent = nullptr);
    ~SignUpWindow();

    private slots:
        void handleSignUp();
    void onBackButtonClicked();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *ageLineEdit;
    QLineEdit *emailLineEdit;
    QPushButton *signUpButton;
    QPushButton *backButton;

};

#endif // SIGNUPWINDOW_H
