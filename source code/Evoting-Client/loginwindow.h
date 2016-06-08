#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "clienthandler.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    QString pathDir;

    void setConn();

private:
    Ui::LoginWindow *ui;

public slots:
    void pushButtonLoginSlot();
    void pushButtonResetSlot();
    void pushButtonCertPathSlot();

};

extern ClientHandler* handler;

#endif // LOGINWINDOW_H
