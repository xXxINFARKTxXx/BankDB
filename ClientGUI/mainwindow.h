#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

class Login;
class ConTransfDialog;
class ConnControler;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setConnController(ConnControler *cc);

    int ConductTransfer(QString &from, QString &to, QString &amount);

    int setUserID(QString user_id);

private slots:
    void on_actionLogOut_triggered();

    void closeEvent (QCloseEvent *event);

    void on_actionConductTransfer_triggered();

    void on_actionShow_userInfo_triggered();

    void on_actionOpenDebitAccount_triggered();

    void on_actionRefresh_accountList_triggered();

    void on_actionAboutProgram_triggered();

    void on_actionCloseDebitAccount_triggered();

    void on_actionShow_transactionHistory_triggered();

private:
    Ui::MainWindow *ui;
    Login* login{};
    ConTransfDialog* transfer{};
    ConnControler* cc{};
    QString user_id{};
};
#endif // MAINWINDOW_H
