#ifndef COLLECTIONDIALOG_H
#define COLLECTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>

class CollectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CollectionDialog(QWidget *parent = nullptr);
    QString getCollectionName() const;
    QString getSelectedImagePath() const;

    private slots:
        void onImageButtonClicked(int id);

private:
    QLineEdit *nameEdit;
    QButtonGroup *buttonGroup;
    QString selectedImagePath;
};

#endif // COLLECTIONDIALOG_H
