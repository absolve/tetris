#ifndef TETRIS_H
#define TETRIS_H

#include <QWidget>
#include "tetrisbox.h"


namespace Ui {
class Tetris;
}

class QLCDNumber;
class QPushButton;

class Tetris : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tetris(QWidget *parent = 0);
    ~Tetris();
    void  paintEvent(QPaintEvent *);
    void  keyPressEvent(QKeyEvent *);
    void  move(int x,int y);
    void  rotate();
    void  boxPaint(QPainter*);
    void  nextBoxPaint(QPainter*);
    void  gameMessage(QPainter*);
    void  item();          //窗口部件
    void  checkFullLine();
    void  cleanAllFullLine();
    int   fullLine(int index)     { int a=0;  for(int i=0;i<10;i++){ a+=boxArea[index][i];}
                                  return a;  }  //判断是否满行
    void  setFullFlag(int index) { for(int i=0;i<10;i++) boxArea[index][i]=2; }
    void  setEmptyFlag(int index) {for(int i=0;i<10;i++) boxArea[index][i]=0;}
    bool  checkIsBOx(int y,int x) {if(boxArea[y][x]==1) return 1;else return 0;}
private slots:
    void  dropBox();
    void  newGame();
    void  restart();
    void  _about();
    void  changeRankAndSpeed(int);
signals:
    void  rankAndSpeedchange(int);
private:
    Ui::Tetris   *ui;
    tetrisbox    box;
    tetrisbox    nextBox;   //下一个方块
    QTimer      *time_1;    //计时器
    QPushButton *gameStartOrPause; //游戏开始和运行
    QPushButton *gameRestart;      //游戏重新开始
    QPushButton *about;            //游戏的关于
    int   rank;    //游戏等级
    int   scores;  //游戏分数
    int   reward;  //消除每行的奖励
    int   boxColor;        //下落后的方块的颜色
    int   dropSpeed;        //方块下落速度
    QLCDNumber   *scoreShow;
    enum  {state_1=0x00EE76,state_2=0x00BFFF,state_3=0x1C86EE};
    enum  {r1=50,r2=70,r3=100};  //不同的游戏等级获取的奖励
    int   boxSize;  //方块大小
    int   newX;
    int   newY;
    int   currX;
    int   currY;    
    int   boxMaxHeight;     //方块所在的最大高度    
    enum  {dropSpeed_1=1800,dropSpeed_2=1500,dropSpeed_3=1000};
    bool  gameOver;         //游戏结束
    bool  gamePause;        //游戏暂停
    enum  {height=320,width=200}; //游戏区域
    enum  {nextBoxX=280,nextBoxY=240}; //下一个方块显示位置
    int   boxArea[16][10];    //游戏区域的所有方块 (0 代表无方块 1 代表有方块  2 代表整行方块已满

};

#endif // TETRIS_H
