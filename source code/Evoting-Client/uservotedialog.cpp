#include "uservotedialog.h"
#include "ui_uservotedialog.h"

UserVoteDialog::UserVoteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserVoteDialog)
{
    ui->setupUi(this);
}

UserVoteDialog::~UserVoteDialog()
{
    delete ui;
}

void UserVoteDialog::setTableSet()
{
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("user name"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("challenge field"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("response field"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("proof field"));
    ui->tableWidget->setHorizontalHeaderItem(4,new QTableWidgetItem("final ballot"));


//    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
