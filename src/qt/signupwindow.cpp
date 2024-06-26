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
    ageLabel->setVisible(false);

    QLabel *emailLabel = new QLabel("Email:", this);
    emailLineEdit = new QLineEdit(this);

    QLabel *genderLabel = new QLabel("Gender:", this);
    genderComboBox = new QComboBox(this);
    for (const auto &gender : getAllGenders()) {
        genderComboBox->addItem(QString::fromStdString(gender));
    }

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
    layout->addWidget(genderLabel);
    layout->addWidget(genderComboBox);
    layout->addWidget(signUpButton);
    layout->addWidget(backButton);

    setLayout(layout);

    setStyleSheet(
        "SignUpWindow {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: 4px solid rgb(229, 217, 190);" //бежевый
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        );

    connect(signUpButton, &QPushButton::clicked, this, &SignUpWindow::handleSignUp);
    connect(dobDateEdit, &QDateEdit::dateChanged, this, &SignUpWindow::updateAge);
}

SignUpWindow::~SignUpWindow() {}

void SignUpWindow::handleSignUp() {
    std::string username = usernameLineEdit->text().toStdString();
    std::string password = passwordLineEdit->text().toStdString();
    std::string name = nameLineEdit->text().toStdString();
    QDate dob = dobDateEdit->date();
    std::string email = emailLineEdit->text().toStdString();
    std::string gender = genderComboBox->currentText().toStdString();
    int age = calculateAge(dob);

    std::cout << "Sign up with username: " << username
              << ", password: " << password
              << ", name: " << name
              << ", date of birth: " << dob.toString().toStdString()
              << ", age: " << age
              << ", email: " << email
              << ", gender: " << gender << std::endl;

    if (SignUp(username, password, name, age, email, gender)) {

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
    ageLabel->setVisible(true);
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
