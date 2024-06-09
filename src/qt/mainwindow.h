#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void openSignInWindow();
    void openSignUpWindow();

private:
    QPushButton *signInButton;
    QPushButton *signUpButton;
};

#endif // MAINWINDOW_H
