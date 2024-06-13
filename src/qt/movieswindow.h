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
#include <QNetworkAccessManager>
#include <vector>
#include <memory>
#include "userprofilewindow.h"
#include "genrewindow.h"
#include "moviedetailwindow.h"
#include "searchwindow.h"
#include "backend.h"

class MoviesWindow : public QWidget {
Q_OBJECT

public:
    MoviesWindow(QWidget *parent = nullptr);
    ~MoviesWindow();

private slots:
    void onUserProfileButtonClicked();
    void onMovieButtonClicked(const QSharedPointer<Movie>& movie);
    void onGenreItemClicked(QListWidgetItem *item);
    void onSearchButtonClicked();
    void showMoviesWindow();

private:
    QLineEdit *searchBar;
    QPushButton *searchButton;
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
    QNetworkAccessManager *networkManager;

    GenreWindow *genreWindow;
    MovieDetailWindow *movieDetailWindow;
    SearchWindow *searchWindow;

    void loadGenres();
    std::vector<std::string> fetchGenres();
    void addMovieToLayout(const QSharedPointer<Movie>& movie, QHBoxLayout* layout);
};

#endif // MOVIESWINDOW_H
