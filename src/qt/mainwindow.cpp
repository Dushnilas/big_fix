#include "mainwindow.h"
#include "signinwindow.h"
#include "signupwindow.h"
#include <QApplication>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *welcomeLabel = new QLabel("WELCOME TO MASE!", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    QFont font = welcomeLabel->font();
    font.setPointSize(24);
    welcomeLabel->setFont(font);

    signInButton = new QPushButton("Sign In", this);
    signUpButton = new QPushButton("Sign Up", this);

    connect(signInButton, &QPushButton::clicked, this, &MainWindow::openSignInWindow);
    connect(signUpButton, &QPushButton::clicked, this, &MainWindow::openSignUpWindow);

    mainLayout->addWidget(welcomeLabel);
    mainLayout->addWidget(signInButton);
    mainLayout->addWidget(signUpButton);
    mainLayout->setAlignment(Qt::AlignCenter);

    setLayout(mainLayout);
    setWindowTitle("Main Window");
    setMinimumSize(400, 300);

    setStyleSheet(
        "MainWindow {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: 4px solid rgb(229, 217, 190);"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        );
}

MainWindow::~MainWindow() {}

void MainWindow::openSignInWindow() {
    SignInWindow *signInWindow = new SignInWindow();
    signInWindow->show();
    this->close();
}

void MainWindow::openSignUpWindow() {
    SignUpWindow *signUpWindow = new SignUpWindow();
    signUpWindow->show();
    this->close();
}
