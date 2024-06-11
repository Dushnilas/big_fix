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

    QLabel *dobLabel = new QLabel("Date of Birth:", this);
    dobDateEdit = new QDateEdit(this);
    dobDateEdit->setCalendarPopup(true);
    dobDateEdit->setDisplayFormat("yyyy-MM-dd");

    ageLabel = new QLabel("Age: ", this);
    ageLabel->setVisible(false);  // Initially hide the age label

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
    layout->addWidget(dobLabel);
    layout->addWidget(dobDateEdit);
    layout->addWidget(ageLabel);
    layout->addWidget(emailLabel);
    layout->addWidget(emailLineEdit);
    layout->addWidget(signUpButton);
    layout->addWidget(backButton);

    setLayout(layout);

    connect(signUpButton, &QPushButton::clicked, this, &SignUpWindow::handleSignUp);
    connect(dobDateEdit, &QDateEdit::dateChanged, this, &SignUpWindow::updateAge);
}

SignUpWindow::~SignUpWindow() {}

void SignUpWindow::handleSignUp() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    QString name = nameLineEdit->text();
    QDate dob = dobDateEdit->date();
    QString email = emailLineEdit->text();
    int age = calculateAge(dob);

    std::cout << "Sign up with username: " << username.toStdString()
              << ", password: " << password.toStdString()
              << ", name: " << name.toStdString()
              << ", date of birth: " << dob.toString().toStdString()
              << ", age: " << age
              << ", email: " << email.toStdString() << std::endl;

    if (SignUp(username.toStdString(), password.toStdString(), age)) {
        MoviesWindow *moviesWindow = new MoviesWindow();
        moviesWindow->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Sign Up Failed", "User with this login already exists.");
    }
}

void SignUpWindow::onBackButtonClicked() {
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
    this->close();
}

void SignUpWindow::updateAge(const QDate &date) {
    int age = calculateAge(date);
    ageLabel->setText("Age: " + QString::number(age));
    ageLabel->setVisible(true);  // Show the age label when the date is selected
}

int SignUpWindow::calculateAge(const QDate &date) {
    QDate currentDate = QDate::currentDate();
    int age = currentDate.year() - date.year();
    if (currentDate.month() < date.month() ||
        (currentDate.month() == date.month() && currentDate.day() < date.day())) {
        age--;
    }
    return age;
}
