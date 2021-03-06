﻿#ifndef TETRISBOX_H
#define TETRISBOX_H
#include <QColor>

/**
 * 这个类用来表示每个方块的信息
 * 每个方块都是以坐标原点为中心，每个长度为1的方块
 * 方块的旋转都是更改它的坐标实现
 * 
 * */
class tetrisbox
{
public:
    tetrisbox();
    ~tetrisbox();

    int   getX(int index) {return currentbox[index][0];}
    int   getY(int index) {return currentbox[index][1];}
    void  setShape();
    void  setX(int index,int x){currentbox[index][0]=x;}
    void  setY(int index,int y){currentbox[index][1]=y;}
    int   getShape() {return shape;}
    QColor getColor() {return currentColor;}
    int   getMinY();   //判断y轴最小方块是否进入游戏区域,如果没有,则游戏结束
    int   getMaxY();   //判断y轴最大的方块
private:
    int  shape;
    int  currentbox[4][2];
    QColor currentColor;
};

#endif // TETRISBOX_H
