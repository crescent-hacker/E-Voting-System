#include "mainwin.h"
#include "ui_mainwin.h"
#include "pvdialog.h"
#include "fvdialog.h"
#include "adddialog.h"
#include "adduserdialog.h"
#include <QDebug>
#include <QStandardItemModel>
#include <QListView>
#include <QException>
#include <QTableWidget>
#include <QTableWidgetItem>

MainWin::MainWin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWin)
{
     setWindowTitle(tr("Stacked Widgets"));
     ui->setupUi(this);

    //table
    setTableSet();
//    showTableList();


    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertRow(1);
    ui->tableWidget->insertRow(2);
    ui->tableWidget->insertRow(3);

    ui->tableWidget->setItem(1,1,new QTableWidgetItem("1"));
    ui->tableWidget->setItem(2,1,new QTableWidgetItem("2"));
    ui->tableWidget->setItem(3,1,new QTableWidgetItem("3"));

    setConn();
}

MainWin::~MainWin()
{
    delete ui;
}

void MainWin::setParameters()
{

}

void MainWin::setConn()
{
    //tablewidget chosen connections
    connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),
            this,SLOT(openVoteProgramSlot(QTableWidgetItem*)));
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),
            this,SLOT(chosenVoteSlot(QTableWidgetItem*)));
    //button connections
    connect(ui->pushButtonAdd,SIGNAL(pressed()),this,SLOT(addListItemSlot()));
    connect(ui->pushButtonExit,SIGNAL(pressed()),this,SLOT(exitProgramSlot()));
    connect(ui->pushButtonDel,SIGNAL(pressed()),this,SLOT(deleteItemSlot()));
    connect(ui->pushButtonLink,SIGNAL(pressed()),this,SLOT(pushButtonLinkSlot()));
    connect(ui->pushButtonPublish,SIGNAL(pressed()),this,SLOT(pushButtonPublishSlot()));
}


void MainWin::addListItemSlot()
{
    QDialog* dlg=new AddDialog();
    dlg->show();
}

//delete item slot
void MainWin::deleteItemSlot()
{

    //delete item in database
}

void MainWin::exitProgramSlot()
{
    handler->EraseUserInfo();
    this->close();
}

void MainWin::pushButtonLinkSlot()
{
    AddUserDialog* dlg=new AddUserDialog();
    dlg->show();
}

bool MainWin::updateVoteList()
{

}

bool MainWin::showTableList()
{
    //clear
    this->v.clear();
    //request
    if(handler->UpDateGetVoteListInfo(this->v)==false)
        return false;
    else
        return true;
}

void MainWin::pushButtonPublishSlot()
{

}

void MainWin::openVoteProgramSlot(QTableWidgetItem*)
{
    handler->UpDateGetVoteDetailInfo(this->detailVote);
}

void MainWin::chosenVoteSlot(QTableWidgetItem* item)
{
    this->curItem=item;
}

void MainWin::setTableSet()
{
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Vote ID"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Deadline"));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Publish Status"));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Finished"));
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidgetNoPublish->setHorizontalHeaderItem(0, new QTableWidgetItem("Vote ID"));
    ui->tableWidgetNoPublish->setHorizontalHeaderItem(1, new QTableWidgetItem("Title"));
    ui->tableWidgetNoPublish->setHorizontalHeaderItem(2, new QTableWidgetItem("Deadline"));
    ui->tableWidgetNoPublish->setHorizontalHeaderItem(3, new QTableWidgetItem("Publish"));
    ui->tableWidgetNoPublish->setHorizontalHeaderItem(4, new QTableWidgetItem("Finished"));
    ui->tableWidgetNoPublish->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
