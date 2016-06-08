#include "usermainwindow.h"
#include "ui_usermainwindow.h"
#include "uservotedialog.h"

UserMainWindow::UserMainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserMainWindow)
{
    ui->setupUi(this);
    ui->labelUserName->setText(QString(handler->getUser()->getUserName().c_str()));
    setTableSet();
}

UserMainWindow::~UserMainWindow()
{
    delete ui;
}

void UserMainWindow::setConn()
{
    connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,
            SLOT(openVoteProgram()));
}

void UserMainWindow::setTableSet()
{
    ui->tableWidget->setHorizontalHeaderItem(0,new QTableWidgetItem("vote_id"));
    ui->tableWidget->setHorizontalHeaderItem(1,new QTableWidgetItem("title"));
    ui->tableWidget->setHorizontalHeaderItem(2,new QTableWidgetItem("deadline"));
    ui->tableWidget->setHorizontalHeaderItem(3,new QTableWidgetItem("status"));

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void UserMainWindow::openVoteProgram(QTableWidgetItem* item)
{
    UserVoteDialog* dlg=new UserVoteDialog();
    dlg->show();
}
