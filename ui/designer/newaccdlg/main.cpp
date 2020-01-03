#include "newaccdlg.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    newaccdlg w;
    w.show();
    return a.exec();
}
