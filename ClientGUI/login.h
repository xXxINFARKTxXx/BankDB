#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"
#include "registration.h"
#include <QCloseEvent>

class ConnControler;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr, ConnControler* pListener = nullptr);
    ~Login();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_signUp_clicked();

    void closeEvent(QCloseEvent *event);

private:
    Ui::Login *ui;
    Registration* reg{};
    MainWindow* mw{};
    ConnControler *pListener{};
};

#endif // LOGIN_H
