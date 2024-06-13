#include "collectionwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QLabel>
#include <QPushButton>

CollectionWindow::CollectionWindow(const QSharedPointer<Collection>& collection, QWidget *parent)
        : QWidget(parent), collection(collection)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();

    collectionPhotoLabel = new QLabel(this);
    QPixmap collectionPhoto(qFilePath("src/qt/pictures/default_collection.jpg")); // Placeholder image
    collectionPhotoLabel->setPixmap(collectionPhoto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    collectionPhotoLabel->setFixedSize(200, 200);
    collectionPhotoLabel->setStyleSheet("border: 1px solid black;");

    QVBoxLayout *infoLayout = new QVBoxLayout();

    collectionNameLabel = new QLabel(QString::fromStdString(collection->getName()), this);
    collectionNameEdit = new QLineEdit(this);
    collectionNameEdit->setText(QString::fromStdString(collection->getName()));
    collectionNameEdit->setVisible(false); // Initially hidden

    changeNameButton = new QPushButton("Change Name", this);
    connect(changeNameButton, &QPushButton::clicked, this, &CollectionWindow::onChangeCollectionNameClicked);

    changePhotoButton = new QPushButton("Change Photo", this);
    connect(changePhotoButton, &QPushButton::clicked, this, &CollectionWindow::onChangeCollectionPhotoClicked);

    infoLayout->addWidget(collectionNameLabel);
    infoLayout->addWidget(collectionNameEdit);
    infoLayout->addWidget(changeNameButton);
    infoLayout->addWidget(changePhotoButton);

    topLayout->addWidget(collectionPhotoLabel);
    topLayout->addLayout(infoLayout);

    mainLayout->addLayout(topLayout);

    moviesArea = new QScrollArea(this);
    moviesArea->setWidgetResizable(true);
    moviesContainer = new QWidget(moviesArea);
    moviesLayout = new QVBoxLayout(moviesContainer);

    loadMovies();

    moviesContainer->setLayout(moviesLayout);
    moviesArea->setWidget(moviesContainer);
    mainLayout->addWidget(moviesArea);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &CollectionWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);
    setWindowTitle("Collection: " + QString::fromStdString(collection->getName()));
    setFixedSize(800, 600);
}

CollectionWindow::~CollectionWindow() {}

void CollectionWindow::onBackButtonClicked()
{
    emit backToUserProfile();
    this->deleteLater();
}

void CollectionWindow::onChangeCollectionNameClicked()
{
    if (collectionNameEdit->isVisible()) {
        // Save new name
        QString newName = collectionNameEdit->text();
        if (!newName.isEmpty()) {
            collectionNameLabel->setText(newName);
            collection->setName(newName.toStdString());
        }
        collectionNameEdit->setVisible(false);
        collectionNameLabel->setVisible(true);
    } else {
        // Edit name
        collectionNameEdit->setVisible(true);
        collectionNameLabel->setVisible(false);
    }
}

void CollectionWindow::onChangeCollectionPhotoClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Photo"), "", tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        QString targetDir = "src/qt/pictures/";
        QString targetPath = targetDir + fileName;

        QDir dir(targetDir);
        if (!dir.exists()) {
            dir.mkpath(".");
        }

        if (QFile::copy(filePath, targetPath)) {
            QPixmap collectionPhoto(targetPath);
            collectionPhotoLabel->setPixmap(collectionPhoto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            collection->setPhoto(targetPath.toStdString());
        } else {
            QMessageBox::warning(this, tr("Copy Failed"), tr("Failed to copy the photo to the target directory."));
        }
    }
}

void CollectionWindow::onDeleteMovieClicked(const QSharedPointer<Movie> &movie)
{
    if (QMessageBox::question(this, tr("Delete Movie"), tr("Are you sure you want to delete this movie?")) == QMessageBox::Yes) {
        collection->removeMovie(movie);
        loadColMovies(); // Reload movies after deletion
    }
}

void CollectionWindow::loadColMovies()
{
    QLayoutItem *child;
    while ((child = moviesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    std::vector<QSharedPointer<Movie>> movies = collection->getMovies();
    for (const auto &movie : movies) {
        addMovieToLayout(movie, moviesLayout);
    }
}

void CollectionWindow::addMovieToLayout(const QSharedPointer<Movie> &movie, QVBoxLayout *layout)
{
    QHBoxLayout *movieLayout = new QHBoxLayout();

    QLabel *movieLabel = new QLabel(QString::fromStdString(movie->getName()), this);

    QPushButton *deleteButton = new QPushButton("Delete", this);
    connect(deleteButton, &QPushButton::clicked, [this, movie]() {
        onDeleteMovieClicked(movie);
    });

    movieLayout->addWidget(movieLabel);
    movieLayout->addWidget(deleteButton);

    layout->addLayout(movieLayout);
}
