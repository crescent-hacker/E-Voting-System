#include "adddialog.h"
#include "ui_adddialog.h"
#include "client_https.hpp"
#include "clienthandler.h"
#include <boost/lexical_cast.hpp>
#include <QDebug>
#include <QDateTime>
#include <QCheckBox>
#include <iostream>
#include <clienthandler.h>

//using namespace boost::property_tree;
typedef SimpleWeb::Client<SimpleWeb::HTTPS> HttpsClient;

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    setConn();
    //display cand usr
    showUsers();
    //show current date
    QDateTime currentTime=QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(currentTime);
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::setConn()
{
    //button conn
    connect(ui->pushButtonOk,SIGNAL(pressed()),this,SLOT(pushButtonOkSlot()));
    connect(ui->pushButtonAdd,SIGNAL(pressed()),this,SLOT(pushButtonAddSlot()));
    connect(ui->pushButtonReset,SIGNAL(pressed()),this,SLOT(pushButtonResetSlot()));
//    connect(ui->dateTimeEdit,SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(getMyTimeSlot(QDateTime)));

}

void AddDialog::pushButtonOkSlot()
{
    //title
    handler->getVote()->setTitle(ui->lineEditTitle->text().toStdString());
    getContent();
    countUsers();
    getMyTime();
    getCurrentTime();
    if(handler->SendVoteReq()==false)
        std::cout << "send vote fail" << std::endl;
    this->close();
}

void AddDialog::pushButtonResetSlot()
{
    handler->getVote()->getCandVec().clear();
    handler->getVote()->getUserVec().clear();
    ui->listWidgetCand->clear();
    ui->listWidgetUsr->clear();
    ui->textEditContent->setText("");
}

void AddDialog::pushButtonAddSlot()
{
    std::string str=ui->lineEditCand->text().toStdString();
    ui->listWidgetCand->addItem(QString(str.c_str()));
    handler->getVote()->getCandVec().push_back(str);
    //clear lineEdit
    ui->lineEditCand->setText("");
}

void AddDialog::showUsers()
{
    handler->CheckActivatedUsersListReq(handler->getVote()->getUserVec());
    QCheckBox* checkbox;
    QListWidgetItem* listItem;
    std::vector<std::string>::iterator it=handler->getVote()->getUserVec().begin();
    for(;it!=handler->getVote()->getUserVec().end();++it){
        checkbox=new QCheckBox(QString(it->c_str()));
        checkbox->setStyleSheet("color:green;font-size:13px");
        listItem=new QListWidgetItem(ui->listWidgetUsr);
        ui->listWidgetUsr->addItem(listItem);
        ui->listWidgetUsr->setItemWidget(listItem,checkbox);
    }
}

void AddDialog::getMyTime()
{
    ui->dateTimeEdit->setDisplayFormat("yyyy/MM/dd hh:mm:ss");
    QString str=ui->dateTimeEdit->text();
    handler->getVote()->setDeadline(str.toStdString());
//    std::cout << str.toStdString();
}

void AddDialog::getContent()
{
    handler->getVote()->setContent(ui->textEditContent->toPlainText().toStdString());
}

void AddDialog::countCands()
{

}

void AddDialog::countUsers()
{
    std::vector<std::string> userVec;
    handler->getVote()->setUserVec(userVec);
    int len=ui->listWidgetUsr->count();
    QListWidgetItem* item;
    for(int i=0;i<len;++i){
        item=ui->listWidgetUsr->item(i);
        QCheckBox* checkbox=(QCheckBox*)ui->listWidgetUsr->itemWidget(item);
        if(checkbox->isChecked()){
            handler->getVote()->getUserVec().push_back(checkbox->text().toStdString());
        }
    }
}

void AddDialog::getCurrentTime()
{
    QDateTimeEdit* edit=new QDateTimeEdit(QDateTime::currentDateTime());
    edit->setDisplayFormat("yyyy/MM/dd hh:mm:ss");
    QString str=edit->text();
    handler->getVote()->setPublishDate(str.toStdString());
}
