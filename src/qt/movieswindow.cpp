#include "movieswindow.h"
#include "genrewindow.h"
#include "moviedetailwindow.h"
#include "mysql-queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "backend.h"

MoviesWindow::MoviesWindow(QWidget *parent) : QWidget(parent), userProfileWindow(new UserProfileWindow(this)), genreWindow(nullptr), movieDetailWindow(nullptr) {
    networkManager = new QNetworkAccessManager(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout();
    logoLabel = new QLabel(this);
    QPixmap logoPixmap(qFilePath("/pictures/logo.jpg"));
    logoLabel->setPixmap(logoPixmap.scaled(150, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedSize(150, 50);

    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search...");
    searchBar->setFixedHeight(30);

    topBarLayout->addWidget(logoLabel);
    topBarLayout->addWidget(searchBar);
    mainLayout->addLayout(topBarLayout);

    genresList = new QListWidget(this);
    genresList->setFixedWidth(150);
    loadGenres();

    userProfileButton = new QPushButton("User Profile", this);
    connect(userProfileButton, &QPushButton::clicked, this, &MoviesWindow::onUserProfileButtonClicked);

    recommendedLabel = new QLabel("Recommended to watch", this);
    moviesArea1 = new QScrollArea(this);
    moviesArea1->setWidgetResizable(true);
    moviesContainer1 = new QWidget(moviesArea1);
    QHBoxLayout *moviesLayout1 = new QHBoxLayout(moviesContainer1);

    otherUsersLikeLabel = new QLabel("What other users like", this);
    moviesArea2 = new QScrollArea(this);
    moviesArea2->setWidgetResizable(true);
    moviesContainer2 = new QWidget(moviesArea2);
    QHBoxLayout *moviesLayout2 = new QHBoxLayout(moviesContainer2);

    std::vector<QSharedPointer<Movie>> movies_no_rec;
    std::vector<QSharedPointer<Movie>> movies_cb_rec;
    std::vector<QSharedPointer<Movie>> movies_user_rec;
    if (isLiked()) {
        std::vector<std::string> cb_rec = GetContentRecommendations(main_user->getLogin());
        std::vector<std::string> user_rec = GetUserRecommendations(main_user->getLogin());
        getRecommendation(movies_cb_rec, cb_rec);
        getRecommendation(movies_user_rec, user_rec);
    } else {
        movies_no_rec = getMoviesSorted(10);
    }

    if (!movies_no_rec.empty()) {
        for (const auto& movie : movies_no_rec) {
            addMovieToLayout(movie, moviesLayout1);
            addMovieToLayout(movie, moviesLayout2);
        }
    } else {
        for (const auto& movie : movies_cb_rec) {
            addMovieToLayout(movie, moviesLayout1);
        }
        for (const auto& movie : movies_user_rec) {
            addMovieToLayout(movie, moviesLayout2);
        }
    }

    moviesContainer1->setLayout(moviesLayout1);
    moviesArea1->setWidget(moviesContainer1);

    moviesContainer2->setLayout(moviesLayout2);
    moviesArea2->setWidget(moviesContainer2);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    QVBoxLayout *leftSidebarLayout = new QVBoxLayout();

    leftSidebarLayout->addWidget(genresList);
    leftSidebarLayout->addStretch();

    QVBoxLayout *moviesLayout = new QVBoxLayout();
    moviesLayout->addWidget(recommendedLabel);
    moviesLayout->addWidget(moviesArea1);
    moviesLayout->addWidget(otherUsersLikeLabel);
    moviesLayout->addWidget(moviesArea2);

    contentLayout->addLayout(leftSidebarLayout);
    contentLayout->addLayout(moviesLayout);

    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(userProfileButton, 0, Qt::AlignRight);

    setLayout(mainLayout);
    setWindowTitle("Movies Window");

    // Устанавливаем начальный размер окна
    setMinimumSize(1024, 768); // Минимальный размер окна
    resize(1280, 720);         // Начальный размер окна
}

MoviesWindow::~MoviesWindow() {
    delete userProfileWindow;
    if (genreWindow) {
        delete genreWindow;
    }
    if (movieDetailWindow) {
        delete movieDetailWindow;
    }
}

void MoviesWindow::onUserProfileButtonClicked() {
    userProfileWindow->show();
    this->hide();
}

void MoviesWindow::onMovieButtonClicked(const QSharedPointer<Movie>& movie) {
    qDebug() << "Movie clicked:" << movie->getName();

    movieDetailWindow = new MovieDetailWindow(movie);
    connect(movieDetailWindow, &MovieDetailWindow::backToPreviousWindow, this, &MoviesWindow::showMoviesWindow);
    movieDetailWindow->show();
    this->hide();
}

void MoviesWindow::showMoviesWindow() {
    qDebug() << "Showing MoviesWindow.";
    this->show();
    if (movieDetailWindow) {
        movieDetailWindow->hide();
    }
    if (genreWindow) {
        genreWindow->hide();
    }
}

void MoviesWindow::loadGenres() {
    std::vector<std::string> genres = fetchGenres();
    for (const auto &genre : genres) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(genre), genresList);
    }
    connect(genresList, &QListWidget::itemClicked, this, &MoviesWindow::onGenreItemClicked);
}


void MoviesWindow::onGenreItemClicked(QListWidgetItem *item) {
    QString genre = item->text();
    qDebug() << "Genre clicked:" << genre;

    genreWindow = new GenreWindow(genre);
    connect(genreWindow, &GenreWindow::backToMoviesWindow, this, &MoviesWindow::showMoviesWindow);
    genreWindow->show();
    this->hide();
}

std::vector<std::string> MoviesWindow::fetchGenres() {
    std::vector<std::string> genres;
    std::string query = "SELECT DISTINCT genre_name FROM genres WHERE genre_name != 'Adult' ORDER BY genre_name;";
    auto results = ExecuteSelectQuery("library", query);

    for (const auto &row : results) {
        genres.push_back(row.at("genre_name"));
    }

    return genres;
}

void MoviesWindow::addMovieToLayout(const QSharedPointer<Movie>& movie, QHBoxLayout* layout) {
    QPushButton *movieButton = new QPushButton(this);
    movieButton->setIconSize(QSize(150, 150));
    connect(movieButton, &QPushButton::clicked, [this, movie]() { onMovieButtonClicked(movie); });

    QUrl imageUrl(QString::fromStdString(movie->getPhoto()));
    QNetworkRequest request(imageUrl);
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [reply, movieButton]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            movieButton->setIcon(QIcon(pixmap));
        } else {
            qDebug() << "Error downloading image:" << reply->errorString();
        }
        reply->deleteLater();
    });

    QLabel *movieTitle = new QLabel(QString::fromStdString(movie->getName()), this);
    movieTitle->setAlignment(Qt::AlignCenter);

    QVBoxLayout *movieLayout = new QVBoxLayout();
    movieLayout->addWidget(movieButton);
    movieLayout->addWidget(movieTitle);

    QWidget *movieWidget = new QWidget();
    movieWidget->setLayout(movieLayout);

    layout->addWidget(movieWidget);
}
