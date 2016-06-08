#include "pvdialog.h"
#include "ui_pvdialog.h"

PvDialog::PvDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PvDialog)
{
    ui->setupUi(this);
    setConn();
}

PvDialog::~PvDialog()
{
    delete ui;
}

void PvDialog::setConn()
{
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),ui->stackedWidget,SLOT(setCurrentIndex(int)));
}
