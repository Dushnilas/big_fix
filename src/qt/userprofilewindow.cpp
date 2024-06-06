#include "userprofilewindow.h"
#include <QPixmap>

UserProfileWindow::UserProfileWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Photo
    photoLabel = new QLabel(this);
    QPixmap userPhoto("/Users/maykorablina/Yandex.Disk.localized/CodingProjects/big_fix_3/src/qt/user_photo.jpg");
    photoLabel->setPixmap(userPhoto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    photoLabel->setFixedSize(200, 200); // Example size for the photo label
    photoLabel->setStyleSheet("border: 1px solid black;");

    // User info
    userIdLabel = new QLabel("User ID: user_id", this);
    nameLabel = new QLabel("Name: [User's Name]", this); // Example name
    ageLabel = new QLabel("Age: [User's Age]", this);    // Example age
    mailLabel = new QLabel("Mail: [User's Email]", this); // Example email

    editButton1 = new QPushButton("Edit", this);
    editButton2 = new QPushButton("Edit", this);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(editButton1);

    QHBoxLayout *ageLayout = new QHBoxLayout();
    ageLayout->addWidget(ageLabel);

    QHBoxLayout *mailLayout = new QHBoxLayout();
    mailLayout->addWidget(mailLabel);
    mailLayout->addWidget(editButton2);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(userIdLabel);
    infoLayout->addLayout(nameLayout);
    infoLayout->addLayout(ageLayout);
    infoLayout->addLayout(mailLayout);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(photoLabel);
    topLayout->addLayout(infoLayout);

    // Collections
    collectionsArea = new QScrollArea(this);
    collectionsArea->setWidgetResizable(true);
    collectionsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn); // Enable horizontal scrollbar
    collectionsContainer = new QWidget(collectionsArea);
    QHBoxLayout *collectionsLayout = new QHBoxLayout(collectionsContainer);

    // Example collection items
    for (int i = 1; i <= 10; ++i) { // Increased number of items to demonstrate scrolling
        QLabel *collectionLabel = new QLabel(QString("Collection %1").arg(i), this);
        collectionLabel->setFixedSize(100, 100); // Example size for collection items
        collectionLabel->setStyleSheet("border: 1px solid black;");

        collectionsLayout->addWidget(collectionLabel);
    }

    collectionsContainer->setLayout(collectionsLayout);
    collectionsArea->setWidget(collectionsContainer);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(collectionsArea);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("User Profile");
    setMinimumSize(600, 400); // Adjust the window size as needed
}

UserProfileWindow::~UserProfileWindow()
{
}
