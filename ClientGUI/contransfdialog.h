#ifndef CONTRANSFDIALOG_H
#define CONTRANSFDIALOG_H

#include <QDialog>

namespace Ui {
class ConTransfDialog;
}

class ConTransfDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConTransfDialog(QWidget *parent = nullptr);
    ~ConTransfDialog();

    QVector<QString> getInput();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_transfer_clicked();

private:
    Ui::ConTransfDialog *ui;
};

#endif // CONTRANSFDIALOG_H
