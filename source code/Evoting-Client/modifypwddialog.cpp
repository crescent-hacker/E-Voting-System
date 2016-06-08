#include "modifypwddialog.h"
#include "ui_modifypwddialog.h"
#include "usermainwindow.h"
#include <QRegExp>
#include <QMessageBox>

ModifyPwdDialog::ModifyPwdDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyPwdDialog)
{
    ui->setupUi(this);
    ui->labelUsername->setText(QString(handler->getUser()->getUserName().c_str()));
    //test
    if(handler->getUser()->getUserName()==std::string("laolv")){
        ui->lineEditPwd->setText("@JCYZ421lzj");
        ui->lineEditRetypePwd->setText("@JCYZ421lzj");
    }
    setConn();

}

ModifyPwdDialog::~ModifyPwdDialog()
{
    delete ui;
}

void ModifyPwdDialog::setConn()
{
    connect(ui->pushButtonOk,SIGNAL(pressed()),this,SLOT(pushButtonOkSlot()));
    connect(ui->pushButtonCancel,SIGNAL(pressed()),this,SLOT(pushButtonCancelSlot()));
    connect(ui->pushButtonReset,SIGNAL(pressed()),this,SLOT(pushButtonResetSlot()));
}

void ModifyPwdDialog::pushButtonOkSlot()
{
    CheckPwd();
    if(handler->ModifyPwdReq()==false)
        std::cout << "modify pwd fail" << std::endl;
    else{
        UserMainWindow* window=new UserMainWindow();
        window->show();
        close();
    }
}

void ModifyPwdDialog::pushButtonResetSlot()
{
    ui->lineEditPwd->setText("");
    ui->lineEditRetypePwd->setText("");
}

void ModifyPwdDialog::pushButtonCancelSlot()
{

}

void ModifyPwdDialog::CheckPwd()
{
    std::string str1=ui->lineEditPwd->text().toStdString();
    std::string str2=ui->lineEditRetypePwd->text().toStdString();
    if(str1==str2){
        if(isPwdSimple(str1)==true){
            QMessageBox* box=new QMessageBox(QMessageBox::Warning,"simple password","password is too simple");
            box->show();
        }
        else
            handler->getUser()->setPwd(str1);
    }
    else{
        QMessageBox* box=new QMessageBox(QMessageBox::Warning,"password","two passwords are not matched");
        box->show();
    }
}

bool ModifyPwdDialog::isPwdSimple(std::string pwd)
{
    QString strPwd=QString(pwd.c_str());
    if(strPwd.length()<8)
        return true;
    int count=0;
    if(strPwd.contains(QRegExp("([a-z]+)")))
        count++;
    if(strPwd.contains(QRegExp("([0-9]+)")))
        count++;
    if(strPwd.contains(QRegExp("([A-Z]+)")))
        count++;
    if(strPwd.contains(QRegExp("([^a-zA-Z0-9]+)")))
        count++;
    if(count<4)
        return true;
    else
        return false;
}
