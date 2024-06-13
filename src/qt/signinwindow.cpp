#include "signinwindow.h"
#include "backend.h"
#include <QMessageBox>
#include <QDebug>

SignInWindow::SignInWindow(QWidget *parent) : QWidget(parent), moviesWindow(nullptr) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameField = new QLineEdit(this);
    usernameField->setPlaceholderText("Username");

    passwordField = new QLineEdit(this);
    passwordField->setPlaceholderText("Password");
    passwordField->setEchoMode(QLineEdit::Password);

    signInButton = new QPushButton("Sign In", this);
    connect(signInButton, &QPushButton::clicked, this, &SignInWindow::onSignInButtonClicked);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &SignInWindow::onBackButtonClicked);

    layout->addWidget(usernameField);
    layout->addWidget(passwordField);
    layout->addWidget(signInButton);
    layout->addWidget(backButton);

    setLayout(layout);
    setWindowTitle("Sign In");
    setFixedSize(300, 180);
}

SignInWindow::~SignInWindow() {
    if (moviesWindow) {
        delete moviesWindow;
    }
}

void SignInWindow::onSignInButtonClicked() {
    QString username = usernameField->text();
    QString password = passwordField->text();

    std::string login = username.toStdString();
    std::string pass = password.toStdString();

    if (SignIn(login, pass)) {
        Logger::getInstance().logError("User " + login + " signed in.");
        qDebug() << "User signed in successfully." << main_user->getLogin();
        showMoviesWindow();
    } else {
        Logger::getInstance().logError("Wrong login or password");
        qDebug() << "Wrong username or password.";
        QMessageBox::warning(this, "Sign In Failed", "Wrong username or password. Please try again.");
    }
}

void SignInWindow::showMoviesWindow() {

    if (!moviesWindow) {
        qDebug() << "Creating MoviesWindow.";
        moviesWindow = new MoviesWindow();
    }
    qDebug() << "Showing MoviesWindow.";
    moviesWindow->show();
    this->hide();
}

void SignInWindow::onBackButtonClicked() {
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}