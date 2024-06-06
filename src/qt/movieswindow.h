#ifndef MOVIESWINDOW_H
#define MOVIESWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <vector>
#include <memory>
#include "userprofilewindow.h"

class MoviesWindow : public QWidget {
    Q_OBJECT

public:
    MoviesWindow(QWidget *parent = nullptr);
    ~MoviesWindow();

    private slots:
        void onUserProfileButtonClicked();

private:
    QPushButton *userProfileButton;
    QListWidget *genresList;
    QScrollArea *moviesArea1;
    QScrollArea *moviesArea2;
    QWidget *moviesContainer1;
    QWidget *moviesContainer2;
    UserProfileWindow *userProfileWindow;

    void loadGenres();
    std::vector<std::string> fetchGenres();
};

#endif // MOVIESWINDOW_H
