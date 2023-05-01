#include "contransfdialog.h"
#include "ui_contransfdialog.h"
#include "mainwindow.h"

#include <QMessageBox>

ConTransfDialog::ConTransfDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConTransfDialog)
{
    ui->setupUi(this);
}

ConTransfDialog::~ConTransfDialog()
{
    delete ui;
}

void ConTransfDialog::on_pushButton_cancel_clicked()
{
    hide();
}


void ConTransfDialog::on_pushButton_transfer_clicked()
{
    auto input = getInput();
    if(input[0].isEmpty() || input[1].isEmpty() || input[2].isEmpty())
        QMessageBox::warning(this, "Error", "All fields should be filled.\n");
    else
        ((MainWindow*)parent())->ConductTransfer(input[0], input[1], input[2]);
}

QVector<QString> ConTransfDialog::getInput() {
    return {ui->lineEdit_from->text(), ui->lineEdit_to->text(), ui->lineEdit_amount->text()};
}
