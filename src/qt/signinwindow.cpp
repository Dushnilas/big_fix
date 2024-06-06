#include "signinwindow.h"
#include "backend.h"
#include "movieswindow.h"
#include <QMessageBox>
#include <iostream>

SignInWindow::SignInWindow(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *loginLabel = new QLabel("Login:", this);
    loginLineEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Log In", this);

    layout->addWidget(loginLabel);
    layout->addWidget(loginLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &SignInWindow::handleLogin);
}

SignInWindow::~SignInWindow() {}

void SignInWindow::handleLogin() {
    QString login = loginLineEdit->text();
    QString password = passwordLineEdit->text();

    if (authenticate(login, password)) {
        std::cout << "Login successful" << std::endl;
        MoviesWindow *moviesWindow = new MoviesWindow();
        moviesWindow->show();
        this->close();
    } else {
        std::cout << "Login failed" << std::endl;
        QMessageBox::warning(this, "Login Failed", "Wrong login or password");
    }
}

bool SignInWindow::authenticate(const QString &login, const QString &password) {
    return SignInFun(login.toStdString(), password.toStdString());
}
