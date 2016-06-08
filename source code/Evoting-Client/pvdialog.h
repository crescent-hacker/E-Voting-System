#ifndef PVDIALOG_H
#define PVDIALOG_H

#include <QDialog>

namespace Ui {
class PvDialog;
}

class PvDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PvDialog(QWidget *parent = 0);
    ~PvDialog();
    void setConn();

private:
    Ui::PvDialog *ui;
};

#endif // PVDIALOG_H
