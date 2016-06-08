#include "loginwindow.h"
#include "usermainwindow.h"
#include "ui_loginwindow.h"
#include "readconf.h"
#include "mainwin.h"
#include <QFileDialog>
#include "modifypwddialog.h"
#include <QMessageBox>
#include <QDebug>
#include <vector>
#include <map>
#include "clienthandler.h"
#include "usermainwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    //set connections
    setConn();
    //test
    ui->lineEditUsername->setText("stefan");
    ui->lineEditpwd->setText("stefan");
//    ui->lineEditUsername->setText("laolv");
//    ui->lineEditpwd->setText("@JCYZ421lzj");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::setConn()
{
    connect(ui->pushButtonLogin,SIGNAL(pressed()),this,SLOT(pushButtonLoginSlot()));
    connect(ui->pushButtonReset,SIGNAL(pressed()),this,SLOT(pushButtonResetSlot()));
    connect(ui->pushButtonCertPath,SIGNAL(pressed()),this,SLOT(pushButtonCertPathSlot()));
}

void LoginWindow::pushButtonLoginSlot()
{
    QString usrName=ui->lineEditUsername->text();
    QString pwd=ui->lineEditpwd->text();
    //admin
    handler->getUser()->setUserName(usrName.toStdString());
    handler->getUser()->setPwd(pwd.toStdString());
    handler=new ClientHandler(pathDir,usrName,pwd);

//    handler=new ClientHandler("/Users/ios/Desktop/test-certs",usrName,pwd);
    close();
}

void LoginWindow::pushButtonResetSlot()
{
    ui->lineEditUsername->setText("");
    ui->lineEditpwd->setText("");
}


void LoginWindow::pushButtonCertPathSlot()
{
    QFileDialog* dlg=new QFileDialog(this,"certificate path");
    pathDir=QFileDialog::getExistingDirectory();
    ui->lineEditCertPath->setText(pathDir);
    dlg->show();
}

