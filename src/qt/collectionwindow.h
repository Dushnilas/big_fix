#ifndef COLLECTIONWINDOW_H
#define COLLECTIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QSharedPointer>
#include <vector>
#include "backend.h"

class CollectionWindow : public QWidget
{
Q_OBJECT

public:
    explicit CollectionWindow(const QSharedPointer<Collection>& collection, QWidget *parent = nullptr);
    ~CollectionWindow();

signals:
    void backToUserProfile();
    void collectionNameUpdated(const QString& newName);

private slots:
    void onBackButtonClicked();
    void onChangeCollectionNameClicked();
    void onChangeCollectionPhotoClicked();
    void onDeleteMovieClicked(const QSharedPointer<Movie> &movie);

private:
    void loadColMovies();
    void addMovieToLayout(const QSharedPointer<Movie> &movie, QHBoxLayout *layout);

    QSharedPointer<Collection> collection;
    QLabel *collectionPhotoLabel;
    QLabel *collectionNameLabel;
    QLineEdit *collectionNameEdit;
    QPushButton *changeNameButton;
    QPushButton *changePhotoButton;
    QScrollArea *moviesArea;
    QWidget *moviesContainer;
    QPushButton *backButton;
    QHBoxLayout *moviesLayout;
    QNetworkAccessManager *networkManager;
};

#endif // COLLECTIONWINDOW_H
