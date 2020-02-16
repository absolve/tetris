#include "tetris.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tetris w;
    w.show();
    
    return a.exec();
}
