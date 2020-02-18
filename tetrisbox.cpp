#include "tetrisbox.h"
#include <stdlib.h>
#include <QtCore>
#include <ctime>


tetrisbox::tetrisbox()
{
    setShape();
}

void tetrisbox::setShape(){
    qsrand((unsigned)time(0));
    //随机颜色
    static const QRgb colorTable[7] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC
    };
    //方块逻辑坐标
    int   box[7][4][2]={
                         { {-2,0} ,{-1,0},{0,0},{1,0} }, // 直线方块
                         { {0,-1} ,{0,0},{1,0},{1,1}},    // z方块
                         { {0,0} ,{1,0},{1,-1},{0,1}},   //  z方块
                         { {-1,0} ,{0,0},{0,-1},{1,0}},    //  t方块
                         { {0,0} ,{1,0},{0,1},{1,1}},     //  方体方块
                         { {-1,1},{-1,0},{0,0},{1,0}},   //  L方块
                         { {1,1} ,{-1,0},{0,0},{1,0} }   //  L方块
                       };
    shape=qrand()%7;
    currentColor=colorTable[qrand()%7];
    for(int i=0;i<4;i++)
        for(int y=0;y<2;y++){
            currentbox[i][y]=box[shape][i][y];
        }
    //随机旋转一定的角度
    if(shape!=4){
    int r=qrand()%4;
    for(int i=0;i<r;i++){
        for(int j=0;j<4;j++){
            int x=-(this->getY(j));
            int y=this->getX(j);
            this->setX(j,x);
            this->setY(j,y);
        }
     }
    }
}

int  tetrisbox::getMinY(){
    int y=currentbox[0][1];
    for(int i=1;i<4;i++){
        y=qMin(y,currentbox[i][1]);
    }
    return y;
}

int  tetrisbox::getMaxY(){
    int y=currentbox[0][1];
    for(int i=1;i<4;i++){
        y=qMax(y,currentbox[i][1]);
    }
    return y;
}

tetrisbox::~tetrisbox(){}


