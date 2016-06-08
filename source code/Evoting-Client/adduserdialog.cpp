#include "adduserdialog.h"
#include "ui_adduserdialog.h"
#include "clienthandler.h"
#include <QCheckBox>

AddUserDialog::AddUserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddUserDialog)
{
    ui->setupUi(this);
    ReqUsersList();
    //update widgets
    UpdateAddUserWidget();
    UpdateActivateUserWidget();
    UpdateForbidUserWidget();
//    std::cout << vAllUsers[0] << std::endl;
//    std::cout << vActivatedUsers[0] << std::endl;
//    std::cout << vNActivatedUsers[0] << std::endl;
    SetConn();
}

AddUserDialog::~AddUserDialog()
{
    delete ui;
}

bool AddUserDialog::ReqUsersList()
{
    int count=0;
    this->vAllUsers.clear();
    this->vActivatedUsers.clear();
    this->vNActivatedUsers.clear();

    if(handler->CheckAllUsersListReq(this->vAllUsers)==true)
        count++;
    if(handler->CheckActivatedUsersListReq(this->vActivatedUsers)==true)
        count++;
    if(handler->CheckNActivatedUsersListReq(this->vNActivatedUsers)==true)
        count++;
    if(count==3)
        return true;
    else
        return false;
}



void AddUserDialog::SetConn()
{
    connect(ui->pushButtonAddUserOk,SIGNAL(pressed()),this,SLOT(pushButtonAddUserOkSlot()));
    connect(ui->pushButtonOk,SIGNAL(pressed()),this,SLOT(pushButtonOkSlot()));
    connect(ui->pushButtonForbidden,SIGNAL(pressed()),this,SLOT(pushButtonForbidSlot()));
    connect(ui->pushButtonActivate,SIGNAL(pressed()),this,SLOT(pushButtonActivateSlot()));
}

void AddUserDialog::pushButtonAddUserOkSlot()
{
    bool ret=handler->AddUserReq("username","email",ui->lineEditName->text().toStdString(),
                        ui->lineEditEmail->text().toStdString());
    if(ret==false){
        std::cout << "add user fail" << std::endl;
    }
    ReqUsersList();
    UpdateAddUserWidget();
    UpdateActivateUserWidget();
    UpdateForbidUserWidget();
    ui->lineEditName->setText("");
    ui->lineEditEmail->setText("");
}

void AddUserDialog::pushButtonOkSlot()
{
    this->close();
}

void AddUserDialog::UpdateAddUserWidget()
{
    ui->listWidgetAddUser->clear();
    QLabel* label;
    QListWidgetItem* listItem;
    std::vector<std::string>::iterator it=vAllUsers.begin();
    for(;it!=vAllUsers.end();++it){
        label=new QLabel(it->c_str());
        label->setStyleSheet("color:green;font-size:13px");
        listItem=new QListWidgetItem(ui->listWidgetAddUser);
        ui->listWidgetAddUser->addItem(listItem);
        ui->listWidgetAddUser->setItemWidget(listItem,label);
    }
}

void AddUserDialog::UpdateActivateUserWidget()
{
    ui->listWidgetActivateUser->clear();
    QCheckBox* checkbox;
    QListWidgetItem* listItem;
    //display list
    std::vector<std::string>::iterator it=vActivatedUsers.begin();
    for(;it!=vActivatedUsers.end();++it){
        checkbox=new QCheckBox(it->c_str());
        checkbox->setStyleSheet("color:green;font-size:13px");
        listItem=new QListWidgetItem(ui->listWidgetActivateUser);
        ui->listWidgetActivateUser->addItem(listItem);
        ui->listWidgetActivateUser->setItemWidget(listItem,checkbox);
    }
}

void AddUserDialog::UpdateForbidUserWidget()
{
    ui->listWidgetForbidUser->clear();
    QCheckBox* checkbox;
    QListWidgetItem* listItem;
    //display list
    std::vector<std::string>::iterator it=vNActivatedUsers.begin();
    for(;it!=vNActivatedUsers.end();++it){
        checkbox=new QCheckBox(it->c_str());
        checkbox->setStyleSheet("color:green;font-size:13px");
        listItem=new QListWidgetItem(ui->listWidgetForbidUser);
        ui->listWidgetForbidUser->addItem(listItem);
        ui->listWidgetForbidUser->setItemWidget(listItem,checkbox);
    }
}

void AddUserDialog::CheckActivatedUser()
{
    vActivateUsrReq.clear();
    int len=ui->listWidgetActivateUser->count();
    QListWidgetItem* curItem;
    QCheckBox* checkbox;
    for(int i=0;i<len;++i){
        curItem=ui->listWidgetActivateUser->item(i);
        checkbox=(QCheckBox*)ui->listWidgetActivateUser->itemWidget(curItem);
        if(checkbox->isChecked())
            this->vActivateUsrReq.push_back(checkbox->text().toStdString());
    }
    std::cout << "vActivateUsrReq" << vActivateUsrReq[0] << std::endl;
}

void AddUserDialog::CheckNActivatedUser()
{
    vNActivateUsrReq.clear();
    int len=ui->listWidgetForbidUser->count();
    QListWidgetItem* curItem;
    QCheckBox* checkbox;
    for(int i=0;i<len;++i){
        curItem=ui->listWidgetForbidUser->item(i);
        checkbox=(QCheckBox*)ui->listWidgetForbidUser->itemWidget(curItem);
        if(checkbox->isChecked())
            this->vNActivateUsrReq.push_back(checkbox->text().toStdString());
    }
    std::cout << "vNActivateUsrReq" << vNActivateUsrReq[0] << std::endl;
}

void AddUserDialog::pushButtonForbidSlot()
{
    CheckActivatedUser();
    handler->ForbidUserReq(this->vActivateUsrReq);
    ReqUsersList();

    UpdateForbidUserWidget();
    UpdateActivateUserWidget();
}

void AddUserDialog::pushButtonActivateSlot()
{
    CheckNActivatedUser();
    handler->ActivateUserReq(this->vNActivateUsrReq);
    ReqUsersList();

    UpdateForbidUserWidget();
    UpdateActivateUserWidget();
}
