#include "genrewindow.h"
#include "moviedetailwindow.h"
#include "mysql-queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <QDebug>

GenreWindow::GenreWindow(const QString &genre, QWidget *parent) : QWidget(parent), genre(genre) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel(QString("Films with genre: %1").arg(genre), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);

    QGridLayout *gridLayout = new QGridLayout();

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            int movieId = i * 4 + j + 1;
            QPushButton *movieButton = new QPushButton(this);
            movieButton->setIcon(QIcon("/Users/maykorablina/Yandex.Disk.localized/CodingProjects/big_fix_3/src/qt/film_image.jpg"));
            movieButton->setIconSize(QSize(100, 150));
            connect(movieButton, &QPushButton::clicked, [this, movieId]() { onMovieButtonClicked(movieId); });
            gridLayout->addWidget(movieButton, i, j);
        }
    }

    QScrollArea *scrollArea = new QScrollArea(this);
    QWidget *scrollAreaContent = new QWidget(scrollArea);
    scrollAreaContent->setLayout(gridLayout);
    scrollArea->setWidget(scrollAreaContent);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &GenreWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    userProfileButton = new QPushButton("User Profile", this);
    connect(userProfileButton, &QPushButton::clicked, this, &GenreWindow::onUserProfileButtonClicked);
    mainLayout->addWidget(userProfileButton);

    setLayout(mainLayout);
    setWindowTitle(QString("Films with genre: %1").arg(genre));
    setFixedSize(800, 600);

    userProfileWindow = new UserProfileWindow(this);
}

void GenreWindow::onMovieButtonClicked(int movieId) {
    qDebug() << "Movie clicked:" << movieId;
    MovieDetailWindow *movieDetailWindow = new MovieDetailWindow(QString::number(movieId));
    connect(movieDetailWindow, &MovieDetailWindow::backToPreviousWindow, this, &GenreWindow::show);
    movieDetailWindow->show();
    this->hide();
}

void GenreWindow::onBackButtonClicked() {
    qDebug() << "Back button clicked.";
    emit backToMoviesWindow();
    this->hide();
}

void GenreWindow::onUserProfileButtonClicked() {
    userProfileWindow->show();
    this->hide();
}

void GenreWindow::loadMovies() {

}
