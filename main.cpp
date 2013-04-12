#include "xsdread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XSDRead w;
    w.show();
    
    return a.exec();
}
