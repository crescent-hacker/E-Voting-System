#ifndef USERMAINWINDOW_H
#define USERMAINWINDOW_H

#include <QDialog>
#include "clienthandler.h"
#include <QTableWidgetItem>
#include <QTableWidget>

namespace Ui {
class UserMainWindow;
}

class UserMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserMainWindow(QWidget *parent = 0);
    ~UserMainWindow();
    void setConn();
    void setTableSet();

private slots:
    void openVoteProgram(QTableWidgetItem*);

private:
    Ui::UserMainWindow *ui;
};

extern ClientHandler* handler;

#endif // USERMAINWINDOW_H
