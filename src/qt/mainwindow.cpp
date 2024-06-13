#include "mainwindow.h"
#include "signinwindow.h"
#include "signupwindow.h"
#include <QApplication>

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
