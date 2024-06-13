#include "searchwindow.h"
#include <QFont>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkReply>
#include <QLabel>
#include <QPushButton>
#include "moviedetailwindow.h"

SearchWindow::SearchWindow(const QString &query, QWidget *parent)
        : QWidget(parent), query(query), networkManager(new QNetworkAccessManager(this)) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    qDebug() << "Initializing SearchWindow with query:" << query;

    QLabel *titleLabel = new QLabel(QString("Search results for: %1").arg(query), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);

    moviesArea = new QScrollArea(this);
    moviesArea->setWidgetResizable(true);
    moviesContainer = new QWidget(moviesArea);
    QGridLayout *moviesLayout = new QGridLayout(moviesContainer);

    qDebug() << "Loading movies for query:" << query;
    loadMoviesByQuery(query);
    moviesContainer->setLayout(moviesLayout);
    moviesArea->setWidget(moviesContainer);
    mainLayout->addWidget(moviesArea);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &SearchWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);
    setWindowTitle(QString("Search results for: %1").arg(query));

    setStyleSheet(
            "SearchWindow {"
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
            "QScrollArea {"
            "    background-color: rgba(255, 255, 255, 0);"
            "    color: rgb(229, 217, 190);"
            "    border: none;"
            "}"
            "QScrollArea QWidget {"
            "    background-color: rgba(255, 255, 255, 0);"
            "    color: rgb(229, 217, 190);"
            "}"
            "QListWidget {"
            "    background-color: rgb(229, 217, 190);"
            "    color: rgb(0, 0, 0);"
            "}"
    );
    setFixedSize(1280, 720);

    qDebug() << "SearchWindow created with query:" << query;
}


void SearchWindow::onBackButtonClicked() {
    emit backToMainWindow();
    this->hide();
    this->deleteLater();
}

void SearchWindow::loadMoviesByQuery(const QString &query) {
    std::vector<QSharedPointer<Movie>> searchResults;
    searchMovies(searchResults, query.toStdString(), 20);
    std::cout << "Search: " << searchResults.size() << '\n';

    QGridLayout *moviesLayout = qobject_cast<QGridLayout *>(moviesContainer->layout());
    if (!moviesLayout) return;

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

void SearchWindow::addMovieToLayout(const QSharedPointer<Movie>& movie, QGridLayout* layout, int row, int col) {
    QPushButton *movieButton = new QPushButton(this);
    movieButton->setIconSize(QSize(150, 150));
    connect(movieButton, &QPushButton::clicked, [this, movie]() {
        MovieDetailWindow *movieDetailWindow = new MovieDetailWindow(movie);
        movieDetailWindow->show();
        this->hide();
        connect(movieDetailWindow, &MovieDetailWindow::backToPreviousWindow, this, &SearchWindow::show);
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
