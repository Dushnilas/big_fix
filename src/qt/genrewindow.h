#ifndef GENREWINDOW_H
#define GENREWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class GenreWindow : public QWidget {
    Q_OBJECT

public:
    explicit GenreWindow(const QString &genre, QWidget *parent = nullptr);

    signals:
        void backToMoviesWindow();

    private slots:
        void onBackButtonClicked();
    void onUserProfileButtonClicked();

private:
    QLabel *label;
    QString genre;
    QPushButton *backButton;
    QPushButton *userProfileButton;
};

#endif // GENREWINDOW_H
