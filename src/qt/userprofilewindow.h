#ifndef USERPROFILEWINDOW_H
#define USERPROFILEWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QInputDialog>
#include <QString>
#include <vector>
#include "backend.h"

class CollectionWindow;

class UserProfileWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit UserProfileWindow(QWidget *previousWindow, QWidget *parent = nullptr);
    ~UserProfileWindow();

signals:
    void backToPreviousWindow();

private slots:
    void onBackButtonClicked();
    void onCollectionClicked(const QSharedPointer<Collection>& col);
    void onEditNameClicked();
    void onEditEmailClicked();
    void onEditGenderClicked();
    void onReturnFromCollection();
    void onAddCollectionClicked();
    void onChangePhotoClicked();

private:
    QWidget *previousWindow;
    QLabel *photoLabel;
    QLabel *userIdLabel;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *genderLabel;
    QLabel *mailLabel;
    QPushButton *editButton1;
    QPushButton *editButton2;
    QPushButton *editButton3;
    QPushButton *changePhotoButton;
    QScrollArea *collectionsArea;
    QWidget *collectionsContainer;
    QPushButton *backButton;
    CollectionWindow *collectionWindow;
    QPushButton *addCollectionButton;
    QComboBox *genderComboBox;
};

#endif // USERPROFILEWINDOW_H
