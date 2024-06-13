#ifndef GENREWINDOW_H
#define GENREWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QLineEdit>
#include <vector>
#include <memory>
#include "backend.h"
#include "userprofilewindow.h"

class GenreWindow : public QWidget {
Q_OBJECT

public:
    explicit GenreWindow(const QString &genre, QWidget *parent = nullptr);

signals:
    void backToMoviesWindow();

private slots:
    void onBackButtonClicked();
    void onUserProfileButtonClicked();
    void showGenreWindow();
    void onSearchButtonClicked();

private:
    QLabel *label;
    QString genre;
    QPushButton *backButton;
    QPushButton *userProfileButton;
    QScrollArea *moviesArea;
    QWidget *moviesContainer;
    QNetworkAccessManager *networkManager;
    UserProfileWindow *userProfileWindow;
    QLineEdit *searchBar;
    QPushButton *searchButton;

    void loadMoviesByGenre(const QString &genre);
    void addMovieToLayout(const QSharedPointer<Movie>& movie, QGridLayout* layout, int row, int col);
    void searchMov(const QString &query);
};

#endif // GENREWINDOW_H
