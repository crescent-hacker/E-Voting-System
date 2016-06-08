#ifndef MODIFYPWDDIALOG_H
#define MODIFYPWDDIALOG_H

#include <QDialog>
#include "clienthandler.h"

namespace Ui {
class ModifyPwdDialog;
}

class ModifyPwdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyPwdDialog(QWidget *parent = 0);
    ~ModifyPwdDialog();
    void setConn();
    void CheckPwd();
    bool isPwdSimple(std::string pwd);

private:
    Ui::ModifyPwdDialog *ui;

private slots:
    void pushButtonOkSlot();
    void pushButtonCancelSlot();
    void pushButtonResetSlot();


};

extern ClientHandler* handler;

#endif // MODIFYPWDDIALOG_H
