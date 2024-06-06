#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SignUpWindow : public QWidget {
    Q_OBJECT

public:
    SignUpWindow(QWidget *parent = nullptr);
    ~SignUpWindow();

    private slots:
        void handleSignUp();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QLineEdit *nameLineEdit;
    QLineEdit *ageLineEdit;
    QLineEdit *emailLineEdit;
    QPushButton *signUpButton;

    bool SignUp(const std::string& login, const std::string& password);
};

#endif // SIGNUPWINDOW_H
