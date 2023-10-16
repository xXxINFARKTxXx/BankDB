#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>

class ConnControler;

class Login;

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr, ConnControler* cc = nullptr);
    ~Registration();

private slots:
    void on_pushButton_signUp_clicked();

    void on_pushButton_cancel_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::Registration *ui;
    Login *login;
    ConnControler* cc;
};

#endif // REGISTRATION_H
