#include "collectionwindow.h"
#include <QPixmap>
#include <QDebug>

CollectionWindow::CollectionWindow(const QString &collectionName, QWidget *parent)
    : QWidget(parent), collectionName(collectionName)
{
    setWindowFlags(Qt::Window); // Ensure it's a top-level window
    setWindowTitle(QString("Collection: %1").arg(collectionName));
    setFixedSize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    titleLabel = new QLabel(QString("Collection: %1").arg(collectionName), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(titleLabel);

    QGridLayout *gridLayout = new QGridLayout();

    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 4; ++j) {
            int movieId = i * 4 + j + 1;
            QPushButton *movieButton = new QPushButton(this);
            movieButton->setIcon(QIcon("/Users/maykorablina/Yandex.Disk.localized/CodingProjects/big_fix_3/src/qt/film_image.jpg"));
            movieButton->setIconSize(QSize(100, 150));
            gridLayout->addWidget(movieButton, i, j);
        }
    }

    moviesArea = new QScrollArea(this);
    moviesContainer = new QWidget(moviesArea);
    moviesContainer->setLayout(gridLayout);
    moviesArea->setWidget(moviesContainer);
    moviesArea->setWidgetResizable(true);

    mainLayout->addWidget(moviesArea);

    backButton = new QPushButton("Back", this);
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
        "    border: 5px solid rgb(229, 217, 190);"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        );
}

CollectionWindow::~CollectionWindow()
{
}

void CollectionWindow::onBackButtonClicked()
{
    qDebug() << "Back button clicked in collection window";
    emit backToUserProfile();
    this->hide();
    if (parentWidget()) {
        parentWidget()->show();
    }
}
