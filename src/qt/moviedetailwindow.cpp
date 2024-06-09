#include "moviedetailwindow.h"
#include "mysql-queries.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QCloseEvent>
#include <QInputDialog>

MovieDetailWindow::MovieDetailWindow(const QString &movieId, QWidget *parent)
    : QWidget(parent), movieId(movieId), closed(false), userHasRated(false), userRating(0) { // Initialize closed and userHasRated here
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();
    titleLabel = new QLabel("Inception", this);
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
    QPixmap movieImagePixmap("/Users/maykorablina/Yandex.Disk.localized/CodingProjects/big_fix_3/src/qt/film_image.jpg");
    movieImageLabel->setPixmap(movieImagePixmap.scaled(200, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    middleLayout->addWidget(movieImageLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    directorLabel = new QLabel("Director: Christopher Nolan", this);
    actorsLabel = new QLabel("Actors: Leonardo DiCaprio, Joseph Gordon-Levitt, Ellen Page", this);
    infoLayout->addWidget(directorLabel);
    infoLayout->addWidget(actorsLabel);
    middleLayout->addLayout(infoLayout);

    mainLayout->addLayout(middleLayout);

    QVBoxLayout *ratingInfoLayout = new QVBoxLayout();
    ratingLabel = new QLabel("Rating: 8.8", this);
    votesLabel = new QLabel("(1,234,567 votes)", this);
    ratingInfoLayout->addWidget(ratingLabel);
    ratingInfoLayout->addWidget(votesLabel);
    mainLayout->addLayout(ratingInfoLayout);

    QHBoxLayout *rateAndUserRatingLayout = new QHBoxLayout();
    QLabel *rateLabel = new QLabel("Rate the film:", this);
    rateAndUserRatingLayout->addWidget(rateLabel);

    userRatingLabel = new QLabel("Your rating:", this);
    userRatingLabel->setVisible(false); // Скрываем метку до первой оценки
    rateAndUserRatingLayout->addWidget(userRatingLabel);

    mainLayout->addLayout(rateAndUserRatingLayout);

    QHBoxLayout *ratingLayout = new QHBoxLayout();
    QGridLayout *ratingButtonsLayout = new QGridLayout();
    for (int i = 10; i >= 1; --i) {
        QPushButton *ratingButton = new QPushButton(QString::number(i), this);
        connect(ratingButton, &QPushButton::clicked, [this, i]() {
            // Обновление рейтинга пользователя
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

    // Добавим тестовые комментарии
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
    setFixedSize(800, 600); // Adjust the window size as needed
}

void MovieDetailWindow::onBackButtonClicked() {
    emit backToPreviousWindow(); // Emit the signal before closing
    this->close();
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
    // Добавьте обработчик для добавления фильма в список "Посмотреть позже"
    qDebug() << "Added to Watch Later";
}

void MovieDetailWindow::onLikeMovieButtonClicked() {
    // Добавьте обработчик для добавления лайка к фильму
    qDebug() << "Liked the movie";
}

void MovieDetailWindow::onAddCommentButtonClicked() {
    QString comment = commentLineEdit->text();
    if (!comment.isEmpty()) {
        QString userId = "User123"; // Здесь вы можете получить ID пользователя из системы
        addComment(comment, userId);
        commentLineEdit->clear();
        // Добавьте обработчик для сохранения комментария в базе данных
        qDebug() << "Added comment:" << comment;
    }
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
    userRating = rating;
    userHasRated = true;
    userRatingLabel->setText(QString("Your rating: %1").arg(userRating));
    userRatingLabel->setVisible(true);
}

bool MovieDetailWindow::isClosed() const {
    return closed;
}

void MovieDetailWindow::closeEvent(QCloseEvent *event) {
    closed = true;
    QWidget::closeEvent(event);
}
