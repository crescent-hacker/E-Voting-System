#ifndef SERVERMAINWIN_H
#define SERVERMAINWIN_H
#include "clienthandler.h"
#include <QMainWindow>
#include <QAbstractListModel>
#include "vote.h"
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHBoxLayout>

namespace Ui {
class MainWin;
}

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWin(QWidget *parent = 0);
    ~MainWin();
    void setParameters();
    void setConn();
    //show votes list
    bool updateVoteList();
    bool showTableList();
    void setTableSet();


private:
    Ui::MainWin* ui;
    std::vector<Vote*> v;
    Vote detailVote;
    QTableWidgetItem* curItem;
signals:



public slots:
    void addListItemSlot();
    void deleteItemSlot();

    void exitProgramSlot();
    void pushButtonLinkSlot();

    void pushButtonPublishSlot();
    //table item slot
    void openVoteProgramSlot(QTableWidgetItem*);
    void chosenVoteSlot(QTableWidgetItem*);
};

extern ClientHandler* handler;

#endif // SERVERMAINWIN_H
