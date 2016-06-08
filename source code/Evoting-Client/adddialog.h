#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVector>
#include "clienthandler.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:

    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();
    void setConn();

    void showUsers();
    void getContent();
    void countCands();
    void countUsers();
    void getMyTime();
    void getCurrentTime();

signals:

private:
    Ui::AddDialog *ui;

public slots:
    void pushButtonOkSlot();
    void pushButtonResetSlot();
    void pushButtonAddSlot();
//    void getMyTimeSlot(QDateTime);
};

extern ClientHandler* handler;

#endif // ADDDIALOG_H
