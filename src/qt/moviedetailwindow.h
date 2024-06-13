#ifndef MOVIEDETAILWINDOW_H
#define MOVIEDETAILWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "backend.h"

class MovieDetailWindow : public QWidget {
Q_OBJECT

public:
    explicit MovieDetailWindow(const QSharedPointer<Movie>& mov, QWidget *parent = nullptr);
    bool isClosed() const;

signals:
    void backToPreviousWindow();

private slots:
    void onBackButtonClicked();
    void onAddToCollectionButtonClicked();
    void onWatchLaterButtonClicked();
    void onLikeMovieButtonClicked();
    void onAddCommentButtonClicked();
    void onImageDownloaded(QNetworkReply* reply);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QSharedPointer<Movie> movie;
    QLabel *titleLabel;
    QLabel *directorLabel;
    QLabel *actorsLabel;
    QLabel *ratingLabel;
    QLabel *votesLabel;
    QLabel *movieImageLabel;
    QLabel *userRatingLabel;
    QPushButton *backButton;
    QPushButton *addToCollectionButton;
    QPushButton *watchLaterButton;
    QPushButton *likeMovieButton;
    QLineEdit *commentLineEdit;
    QPushButton *addCommentButton;
    QScrollArea *commentsArea;
    bool closed;
    bool userHasRated;
    int userRating;
    QNetworkAccessManager *networkManager;

    void showCollectionDialog();
    void addComment(const QString &commentText, const QString &userId);
    void updateUserRating(int rating);
};

#endif // MOVIEDETAILWINDOW_H
