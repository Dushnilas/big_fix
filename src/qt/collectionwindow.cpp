#include "collectionwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QFileInfo>
#include <QDir>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "moviedetailwindow.h"
#include "backend.h" // Включите ваш заголовочный файл backend, если это необходимо
#include <QImageReader>

CollectionWindow::CollectionWindow(const QSharedPointer<Collection>& col, QWidget *parent)
        : QWidget(parent), collection(col), networkManager(new QNetworkAccessManager(this)) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout();

    collectionPhotoLabel = new QLabel(this);
    QPixmap collectionPhoto(QString::fromStdString(MY_PATH + collection->getPhoto()));
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
    moviesLayout = new QHBoxLayout(moviesContainer);

    loadColMovies();

    moviesContainer->setLayout(moviesLayout);
    moviesArea->setWidget(moviesContainer);
    mainLayout->addWidget(moviesArea);

    backButton = new QPushButton("Back");
    connect(backButton, &QPushButton::clicked, this, &CollectionWindow::onBackButtonClicked);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);

    setStyleSheet(
            "CollectionWindow {"
            "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
            "    color: white;"
            "}"
            "QLabel {"
            "    color: rgb(229, 217, 190);"
            "}"
            "QPushButton {"
            "    background-color: rgba(255, 255, 255, 0);"
            "    color: rgb(229, 217, 190);"
            "    padding: 5px;"
            "}"
    );
    setWindowTitle("Collection: " + QString::fromStdString(collection->getName()));
    setMinimumSize(1024, 768);
    resize(1280, 720);
}

CollectionWindow::~CollectionWindow() {
    std::cout << "Aboba2" << '\n';
    delete networkManager;
    std::cout << "Aboba3" << '\n';
}

void CollectionWindow::onBackButtonClicked() {
    emit backToUserProfile();
    close();
    std::cout << "Aboba1" << '\n';
}

void CollectionWindow::onChangeCollectionNameClicked() {
    if (collectionNameEdit->isVisible()) {
        QString newName = collectionNameEdit->text();
        if (!newName.isEmpty()) {
            collectionNameLabel->setText(newName);
            collection->setName(newName.toStdString());
            emit collectionNameUpdated(newName);
        }
        collectionNameEdit->setVisible(false);
        collectionNameLabel->setVisible(true);
    } else {
        collectionNameEdit->setVisible(true);
        collectionNameLabel->setVisible(false);
    }
}

void CollectionWindow::onChangeCollectionPhotoClicked() {
//    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Photo"), "", tr("Images (*.png *.xpm *.jpg *.jpeg)"));
//    if (!filePath.isEmpty()) {
//        QFileInfo fileInfo(filePath);
//        QString fileName = fileInfo.fileName();
//        QString targetDir = "src/qt/pictures/";
//        QString targetPath = targetDir + fileName;
//
//        QDir dir(targetDir);
//        if (!dir.exists()) {
//            dir.mkpath(".");
//        }
//
//        QImageReader reader(filePath);
//        reader.setAutoTransform(true);
//        QImage newImage = reader.read();
//        if (newImage.isNull()) {
//            QMessageBox::warning(this, tr("Load Failed"), tr("Failed to load the image."));
//            return;
//        }
//
//        QImage scaledImage = newImage.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        if (scaledImage.save(targetPath)) {
//            collectionPhotoLabel->setPixmap(QPixmap::fromImage(scaledImage));
//            collection->setPhoto(targetPath.toStdString());
//        } else {
//            QMessageBox::warning(this, tr("Copy Failed"), tr("Failed to copy the photo to the target directory."));
//        }
//    }
}

void CollectionWindow::onDeleteMovieClicked(const QSharedPointer<Movie> &movie) {
    if (QMessageBox::question(this, tr("Delete Movie"), tr("Are you sure you want to delete this movie?")) == QMessageBox::Yes) {
        collection->removeMovie(movie);
        loadColMovies();
    }
}

void CollectionWindow::loadColMovies() {
    QLayoutItem *child;
    while ((child = moviesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    std::vector<QSharedPointer<Movie>> movies = collection->getMovies();
    for (const auto &movie : movies) {
        addMovieToLayout(movie, moviesLayout);
    }

    if (movies.size() < 5) {
        QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        moviesLayout->addItem(spacer);
    }
}

void CollectionWindow::addMovieToLayout(const QSharedPointer<Movie> &movie, QHBoxLayout *layout) {
    QPushButton *movieButton = new QPushButton(this);
    movieButton->setIconSize(QSize(150, 150));
    connect(movieButton, &QPushButton::clicked, [this, movie]() {
        MovieDetailWindow *movieDetailWindow = new MovieDetailWindow(movie);
        movieDetailWindow->show();
        this->hide();
        connect(movieDetailWindow, &MovieDetailWindow::backToPreviousWindow, this, &CollectionWindow::show);
    });

    QUrl imageUrl(QString::fromStdString(movie->getPhoto()));
    QNetworkRequest request(imageUrl);
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, [reply, movieButton]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            movieButton->setIcon(QIcon(pixmap));
        } else {
            qDebug() << "Error downloading image:" << reply->errorString();
        }
        reply->deleteLater();
    });

    QLabel *movieTitle = new QLabel(QString::fromStdString(movie->getName()), this);
    movieTitle->setAlignment(Qt::AlignCenter);

    QPushButton *deleteButton = new QPushButton("Delete", this);
    connect(deleteButton, &QPushButton::clicked, [this, movie]() {
        onDeleteMovieClicked(movie);
    });

    QVBoxLayout *movieLayout = new QVBoxLayout();
    movieLayout->addWidget(movieButton);
    movieLayout->addWidget(movieTitle);
    movieLayout->addWidget(deleteButton);

    QWidget *movieWidget = new QWidget();
    movieWidget->setLayout(movieLayout);

    layout->addWidget(movieWidget);
}
