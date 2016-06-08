#ifndef ADDUSERDIALOG_H
#define ADDUSERDIALOG_H

#include <QDialog>
#include "clienthandler.h"

namespace Ui {
class AddUserDialog;
}

class AddUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddUserDialog(QWidget *parent = 0);
    ~AddUserDialog();
    bool ReqUsersList();
    void UpdateAddUserWidget();
    void UpdateActivateUserWidget();
    void UpdateForbidUserWidget();
    void SetConn();
    void CheckActivatedUser();
    void CheckNActivatedUser();

private slots:
    void pushButtonAddUserOkSlot();
    void pushButtonForbidSlot();
    void pushButtonActivateSlot();
    void pushButtonOkSlot();

private:
    Ui::AddUserDialog *ui;
    std::vector<std::string> vAllUsers;
    std::vector<std::string> vActivatedUsers;
    std::vector<std::string> vNActivatedUsers;
    //req vectors
    std::vector<std::string> vActivateUsrReq;
    std::vector<std::string> vNActivateUsrReq;
};

extern ClientHandler* handler;
#endif // ADDUSERDIALOG_H
