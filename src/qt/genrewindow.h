#ifndef GENREWINDOW_H
#define GENREWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
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
    void onMovieButtonClicked(int movieId);

private:
    QLabel *label;
    QString genre;
    QPushButton *backButton;
    QPushButton *userProfileButton;
    QScrollArea *moviesArea;
    QWidget *moviesContainer;
    QVBoxLayout *layout;
    QGridLayout *moviesLayout;
    UserProfileWindow *userProfileWindow;

    void loadMovies();
};

#endif // GENREWINDOW_H
