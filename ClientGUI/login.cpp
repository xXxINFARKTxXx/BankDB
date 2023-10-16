#include "login.h"
#include "ui_login.h"
#include "ConnControler.h"

#include <QMessageBox>
#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

Login::Login(QWidget *parent, ConnControler *pListener) :
    QDialog(parent),
    ui(new Ui::Login),
    pListener{pListener}
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete pListener;
    delete reg;
    delete mw;
    delete ui;
}

void Login::on_pushButton_login_clicked()
{
    QString login = this->ui->lineEdit_username->text();
    QString password = this->ui->lineEdit_password->text();

    if(login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Auth error", "Wrong username or password.\n");
        return;
    }
    json auth = {
        {"type",     0},
        {"login",    login.toStdString().c_str()},
        {"password", password.toStdString().c_str()}
    };

    pListener->sendMessage(auth.dump());

    auto answer = json::parse(pListener->getMessage());

    if(answer["result"]) {

        QString message {
            QString::fromStdString("Welcome, " + answer["name"].get<std::string>() +
                                   " " + answer["surname"].get<std::string>()) + "!\n"
            };
        QMessageBox::information(this, "Auth success", message);

        delete mw;
        mw = nullptr;
        delete reg;
        reg = nullptr;
        this->ui->lineEdit_password->setText("");
        this->ui->lineEdit_username->setText("");
        mw = new MainWindow(this);
        mw->setConnController(this->pListener);
        mw->setUserID(QString::number(answer["user_id"].get<long long>()));
        mw->show();
        hide();
    } else {
        QMessageBox::warning(this, "Auth error", "Wrong username or password.\n");
    }
}

void Login::on_pushButton_signUp_clicked()
{
    if(reg == nullptr)
        reg = new Registration(this, pListener);
    reg->show();
    hide();
}

void Login::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "MVVM Bank Client",
                                                               tr("Are you sure?\n"),
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }
}

