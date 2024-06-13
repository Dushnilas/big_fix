#include "genrewindow.h"
#include <QFont>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QLabel>
#include <QPushButton>
#include "moviedetailwindow.h"
#include "backend.h"

GenreWindow::GenreWindow(const QString &genre, QWidget *parent)
        : QWidget(parent), genre(genre), userProfileWindow(nullptr) {
    networkManager = new QNetworkAccessManager(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout();

    QLabel *titleLabel = new QLabel(QString("Films with genre: %1").arg(genre), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    topBarLayout->addWidget(titleLabel, 1);

    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search...");
    searchBar->setFixedHeight(30);
    topBarLayout->addWidget(searchBar);

    searchButton = new QPushButton("Search", this);
    connect(searchButton, &QPushButton::clicked, this, &GenreWindow::onSearchButtonClicked);
    topBarLayout->addWidget(searchButton);

    mainLayout->addLayout(topBarLayout);

    moviesArea = new QScrollArea(this);
    moviesArea->setWidgetResizable(true);
    moviesContainer = new QWidget(moviesArea);
    QGridLayout *moviesLayout = new QGridLayout(moviesContainer);

    loadMoviesByGenre(genre);
    moviesContainer->setLayout(moviesLayout);
    moviesArea->setWidget(moviesContainer);
    mainLayout->addWidget(moviesArea);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &GenreWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    userProfileButton = new QPushButton("User Profile", this);
    connect(userProfileButton, &QPushButton::clicked, this, &GenreWindow::onUserProfileButtonClicked);
    mainLayout->addWidget(userProfileButton);

    setLayout(mainLayout);
    setWindowTitle(QString("Films with genre: %1").arg(genre));
    setFixedSize(800, 600);

    setStyleSheet(
        "GenreWindow {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    padding: 5px;"
        "}"
        );
    setFixedSize(1280, 720);
}

void GenreWindow::onBackButtonClicked() {
    emit backToMoviesWindow();
    this->hide();
    this->deleteLater();
}

void GenreWindow::onUserProfileButtonClicked() {
    if (!userProfileWindow) {
        userProfileWindow = new UserProfileWindow(this);
        connect(userProfileWindow, &UserProfileWindow::backToPreviousWindow, this, &GenreWindow::showGenreWindow);
    }
    userProfileWindow->show();
    this->hide();
}

void GenreWindow::showGenreWindow() {
    this->show();
    if (userProfileWindow) {
        userProfileWindow->hide();
    }
}

void GenreWindow::onSearchButtonClicked() {
    QString query = searchBar->text();
    searchMov(query);
}

void GenreWindow::loadMoviesByGenre(const QString &genre) {
    std::vector<QSharedPointer<Movie>> movies = getMoviesSorted(20, genre.toStdString());

    QGridLayout *moviesLayout = qobject_cast<QGridLayout *>(moviesContainer->layout());
    if (!moviesLayout) return;

    int row = 0;
    int col = 0;
    for (const auto &movie : movies) {
        addMovieToLayout(movie, moviesLayout, row, col);
        col++;
        if (col >= 4) {
            col = 0;
            row++;
        }
    }
}

void GenreWindow::addMovieToLayout(const QSharedPointer<Movie>& movie, QGridLayout* layout, int row, int col) {
    QPushButton *movieButton = new QPushButton(this);
    movieButton->setIconSize(QSize(150, 150));
    connect(movieButton, &QPushButton::clicked, [this, movie]() {
        MovieDetailWindow *movieDetailWindow = new MovieDetailWindow(movie);
        movieDetailWindow->show();
        this->hide();
        connect(movieDetailWindow, &MovieDetailWindow::backToPreviousWindow, this, &GenreWindow::show);
    });

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

    layout->addWidget(movieWidget, row, col);
}

void GenreWindow::searchMov(const QString &query) {
    std::vector<QSharedPointer<Movie>> searchResults;
    searchMovies(searchResults, query.toStdString(), 20, genre.toStdString());

    QGridLayout *moviesLayout = qobject_cast<QGridLayout *>(moviesContainer->layout());
    if (!moviesLayout) return;

    QLayoutItem *item;
    while ((item = moviesLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int row = 0;
    int col = 0;
    for (const auto &movie : searchResults) {
        addMovieToLayout(movie, moviesLayout, row, col);
        col++;
        if (col >= 4) {
            col = 0;
            row++;
        }
    }
}
