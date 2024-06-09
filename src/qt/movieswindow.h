#ifndef MOVIESWINDOW_H
#define MOVIESWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QLabel>
#include <vector>
#include <memory>
#include "userprofilewindow.h"
#include "genrewindow.h"
#include "moviedetailwindow.h"

class MoviesWindow : public QWidget {
    Q_OBJECT

public:
    MoviesWindow(QWidget *parent = nullptr);
    ~MoviesWindow();

    private slots:
        void onUserProfileButtonClicked();
    void onMovieButtonClicked(int movieId);
    void onGenreItemClicked(QListWidgetItem *item);
    void showMoviesWindow();

private:
    QLineEdit *searchBar;
    QPushButton *userProfileButton;
    QListWidget *genresList;
    QScrollArea *moviesArea1;
    QScrollArea *moviesArea2;
    QWidget *moviesContainer1;
    QWidget *moviesContainer2;
    UserProfileWindow *userProfileWindow;
    QLabel *recommendedLabel;
    QLabel *otherUsersLikeLabel;
    QLabel *logoLabel;

    GenreWindow *genreWindow;
    MovieDetailWindow *movieDetailWindow;

    void loadGenres();
    std::vector<std::string> fetchGenres();
};

#endif // MOVIESWINDOW_H
