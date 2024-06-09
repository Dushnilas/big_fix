#ifndef COLLECTIONWINDOW_H
#define COLLECTIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CollectionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CollectionWindow(const QString &collectionName, QWidget *parent = nullptr);
    ~CollectionWindow();

    signals:
        void backToUserProfile();

    private slots:
        void onBackButtonClicked();

private:
    QString collectionName;
    QLabel *titleLabel;
    QScrollArea *moviesArea;
    QWidget *moviesContainer;
    QPushButton *backButton;
};

#endif // COLLECTIONWINDOW_H
