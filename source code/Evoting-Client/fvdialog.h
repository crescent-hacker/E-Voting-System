#ifndef FVDIALOG_H
#define FVDIALOG_H

#include <QDialog>

namespace Ui {
class FvDialog;
}

class FvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FvDialog(QWidget *parent = 0);
    ~FvDialog();
    void setConn();

private:
    Ui::FvDialog *ui;
};

#endif // FVDIALOG_H
