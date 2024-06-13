#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QNetworkAccessManager>
#include <vector>
#include <memory>
#include "backend.h"

class SearchWindow : public QWidget {
Q_OBJECT

public:
    explicit SearchWindow(const QString &query, QWidget *parent = nullptr);

signals:
    void backToMainWindow();

private slots:
    void onBackButtonClicked();

private:
    QLabel *label;
    QString query;
    QPushButton *backButton;
    QScrollArea *moviesArea;
    QWidget *moviesContainer;
    QNetworkAccessManager *networkManager;

    void loadMoviesByQuery(const QString &query);
    void addMovieToLayout(const QSharedPointer<Movie>& movie, QGridLayout* layout, int row, int col);
};

#endif // SEARCHWINDOW_H
