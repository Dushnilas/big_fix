#include "collectiondialog.h"
#include <QMessageBox>
#include <QPixmap>
#include "backend.h"

CollectionDialog::CollectionDialog(QWidget *parent)
    : QDialog(parent), selectedImagePath(""), selectedButton(nullptr)
{
    setWindowTitle("Add Collection");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Enter collection name:", this);
    nameEdit = new QLineEdit(this);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);

    QLabel *imageLabel = new QLabel("Select an image:", this);
    mainLayout->addWidget(imageLabel);

    buttonGroup = new QButtonGroup(this);
    QHBoxLayout *imagesLayout = new QHBoxLayout();

    for (int i = 1; i <= 6; ++i) {
        QString imagePath = QString(qFilePath("/pictures/collection_image_%1.jpg")).arg(i);
        QPixmap pixmap(imagePath);

        QPushButton *imageButton = new QPushButton(this);
        imageButton->setIcon(QIcon(pixmap));
        imageButton->setIconSize(QSize(100, 100));
        imageButton->setFixedSize(100, 100);
        imageButton->setCheckable(true);

        buttonGroup->addButton(imageButton, i);
        imagesLayout->addWidget(imageButton);

        connect(buttonGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &CollectionDialog::onImageButtonClicked);
    }

    mainLayout->addLayout(imagesLayout);

    QPushButton *addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(addButton);

    setLayout(mainLayout);
}

QString CollectionDialog::getCollectionName() const
{
    return nameEdit->text();
}

QString CollectionDialog::getSelectedImagePath() const
{
    return selectedImagePath;
}

void CollectionDialog::onImageButtonClicked(int id)
{
    selectedImagePath = QString(qFilePath("/pictures/collection_image_%1.jpg")).arg(id);
    selectedButton = qobject_cast<QPushButton*>(buttonGroup->button(id));
    updateButtonStyles();
}

void CollectionDialog::updateButtonStyles()
{
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QPushButton *pushButton = qobject_cast<QPushButton*>(button);
        if (pushButton) {
            if (pushButton == selectedButton) {
                pushButton->setStyleSheet("border: 2px solid blue;");
            } else {
                pushButton->setStyleSheet("");
            }
        }
    }
}
