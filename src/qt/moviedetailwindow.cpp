#include "moviedetailwindow.h"
#include "mysql-queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QCloseEvent>
#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QMessageBox>

MovieDetailWindow::MovieDetailWindow(const QSharedPointer<Movie>& mov, QWidget *parent)
        : QWidget(parent), movie(mov), closed(false), userHasRated(false), userRating(0) {
    movie->loadActors();

    networkManager = new QNetworkAccessManager(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    titleLabel = new QLabel(QString::fromStdString(movie->getName()), this);
    titleLabel->setAlignment(Qt::AlignLeft);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleLabel->setFont(titleFont);
    topLayout->addWidget(titleLabel);

    watchLaterButton = new QPushButton("Watch Later", this);
    connect(watchLaterButton, &QPushButton::clicked, this, &MovieDetailWindow::onWatchLaterButtonClicked);
    topLayout->addWidget(watchLaterButton);

    likeMovieButton = new QPushButton("Like Movie", this);
    connect(likeMovieButton, &QPushButton::clicked, this, &MovieDetailWindow::onLikeMovieButtonClicked);
    topLayout->addWidget(likeMovieButton);

    addToCollectionButton = new QPushButton("Add to Collection", this);
    connect(addToCollectionButton, &QPushButton::clicked, this, &MovieDetailWindow::onAddToCollectionButtonClicked);
    topLayout->addWidget(addToCollectionButton);

    mainLayout->addLayout(topLayout);

    QHBoxLayout *middleLayout = new QHBoxLayout();
    movieImageLabel = new QLabel(this);
    QUrl imageUrl(QString::fromStdString(movie->getPhoto()));
    QNetworkRequest request(imageUrl);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MovieDetailWindow::onImageDownloaded);
    networkManager->get(request);

    middleLayout->addWidget(movieImageLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    std::cout << "directors " << movie->getActors()["director"].size() << '\n';
    if (!movie->getActors()["director"].empty()) {
        directorLabel = new QLabel(QString::fromStdString("Director: " + movie->getActors()["director"][0]->getName()), this);
    } else {
        directorLabel = new QLabel(QString("Director: "), this);
    }

    std::cout << "Actors " << movie->getActors()["actor"].size() << '\n';
    if (!movie->getActors()["actor"].empty()) {
        QString buf = "Actors: ";
        for (auto el: movie->getActors()["actor"]) {
            buf += QString::fromStdString(el->getName()) + ", ";
        }
        buf.chop(2);

        actorsLabel = new QLabel(buf, this);
        actorsLabel->setWordWrap(true);
        actorsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    } else {
        actorsLabel = new QLabel(QString("Actors: "), this);
        actorsLabel->setWordWrap(true);
        actorsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    infoLayout->addWidget(directorLabel);
    infoLayout->addWidget(actorsLabel);
    middleLayout->addLayout(infoLayout);

    mainLayout->addLayout(middleLayout);

    QVBoxLayout *ratingInfoLayout = new QVBoxLayout();
    ratingLabel = new QLabel(QString::fromStdString(std::to_string(movie->getRating())), this);
    votesLabel = new QLabel(QString::fromStdString(std::to_string(movie->getVotes())), this);
    ratingInfoLayout->addWidget(ratingLabel);
    ratingInfoLayout->addWidget(votesLabel);
    mainLayout->addLayout(ratingInfoLayout);

    QHBoxLayout *rateAndUserRatingLayout = new QHBoxLayout();
    QLabel *rateLabel = new QLabel("Rate the film:", this);
    rateAndUserRatingLayout->addWidget(rateLabel);

    userRatingLabel = new QLabel("Your rating:", this);
    userRatingLabel->setVisible(false);
    rateAndUserRatingLayout->addWidget(userRatingLabel);

    mainLayout->addLayout(rateAndUserRatingLayout);

    QHBoxLayout *ratingLayout = new QHBoxLayout();
    QGridLayout *ratingButtonsLayout = new QGridLayout();
    for (int i = 10; i >= 1; --i) {
        QPushButton *ratingButton = new QPushButton(QString::number(i), this);
        connect(ratingButton, &QPushButton::clicked, [this, i]() {
            updateUserRating(i);
            qDebug() << "User rated the film:" << i;
        });
        ratingButtonsLayout->addWidget(ratingButton, (10 - i) / 5, (10 - i) % 5);
    }
    ratingLayout->addLayout(ratingButtonsLayout);

    mainLayout->addLayout(ratingLayout);

    commentsArea = new QScrollArea(this);
    commentsArea->setWidgetResizable(true);
    QWidget *commentsContainer = new QWidget();
    QVBoxLayout *commentsLayout = new QVBoxLayout(commentsContainer);
    commentsContainer->setLayout(commentsLayout);
    commentsArea->setWidget(commentsContainer);
    mainLayout->addWidget(commentsArea);

    addComment("Great movie!", "User123");
    addComment("Really enjoyed it.", "User456");

    QHBoxLayout *addCommentLayout = new QHBoxLayout();
    commentLineEdit = new QLineEdit(this);
    addCommentButton = new QPushButton("Add Comment", this);
    connect(addCommentButton, &QPushButton::clicked, this, &MovieDetailWindow::onAddCommentButtonClicked);

    addCommentLayout->addWidget(commentLineEdit);
    addCommentLayout->addWidget(addCommentButton);

    mainLayout->addLayout(addCommentLayout);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &MovieDetailWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);
    setWindowTitle("Movie Details");
    setFixedSize(800, 600);
}

void MovieDetailWindow::onBackButtonClicked() {
    emit backToPreviousWindow();
    this->deleteLater();
}

void MovieDetailWindow::onAddToCollectionButtonClicked() {
    showCollectionDialog();
}

void MovieDetailWindow::showCollectionDialog() {
    bool ok;
    QStringList collections;
    collections << "Collection 1" << "Collection 2" << "Collection 3";
    QString collection = QInputDialog::getItem(this, "Select Collection", "Add to collection:", collections, 0, false, &ok);
    if (ok && !collection.isEmpty()) {
        qDebug() << "Added to collection:" << collection;
    }
}

void MovieDetailWindow::onWatchLaterButtonClicked() {
    if (!main_user->addToCol("Watch later", movie)) {
        QMessageBox::warning(this, "Warning", "This movie is already in your Watch Later collection.");
    } else {
        qDebug() << "Added to Watch Later";
    }
}

void MovieDetailWindow::onLikeMovieButtonClicked() {
    if (!main_user->addToCol("Liked", movie)) {
        QMessageBox::warning(this, "Warning", "This movie is already in your Liked collection.");
    } else {
        qDebug() << "Liked the movie";
    }
}

void MovieDetailWindow::onAddCommentButtonClicked() {
    QString comment = commentLineEdit->text();
    if (!comment.isEmpty()) {
        QString userId = "User123";
        addComment(comment, userId);
        commentLineEdit->clear();
        qDebug() << "Added comment:" << comment;
    }
}

void MovieDetailWindow::onImageDownloaded(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(imageData);
        movieImageLabel->setPixmap(pixmap.scaled(200, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "Error downloading image:" << reply->errorString();
    }
    reply->deleteLater();
}

void MovieDetailWindow::addComment(const QString &commentText, const QString &userId) {
    QWidget *commentWidget = new QWidget();
    QHBoxLayout *commentLayout = new QHBoxLayout(commentWidget);

    QLabel *commentLabel = new QLabel(commentText, commentWidget);
    commentLabel->setWordWrap(true);

    QLabel *userIdLabel = new QLabel(userId, commentWidget);
    userIdLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    commentLayout->addWidget(commentLabel);
    commentLayout->addWidget(userIdLabel);

    QVBoxLayout *commentsLayout = qobject_cast<QVBoxLayout *>(commentsArea->widget()->layout());
    if (commentsLayout) {
        commentsLayout->addWidget(commentWidget);
    }
}

void MovieDetailWindow::updateUserRating(int rating) {
    if (userHasRated) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Change Rating", "Are you sure you want to change your rating?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }

        main_user->makeVote(movie, rating, false, userRating);
        userRating = rating;
    } else {
        userHasRated = true;

        main_user->makeVote(movie, rating);
        userRating = rating;
    }

    userRatingLabel->setText(QString("Your rating: %1").arg(userRating));
    userRatingLabel->setVisible(true);

    ratingLabel->setText(QString::number(movie->getRating()));
    votesLabel->setText(QString::number(movie->getVotes()));

    qDebug() << "User rated the film:" << rating;
}

bool MovieDetailWindow::isClosed() const {
    return closed;
}

void MovieDetailWindow::closeEvent(QCloseEvent *event) {
    closed = true;
    QWidget::closeEvent(event);
}
