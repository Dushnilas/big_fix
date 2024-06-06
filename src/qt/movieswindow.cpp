#include "movieswindow.h"
#include <QLabel>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <iostream>
#include "backend.h"

MoviesWindow::MoviesWindow(QWidget *parent) : QWidget(parent), userProfileWindow(new UserProfileWindow(this)) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Logo
    QLabel *logoLabel = new QLabel("Logo", this);
    logoLabel->setAlignment(Qt::AlignCenter);

    // Genres list
    genresList = new QListWidget(this);
    genresList->setFixedWidth(150); // Adjust width for narrower sidebar
    loadGenres();

    // User Profile button
    userProfileButton = new QPushButton("User Profile", this);
    connect(userProfileButton, &QPushButton::clicked, this, &MoviesWindow::onUserProfileButtonClicked);

    // Movie display areas
    moviesArea1 = new QScrollArea(this);
    moviesArea1->setWidgetResizable(true);
    moviesContainer1 = new QWidget(moviesArea1);
    QHBoxLayout *moviesLayout1 = new QHBoxLayout(moviesContainer1);

    moviesArea2 = new QScrollArea(this);
    moviesArea2->setWidgetResizable(true);
    moviesContainer2 = new QWidget(moviesArea2);
    QHBoxLayout *moviesLayout2 = new QHBoxLayout(moviesContainer2);

    // Load film image
    QPixmap filmImage("/Users/maykorablina/Yandex.Disk.localized/CodingProjects/BITCH/src/qt/film_image.jpg");

    // Adding some example movies to the first row
    for (int i = 1; i <= 10; ++i) {
        QLabel *movieLabel = new QLabel(this);
        movieLabel->setAlignment(Qt::AlignCenter);
        movieLabel->setFixedSize(150, 150); // Placeholder for movie poster
        movieLabel->setPixmap(filmImage.scaled(movieLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QLabel *movieTitle = new QLabel(QString("Movie Title %1").arg(i), this);
        movieTitle->setAlignment(Qt::AlignCenter);

        QVBoxLayout *movieLayout = new QVBoxLayout();
        movieLayout->addWidget(movieLabel);
        movieLayout->addWidget(movieTitle);

        QWidget *movieWidget = new QWidget();
        movieWidget->setLayout(movieLayout);

        moviesLayout1->addWidget(movieWidget);
    }

    // Adding some example movies to the second row
    for (int i = 11; i <= 20; ++i) {
        QLabel *movieLabel = new QLabel(this);
        movieLabel->setAlignment(Qt::AlignCenter);
        movieLabel->setFixedSize(150, 150); // Placeholder for movie poster
        movieLabel->setPixmap(filmImage.scaled(movieLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        QLabel *movieTitle = new QLabel(QString("Movie Title %1").arg(i), this);
        movieTitle->setAlignment(Qt::AlignCenter);

        QVBoxLayout *movieLayout = new QVBoxLayout();
        movieLayout->addWidget(movieLabel);
        movieLayout->addWidget(movieTitle);

        QWidget *movieWidget = new QWidget();
        movieWidget->setLayout(movieLayout);

        moviesLayout2->addWidget(movieWidget);
    }

    moviesContainer1->setLayout(moviesLayout1);
    moviesArea1->setWidget(moviesContainer1);

    moviesContainer2->setLayout(moviesLayout2);
    moviesArea2->setWidget(moviesContainer2);

    // Layout for logo, genres list, and movies areas
    QHBoxLayout *contentLayout = new QHBoxLayout();
    QVBoxLayout *leftSidebarLayout = new QVBoxLayout();

    leftSidebarLayout->addWidget(logoLabel);
    leftSidebarLayout->addWidget(genresList);
    leftSidebarLayout->addStretch();

    QVBoxLayout *moviesLayout = new QVBoxLayout();
    moviesLayout->addWidget(moviesArea1);
    moviesLayout->addWidget(moviesArea2);

    contentLayout->addLayout(leftSidebarLayout);
    contentLayout->addLayout(moviesLayout);

    // Main layout
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(userProfileButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
    setWindowTitle("Movies Window");
    setMinimumSize(800, 600); // Adjust the window size as needed
}

MoviesWindow::~MoviesWindow() {
}

void MoviesWindow::onUserProfileButtonClicked() {
    userProfileWindow->show();
}

void MoviesWindow::loadGenres() {
    std::vector<std::string> genres = fetchGenres();
    for (const auto &genre : genres) {
        genresList->addItem(new QListWidgetItem(QString::fromStdString(genre)));
    }
}

std::vector<std::string> MoviesWindow::fetchGenres() {
    std::vector<std::string> genres;
    std::string query = "SELECT DISTINCT genre_name FROM genres ORDER BY genre_name;";
    auto results = ExecuteSelectQuery("library", query);

    for (const auto &row : results) {
        genres.push_back(row.at("genre_name"));
    }

    return genres;
}
