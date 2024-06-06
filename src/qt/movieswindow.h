#ifndef MOVIESWINDOW_H
#define MOVIESWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <vector>
#include <memory>

class MoviesWindow : public QWidget {
    Q_OBJECT

public:
    MoviesWindow(QWidget *parent = nullptr);
    ~MoviesWindow();

    private slots:
        void openUserProfile();

private:
    QPushButton *userProfileButton;
    QListWidget *genresList;
    QScrollArea *moviesArea1;
    QScrollArea *moviesArea2;
    QWidget *moviesContainer1;
    QWidget *moviesContainer2;

    void loadGenres();
    std::vector<std::string> fetchGenres();
};

#endif // MOVIESWINDOW_H
