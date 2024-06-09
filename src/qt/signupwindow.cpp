#include "signupwindow.h"
#include "movieswindow.h"
#include "backend.h"
#include <QMessageBox>
#include <iostream>

SignUpWindow::SignUpWindow(QWidget *parent)
    : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *signUpLabel = new QLabel("Sign up!", this);
    signUpLabel->setAlignment(Qt::AlignCenter);
    QFont font = signUpLabel->font();
    font.setPointSize(24);
    signUpLabel->setFont(font);

    QLabel *usernameLabel = new QLabel("Username:", this);
    usernameLineEdit = new QLineEdit(this);

    QLabel *passwordLabel = new QLabel("Password:", this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    QLabel *nameLabel = new QLabel("Name:", this);
    nameLineEdit = new QLineEdit(this);

    QLabel *ageLabel = new QLabel("Age:", this);
    ageLineEdit = new QLineEdit(this);

    QLabel *emailLabel = new QLabel("Email:", this);
    emailLineEdit = new QLineEdit(this);

    signUpButton = new QPushButton("Sign Up", this);
    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &SignUpWindow::onBackButtonClicked);

    layout->addWidget(signUpLabel);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordLineEdit);
    layout->addSpacing(20);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(ageLabel);
    layout->addWidget(ageLineEdit);
    layout->addWidget(emailLabel);
    layout->addWidget(emailLineEdit);
    layout->addWidget(signUpButton);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(signUpButton, &QPushButton::clicked, this, &SignUpWindow::handleSignUp);
}

SignUpWindow::~SignUpWindow() {}

void SignUpWindow::handleSignUp() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString name = nameLineEdit->text();
    QString age = ageLineEdit->text();
    QString email = emailLineEdit->text();

    std::cout << "Sign up with username: " << username.toStdString()
              << ", password: " << password.toStdString()
              << ", name: " << name.toStdString()
              << ", age: " << age.toStdString()
              << ", email: " << email.toStdString() << std::endl;

    if (SignUp(username.toStdString(), password.toStdString())) {
        MoviesWindow *moviesWindow = new MoviesWindow();
        moviesWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Sign Up Failed", "User with this login already exists.");
    }
}

bool SignUpWindow::SignUp(const std::string& login, const std::string& password) {
    std::vector<std::map<std::string, std::string>> buf = ExecuteSelectQuery("library", "SELECT * FROM auth;");

    if (std::find_if(buf.begin(), buf.end(), [&](const auto& c) {
        return login == c.at("user_id"); }) == buf.end()) {

        std::vector<std::map<std::string, std::string>> data = {
                {{"user_id", login}, {"name", login}, {"age", "0"}, {"photo_url", ""}}};

        std::vector<std::map<std::string, std::string>> data2 = {
                {{"user_id", login}, {"pass", password}}};

        if (ExecuteInsertQuery("library", "insert", "user_profile", data) &&
            ExecuteInsertQuery("library", "insert", "auth", data2)) {

            Logger::getInstance().logInfo("User " + login + " signed up.");
            main_user = std::make_shared<User>(login, login, password, 0, "");
            main_user->loadCol();
            return true;
        }
    }

    Logger::getInstance().logError("User with this login already exists.");
    buf.clear();
    return false;
}

void SignUpWindow::onBackButtonClicked() {
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}
