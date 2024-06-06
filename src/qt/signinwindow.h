// src/qt/signinwindow.h
#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <iostream> // Для вывода в консоль

class SignInWindow : public QWidget {
    Q_OBJECT

public:
    SignInWindow(QWidget *parent = nullptr);
    ~SignInWindow();

    private slots:
        void handleLogin();

private:
    QLineEdit *loginLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;

    bool authenticate(const QString &login, const QString &password);
};

#endif // SIGNINWINDOW_H
