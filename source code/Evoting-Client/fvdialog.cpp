#include "fvdialog.h"
#include "ui_fvdialog.h"

FvDialog::FvDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FvDialog)
{
    ui->setupUi(this);
    setConn();
}

FvDialog::~FvDialog()
{
    delete ui;
}

void FvDialog::setConn()
{
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));

}
