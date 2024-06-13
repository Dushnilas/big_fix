#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <memory>
#include "movieswindow.h"
#include "mainwindow.h"
#include <QLabel>

class SignInWindow : public QWidget {
    Q_OBJECT

public:
    explicit SignInWindow(QWidget *parent = nullptr);
    ~SignInWindow() override;

    private slots:
        void onSignInButtonClicked();
    void showMoviesWindow();
    void onBackButtonClicked();

private:
    QLineEdit *usernameField;
    QLineEdit *passwordField;
    QPushButton *signInButton;
    QPushButton *backButton;
    MoviesWindow *moviesWindow;
    QLabel *headtext;
};

#endif // SIGNINWINDOW_H
