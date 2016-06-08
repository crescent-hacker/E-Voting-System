#include "loginwindow.h"
#include <QApplication>
#include "clienthandler.h"
#include "readconf.h"
#include <map>
#include <QFile>

//global variable
ClientHandler* handler;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    w.show();
    return a.exec();
}
