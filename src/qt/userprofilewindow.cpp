#include "userprofilewindow.h"
#include "collectionwindow.h"
#include "collectiondialog.h"
#include "backend.h"
#include <QPixmap>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QVBoxLayout>
#include "genders.h"
#include <QComboBox>

UserProfileWindow::UserProfileWindow(QWidget *previousWindow, QWidget *parent)
    : QMainWindow(parent), previousWindow(previousWindow), collectionWindow(nullptr)
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    main_user->loadCol();

    photoLabel = new QLabel(this);
    QPixmap userPhoto(qFilePath("src/qt/pictures/user_photo.jpg"));
    photoLabel->setPixmap(userPhoto.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    photoLabel->setFixedSize(200, 200);
    photoLabel->setStyleSheet(
        "border: 1px solid rgba(229, 217, 190, 1);"
        "border-radius: 10px;"
        "background-color: rgba(0, 0, 0, 0);"
    );

    userIdLabel = new QLabel(QString::fromStdString(main_user->getLogin()), this);
    nameLabel = new QLabel("Name: " + QString::fromStdString(main_user->getName()), this);
    ageLabel = new QLabel("Age: " + QString::fromStdString(std::to_string(main_user->getAge())), this);
    genderLabel = new QLabel("Gender: " + QString::fromStdString(genderToString(main_user->getGender())), this);
    mailLabel = new QLabel("Email: " + QString::fromStdString(main_user->getEmail()), this);

    editButton1 = new QPushButton("Edit", this);
    editButton2 = new QPushButton("Edit", this);
    editButton3 = new QPushButton("Edit", this);

    connect(editButton1, &QPushButton::clicked, this, &UserProfileWindow::onEditNameClicked);
    connect(editButton2, &QPushButton::clicked, this, &UserProfileWindow::onEditEmailClicked);
    connect(editButton3, &QPushButton::clicked, this, &UserProfileWindow::onEditGenderClicked);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(editButton1);

    QHBoxLayout *ageLayout = new QHBoxLayout();
    ageLayout->addWidget(ageLabel);

    QHBoxLayout *genderLayout = new QHBoxLayout();
    genderLayout->addWidget(genderLabel);
    genderLayout->addWidget(editButton3);

    QHBoxLayout *mailLayout = new QHBoxLayout();
    mailLayout->addWidget(mailLabel);
    mailLayout->addWidget(editButton2);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(userIdLabel);
    infoLayout->addLayout(nameLayout);
    infoLayout->addLayout(ageLayout);
    infoLayout->addLayout(genderLayout);
    infoLayout->addLayout(mailLayout);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(photoLabel);
    topLayout->addLayout(infoLayout);

    collectionsArea = new QScrollArea(this);
    collectionsArea->setWidgetResizable(true);
    collectionsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    collectionsContainer = new QWidget(collectionsArea);
    QHBoxLayout *collectionsLayout = new QHBoxLayout(collectionsContainer);

    std::vector<QSharedPointer<Collection>> all_colls = main_user->getAllCol();
    std::cout << yellow_color_code << all_colls.size() << reset_color_code << "\n";
    for (int i = 0; i < all_colls.size(); ++i) {
        QPushButton *collectionButton = new QPushButton(this);
        collectionButton->setIcon(QIcon(qFilePath(all_colls[i]->getPhoto())));
        collectionButton->setIconSize(QSize(150, 150));
        collectionButton->setFixedSize(150, 150);
        collectionButton->setStyleSheet("border: none;");
        connect(collectionButton, &QPushButton::clicked, [this, collectionName = QString::fromStdString(all_colls[i]->getName())]() {
        onCollectionClicked(collectionName);
        });

        QVBoxLayout *collectionLayout = new QVBoxLayout();
        collectionLayout->setAlignment(Qt::AlignCenter);
        collectionLayout->addWidget(collectionButton);

        QLabel *collectionLabel = new QLabel(QString::fromStdString(all_colls[i]->getName()), this);
        collectionLabel->setAlignment(Qt::AlignCenter);
        collectionLayout->addWidget(collectionLabel);

        QWidget *collectionWidget = new QWidget();
        collectionWidget->setLayout(collectionLayout);

        collectionsLayout->addWidget(collectionWidget);
    }

    collectionsContainer->setLayout(collectionsLayout);
    collectionsArea->setWidget(collectionsContainer);

    collectionsArea->setFixedHeight(300);

    addCollectionButton = new QPushButton("Add Collection", this);
    connect(addCollectionButton, &QPushButton::clicked, this, &UserProfileWindow::onAddCollectionClicked);

    backButton = new QPushButton("Back", this);
    connect(backButton, &QPushButton::clicked, this, &UserProfileWindow::onBackButtonClicked);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(collectionsArea);
    mainLayout->addWidget(addCollectionButton);
    mainLayout->addWidget(backButton);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("User Profile");
    setFixedSize(800, 600);

    setStyleSheet(
        "UserProfileWindow {"
        "    background: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgb(0, 0, 0), stop:1 rgb(19, 21, 59));"
        "    color: white;"
        "}"
        "QLabel {"
        "    color: rgb(229, 217, 190);"
        "}"
        "QPushButton {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: 1px solid rgb(229, 217, 190);"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        "QScrollArea {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "    border: none;"
        "}"
        "QScrollArea QWidget {"
        "    background-color: rgba(255, 255, 255, 0);"
        "    color: rgb(229, 217, 190);"
        "}"
        );
}

UserProfileWindow::~UserProfileWindow()
{
    if (collectionWindow) {
        delete collectionWindow;
    }
}

void UserProfileWindow::onBackButtonClicked()
{
    emit backToPreviousWindow();
    this->hide();
    previousWindow->show();
}

void UserProfileWindow::onCollectionClicked(const QString &collectionName)
{
    qDebug() << "Collection clicked:" << collectionName;
    if (collectionWindow) {
        delete collectionWindow;
    }
    collectionWindow = new CollectionWindow(collectionName, this);
    qDebug() << "Collection window created";
    connect(collectionWindow, &CollectionWindow::backToUserProfile, this, &UserProfileWindow::onReturnFromCollection);
    collectionWindow->show();
    qDebug() << "Collection window shown";
    this->hide();
    qDebug() << "User profile window hidden";
}

void UserProfileWindow::onReturnFromCollection()
{
    this->show();
    if (collectionWindow) {
        collectionWindow->hide();
        delete collectionWindow;
        collectionWindow = nullptr;
    }
}

void UserProfileWindow::onEditNameClicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Edit Name"),
                                         tr("Enter new name:"), QLineEdit::Normal,
                                         nameLabel->text().mid(6), &ok);
    if (ok && !text.isEmpty())
    {
        main_user->setName(text.toStdString());
        nameLabel->setText("Name: " + text);
    }
}

void UserProfileWindow::onEditEmailClicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Edit Email"),
                                         tr("Enter new email:"), QLineEdit::Normal,
                                         mailLabel->text().mid(6), &ok);
    if (ok && !text.isEmpty())
    {
        main_user->setEmail(text.toStdString());
        mailLabel->setText("Email: " + text);
    }
}

void UserProfileWindow::onEditGenderClicked()
{
    QComboBox* genderComboBox = new QComboBox(this);
    for (const auto &gender : getAllGenders()) {
        genderComboBox->addItem(QString::fromStdString(gender));
    }

    QStringList genderList;
    for (int i = 0; i < genderComboBox->count(); ++i) {
        genderList << genderComboBox->itemText(i);
    }

    bool ok;
    QString selectedGender = QInputDialog::getItem(this, tr("Edit Gender"),
                                                   tr("Select new gender:"),
                                                   genderList,
                                                   0, false, &ok);
    if (ok && !selectedGender.isEmpty())
    {
        main_user->setGender(selectedGender.toStdString());
        genderLabel->setText("Gender: " + selectedGender);
    }

    delete genderComboBox;
}


void UserProfileWindow::onAddCollectionClicked()
{
    CollectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString collectionName = dialog.getCollectionName();
        QString imagePath = dialog.getSelectedImagePath();
        std::string dbPath = replaceAllOccurrences(imagePath.toStdString(), MY_PATH);
        if (main_user->getAllCol().size() >= 6) {
            QMessageBox::warning(this, tr("Too Many Collections"), tr("You can create no more than SIX collections."));
            return;
        }
        if (collectionName.isEmpty()) {
            QMessageBox::warning(this, tr("Invalid Name"), tr("Collection name cannot be empty."));
            return;
        }

        if (collectionName.length() > 30) {
            QMessageBox::warning(this, tr("Invalid Name"), tr("Collection name must be 30 characters or less."));
            return;
        }

        if (imagePath.isEmpty()) {
            QMessageBox::warning(this, tr("No Image Selected"), tr("You must select an image for the collection."));
            return;
        }

        bool create_col_status = main_user->createCol(collectionName.toStdString(), dbPath);

        if (!create_col_status) {
            QMessageBox::warning(this, tr("Duplicate detected"), tr("Collection with this name already exists."));
            return;
        }

        QPushButton *collectionButton = new QPushButton(this);
        collectionButton->setIcon(QIcon(imagePath));
        collectionButton->setIconSize(QSize(150, 150));
        collectionButton->setFixedSize(150, 150);
        collectionButton->setStyleSheet("border: none;");
        connect(collectionButton, &QPushButton::clicked, [this, collectionName]() {
            onCollectionClicked(collectionName);
        });

        QVBoxLayout *collectionLayout = new QVBoxLayout();
        collectionLayout->setAlignment(Qt::AlignCenter);
        collectionLayout->addWidget(collectionButton);

        QLabel *collectionLabel = new QLabel(collectionName, this);
        collectionLabel->setAlignment(Qt::AlignCenter);
        collectionLayout->addWidget(collectionLabel);

        QWidget *collectionWidget = new QWidget();
        collectionWidget->setLayout(collectionLayout);

        QHBoxLayout *collectionsLayout = qobject_cast<QHBoxLayout*>(collectionsContainer->layout());
        collectionsLayout->addWidget(collectionWidget);
    }
}
