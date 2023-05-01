#include "registration.h"
#include "qstyle.h"
#include "ui_registration.h"
#include "login.h"
#include "ConnControler.h"

#include "nlohmann/json.hpp"
#include <QMessageBox>>

using json = nlohmann::json;

Registration::Registration(QWidget *parent, ConnControler *cc) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    login = (Login *)parent;
    this->cc = cc;
}

Registration::~Registration()
{
    delete ui;
    login->show();
}

void Registration::on_pushButton_signUp_clicked()
{
    QString
        passport_id =               this->ui->lineEdit_passport->text(),
        first_name =                this->ui->lineEdit_name->text(),
        second_name =               this->ui->lineEdit_surname->text(),
        last_name =                 this->ui->lineEdit_lastName->text(),
        place_of_birth =            this->ui->textEdit_placeOfBirth->toPlainText(),
        country =                   this->ui->lineEdit_country->text(),
        passport_place_of_issue =   this->ui->textEdit_placeOfIssue->toPlainText(),
        passport_authority =        this->ui->textEdit_passportAuthority->toPlainText(),
        address_of_living =         this->ui->textEdit_adressOfLivivng->toPlainText(),
        email =                     this->ui->lineEdit_email->text();

    QString message = "Incorrect input!\n";

    QString BorderThickness("2");
    QString hexColorCode("#FF00FF");

    static const QRegularExpression pid_exp{R"(^[1-9][0-9]{0,}$)"};
    static const QRegularExpression email_exp{R"(^\S+@\S+\.\S+$)"};

    if(passport_id.isEmpty()) {
        message += "Passport ID field required to be filled\n";
        ui->passportID_frame->setStyleSheet("#" + ui->passportID_frame->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else if (!pid_exp.match(passport_id).hasMatch()) {
        message += "Passport ID field does not mach constraints\n";
        ui->passportID_frame->setStyleSheet("#" + ui->passportID_frame->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->passportID_frame->setStyleSheet("");

    if(first_name.isEmpty()) {
        message += "Name field required to be filled\n";
        ui->lineEdit_name->setStyleSheet("#" + ui->lineEdit_name->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->lineEdit_name->setStyleSheet("");

    if(second_name.isEmpty()) {
        message += "Surname field required to be filled\n";
        ui->lineEdit_surname->setStyleSheet("#" + ui->lineEdit_surname->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->lineEdit_surname->setStyleSheet("");

    if(last_name.isEmpty()) {
        message += "Last name field required to be filled\n";
        ui->lineEdit_lastName->setStyleSheet("#" + ui->lineEdit_lastName->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->lineEdit_lastName->setStyleSheet("");

    if(place_of_birth.isEmpty()) {
        message += "Place of birth field required to be filled\n";
        ui->textEdit_placeOfBirth->setStyleSheet("#" + ui->textEdit_placeOfBirth->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->textEdit_placeOfBirth->setStyleSheet("");

    if(country.isEmpty()) {
        message += "Country field required to be filled\n";
        ui->lineEdit_country->setStyleSheet("#" + ui->lineEdit_country->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->lineEdit_country->setStyleSheet("");

    if(passport_place_of_issue.isEmpty()) {
        message += "Passport place of issue field required to be filled\n";
        ui->textEdit_placeOfIssue->setStyleSheet("#" + ui->textEdit_placeOfIssue->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->textEdit_placeOfIssue->setStyleSheet("");

    if(passport_authority.isEmpty()) {
        message += "Passport authority field required to be filled\n";
        ui->textEdit_passportAuthority->setStyleSheet("#" + ui->textEdit_passportAuthority->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->textEdit_passportAuthority->setStyleSheet("");

    if(address_of_living.isEmpty()) {
        message += "Address of living field required to be filled\n";
        ui->textEdit_adressOfLivivng->setStyleSheet("#" + ui->textEdit_adressOfLivivng->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->textEdit_adressOfLivivng->setStyleSheet("");

    if(email.isEmpty()) {
        message += "Email field required to be filled\n";
        ui->lineEdit_email->setStyleSheet("#" + ui->lineEdit_email->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else if (!email_exp.match(email).hasMatch()) {
        message += "Email field does not mach constraints\n";
        ui->lineEdit_email->setStyleSheet("#" + ui->lineEdit_email->objectName() + " { border: 1px solid rgb(90, 0, 0); }");
    } else ui->lineEdit_email->setStyleSheet("");

    if(!(this->ui->radioButton_male->isChecked() || this->ui->radioButton_female->isChecked())){
        message += "choose male or female as their gender\n";
        ui->sex_frame->setStyleSheet("#" + ui->sex_frame->objectName() + "{ border: 1px solid rgb(90, 0, 0); }");
    } else ui->sex_frame->setStyleSheet("");


    if(message != "Incorrect input!\n") {
        QMessageBox::warning(this, "MVVM Bank Client", message);
    } else {
        json registration = {
            {"type",                    1},
            {"passport_id",             passport_id.toStdString().c_str()},
            {"first_name",              first_name.toStdString().c_str()},
            {"second_name",             second_name.toStdString().c_str()},
            {"last_name",               last_name.toStdString().c_str()},
            {"date_of_birth",           this->ui->dateEdit_dob->date().toString("yyyy-MM-dd").toStdString().c_str()},
            {"place_of_birth",          place_of_birth.toStdString().c_str()},
            {"country",                 country.toStdString().c_str()},
            {"passport_date_of_issue",  this->ui->dateEdit_dateOfIssue->date().toString("yyyy-MM-dd").toStdString().c_str()},
            {"passport_date_of_expire", this->ui->dateEdit_dateOfExpire->date().toString("yyyy-MM-dd").toStdString().c_str()},
            {"passport_place_of_issue", passport_place_of_issue.toStdString().c_str()},
            {"passport_authority",      passport_authority.toStdString().c_str()},
            {"address_of_living",       address_of_living.toStdString().c_str()},
            {"email",                   email.toStdString().c_str()},
            {"sex",                     this->ui->radioButton_male->isChecked()}
        };

        cc->sendMessage(registration.dump());
        json answer;
        try {
            answer = json::parse(cc->getMessage());
        }
        catch (std::exception &e) {
            QMessageBox::information(this, "", e.what());
            return;
        }

        if(answer["result"]) {
            auto name = QString::fromStdString(answer["name"]);
            auto surname = QString::fromStdString(answer["surname"]);
            auto login = QString::fromStdString(answer["login"]);
            auto password = QString::fromStdString(answer["password"]);
            QMessageBox::information(this, "MVVM Bank Client",
                                     "Registration Success!\n"
                                     "Welcome aboard, " + name + " " + surname + "!\n"
                                     "Your login: " + login + "\n"
                                     "And password " + password + "\n");
            this->login->show();
            close();
        } else {
            QMessageBox::warning(this, "MVVM Bank Client", "Wrong personal data input!\n");
        }
    }
}


void Registration::on_pushButton_cancel_clicked()
{
    login->show();
    close();
}

void Registration::closeEvent (QCloseEvent *event)
{
    login->show();
    event->accept();
}
