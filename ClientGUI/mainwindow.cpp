#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "contransfdialog.h"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <QMessageBox>
#include "ConnControler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    login = (Login*)parent;
    ui->tableWidget_accInfo->hide();
    ui->tableWidget_transactionHistory->hide();
    ui->tableWidget_accountList->hide();

    QTableWidget *pWidget = ui->tableWidget_accInfo;
    for(int col = 0; col < pWidget->columnCount(); col++)
        for(int row = 0; row < pWidget->rowCount(); row++) {
            pWidget->setItem( row, col, new QTableWidgetItem());
        }
    pWidget->setColumnWidth(0, 700);

    QTableWidget* accounts = ui->tableWidget_accountList;
    accounts->setColumnWidth(0, accounts->width()/3);
    accounts->setColumnWidth(1, accounts->width()/3);
    accounts->setColumnWidth(2, accounts->width()/3);
}

MainWindow::~MainWindow()
{
    QTableWidget *pWidget = ui->tableWidget_accInfo;
    for(int col = 0; col < pWidget->columnCount(); col++)
        for(int row = 0; row < pWidget->rowCount(); row++)
            delete pWidget->item(row, col);

    pWidget = ui->tableWidget_accountList;
    for(int col = 0; col < pWidget->columnCount(); col++)
        for(int row = 0; row < pWidget->rowCount(); row++)
            delete pWidget->item(row, col);
    delete ui;
    login->show();
}

void MainWindow::setConnController(ConnControler *cc)
{
    this->cc = cc;
}

int MainWindow::ConductTransfer(QString &from, QString &to, QString &amount)
{
    json request = {
        {"type", 2},
        {"owners_id", user_id.toLongLong()},
        {"from", from.toLongLong()},
        {"to", to.toLongLong()},
        {"amount", amount.toDouble()},
    };

    this->cc->sendMessage(request.dump());
    QString answer = QString::fromStdString(json::parse(this->cc->getMessage())["description"]);

    QMessageBox::about(this, "Transfer", answer);

}

int MainWindow::setUserID(QString user_id)
{
    this->user_id = user_id;
}

void MainWindow::on_actionLogOut_triggered()
{
    login->show();
    close();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    login->show();
    event->accept();
}

void MainWindow::on_actionConductTransfer_triggered()
{
    if(transfer == nullptr) {
        transfer = new ConTransfDialog(this);
    }
    transfer->show();
}

void MainWindow::on_actionOpenDebitAccount_triggered()
{
    cc->sendMessage(json {{"type", 3}, {"user_id", user_id.toLongLong()}}.dump());
    QMessageBox::information(this, "Success",
                                "Your new account ID number:" +
                                 QString::number(json::parse(cc->getMessage())["account_id"].get<long long>())
                             );
}

void MainWindow::on_actionAboutProgram_triggered()
{
    QMessageBox::about(this, "About Program", "XD 2023(C)");
}


void MainWindow::on_actionCloseDebitAccount_triggered()
{

}


void MainWindow::on_actionShow_transactionHistory_triggered()
{
    ui->tableWidget_accInfo->hide();
    ui->tableWidget_accountList->hide();
    ui->tableWidget_transactionHistory->show();
}

void MainWindow::on_actionShow_userInfo_triggered()
{
    std::string request = json {
        {"type", 6},
        {"user_id", user_id.toStdString()}
    }.dump();
    cc->sendMessage(request);
    json answer = json::parse(cc->getMessage());

    QTableWidget *pWidget = ui->tableWidget_accInfo;

    QVariant name = QString::fromStdString(answer["name"].get<std::string>());
    pWidget->item(0, 0)->setData(Qt::DisplayRole, name);
    QVariant surname = QString::fromStdString(answer["surname"].get<std::string>());
    pWidget->item(1, 0)->setData(Qt::DisplayRole, surname);
    QVariant last_name = QString::fromStdString(answer["last_name"].get<std::string>());
    pWidget->item(2, 0)->setData(Qt::DisplayRole, last_name);
    QVariant sex = answer["sex"].get<std::string>() == "f" ? QString{"female"} : QString{"male"};
    pWidget->item(3, 0)->setData(Qt::DisplayRole, sex);
    QVariant date_of_birth = QString::fromStdString(answer["date_of_birth"].get<std::string>());
    pWidget->item(4, 0)->setData(Qt::DisplayRole, date_of_birth);
    QVariant place_of_birth = QString::fromStdString(answer["place_of_birth"].get<std::string>());
    pWidget->item(5, 0)->setData(Qt::DisplayRole, place_of_birth);
    QVariant country = QString::fromStdString(answer["country"].get<std::string>());
    pWidget->item(6, 0)->setData(Qt::DisplayRole, country);
    QVariant passport_date_of_issue = QString::fromStdString(answer["passport_date_of_issue"].get<std::string>());
    pWidget->item(7, 0)->setData(Qt::DisplayRole, passport_date_of_issue);
    QVariant passport_date_of_expire = QString::fromStdString(answer["passport_date_of_expire"].get<std::string>());
    pWidget->item(8, 0)->setData(Qt::DisplayRole, passport_date_of_expire);
    QVariant passport_place_of_issue = QString::fromStdString(answer["passport_place_of_issue"].get<std::string>());
    pWidget->item(9, 0)->setData(Qt::DisplayRole, passport_place_of_issue);
    QVariant passport_authority = QString::fromStdString(answer["passport_authority"].get<std::string>());
    pWidget->item(10, 0)->setData(Qt::DisplayRole, passport_authority);
    QVariant address_of_living = QString::fromStdString(answer["address_of_living"].get<std::string>());
    pWidget->item(11, 0)->setData(Qt::DisplayRole, address_of_living);
    QVariant email = QString::fromStdString(answer["email"].get<std::string>());
    pWidget->item(12, 0)->setData(Qt::DisplayRole, email);

    ui->tableWidget_transactionHistory->hide();
    ui->tableWidget_accountList->hide();
    ui->tableWidget_accInfo->show();

}

void MainWindow::on_actionRefresh_accountList_triggered()
{
    cc->sendMessage(json {{"type", 5}, {"user_id", user_id.toLongLong()}}.dump());
    json answer = json::parse(cc->getMessage());
    ui->tableWidget_accInfo->hide();
    ui->tableWidget_transactionHistory->hide();
    ui->tableWidget_accountList->show();

    if(answer.empty()){
        QMessageBox::information(this, "MVVM Bank Client", "You have no accounts!");
        return;
    }

    QTableWidget *accounts = ui->tableWidget_accountList;
    int rowsDiff = answer.size() - accounts->rowCount();
    if(rowsDiff > 0)
        for(int currentRows = accounts->rowCount(); currentRows < answer.size() ;currentRows++)
            accounts->insertRow(accounts->rowCount());

    for (int accIndex = 0; accIndex < answer.size(); ++accIndex) {
        for (int fieldIndex = 0; fieldIndex < 3; ++fieldIndex) {
            if(accounts->item(accIndex, fieldIndex) == nullptr) {
                accounts->setItem(accIndex, fieldIndex, new QTableWidgetItem);
            }
        }
    }

    int accIndex{}, fieldIndex{};
    for (auto &it: answer) {
        accounts->item(accIndex, 0)->setText(QString::fromStdString(it[0]["account_number"].get<std::string>()));
        accounts->item(accIndex, 1)->setText(QString::fromStdString(it[0]["deposit"].get<std::string>()));
        accounts->item(accIndex, 2)->setText(QString::fromStdString(it[0]["date_and_time_of_opening"].get<std::string>()));
        accIndex++;
    }

    ui->tableWidget_accInfo->hide();
    ui->tableWidget_transactionHistory->hide();
    ui->tableWidget_accountList->show();
}
