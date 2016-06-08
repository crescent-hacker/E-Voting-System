#ifndef USERVOTEDIALOG_H
#define USERVOTEDIALOG_H

#include <QDialog>

namespace Ui {
class UserVoteDialog;
}

class UserVoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserVoteDialog(QWidget *parent = 0);
    ~UserVoteDialog();
    void setTableSet();

private:
    Ui::UserVoteDialog *ui;
};

#endif // USERVOTEDIALOG_H
