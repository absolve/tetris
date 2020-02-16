#include "tetris.h"
#include "ui_tetris.h"
#include "tetrisbox.h"
#include <QPainter>
#include <stdlib.h>
#include <ctime>
#include <QtGui>
#include <QLCDNumber>
#include <QPushButton>
#include <QMessageBox>

Tetris::Tetris(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tetris)
{
    ui->setupUi(this);
    this->setFixedSize(480,320);
    this->setWindowTitle(QString("Tetris v1.0 by king"));
    this->setWindowIcon(QIcon(":/Tetris.ico"));


    boxSize=20;
    newX=5*boxSize;
    newY=-2*boxSize;   
    currX=newX;
    currY=newY;
    gamePause=true;
    box=nextBox;
    nextBox.setShape();
    rank=1;
    scores=0;


    boxColor=state_1;
    dropSpeed=dropSpeed_1;
    reward=r1;
    gameOver=false;

    time_1=new QTimer(this);
    connect(time_1,SIGNAL(timeout()),this,SLOT(dropBox()));
    item();
    //将区域内的方块情空
    for(int i=0;i<16;i++)
        for(int j=0;j<10;j++){
            boxArea[i][j]=0;
        }

}

void Tetris::newGame(){
    gamePause=!gamePause;
    if(gamePause){
        time_1->stop();
        gamePause=true;
        gameStartOrPause->setIcon(QIcon(":/stop.ico"));

    }else{
        time_1->start(dropSpeed);
        gamePause=false;
        gameStartOrPause->setIcon(QIcon(":/play.ico"));
    }
}

void Tetris::restart(){
    box.setShape();
    nextBox.setShape();
    newX=5*boxSize;
    newY=-2*boxSize;
    currX=newX;
    currY=newY;
    gameOver=false;
    gamePause=false;
    rank=1;
    scores=0;    
    boxColor=state_1;
    reward=r1;
    for(int i=0;i<16;i++)
        for(int j=0;j<10;j++){
            boxArea[i][j]=0;
        }
    scoreShow->display(scores);
    update();
}

void Tetris::_about(){
    QMessageBox::about(this,tr("About Tetris v1.0"),
                       tr("<h2> Tetris v1.0 </h2>"
                          "<p>Copyright &copy; 2013.7.24 SoftWare."
                          "<p>It is my first simple game program."
                          "<p>nothing to say."
                          "<p>space is rotate."));

}

void Tetris::item(){
    scoreShow=new  QLCDNumber(8,this);
    scoreShow->setSegmentStyle(QLCDNumber::Filled);
    scoreShow->setGeometry(width+2*boxSize,2*boxSize,6*boxSize,2*boxSize);

    gameRestart =new QPushButton(QIcon(":/refresh.ico"),"",this);
    gameRestart->setIconSize(QSize(2*boxSize,2*boxSize));
    gameRestart->setGeometry(400,6*boxSize,2*boxSize,2*boxSize);
    gameRestart->setFocusPolicy(Qt::NoFocus);
    connect(gameRestart,SIGNAL(clicked()),this,SLOT(restart()));

    gameStartOrPause =new QPushButton(QIcon(":/stop.ico"),"",this);
    gameStartOrPause->setIconSize(QSize(2*boxSize,2*boxSize));
    gameStartOrPause->setGeometry(400,4*boxSize,2*boxSize,2*boxSize);
    gameStartOrPause->setFocusPolicy(Qt::NoFocus);
    connect(gameStartOrPause,SIGNAL(clicked()),this,SLOT(newGame()));

    about =new QPushButton(QIcon(":/help.ico"),"",this);
    about->setIconSize(QSize(2*boxSize,2*boxSize));
    about->setGeometry(400,8*boxSize,2*boxSize,2*boxSize);
    about->setFocusPolicy(Qt::NoFocus);
    connect(about,SIGNAL(clicked()),this,SLOT(_about()));
}

void Tetris::paintEvent(QPaintEvent *){
    QPainter  paint(this);

    //绘制游戏一些信息
    gameMessage(&paint);

    if(gameOver){
        time_1->stop();
        paint.setPen(QPen(Qt::black));
        paint.setFont(QFont("Times", 20, QFont::Bold));
        paint.drawText(1,2*boxSize,10*boxSize-1,4*boxSize,Qt::AlignCenter|Qt::TextExpandTabs,QString("Game over"));
        return;
    }
    paint.setPen(QPen(Qt::white));
    //绘制区域方块
    for(int i=0;i<16;i++)
        for(int j=0;j<10;j++){
            if(boxArea[i][j]==1){
                paint.fillRect(j*boxSize,i*boxSize,boxSize,boxSize,QColor(boxColor));
                paint.drawLine(j*boxSize-1,i*boxSize+boxSize-1,j*boxSize+boxSize,i*boxSize+boxSize-1);
                paint.drawLine(j*boxSize+boxSize-1,i*boxSize,j*boxSize+boxSize,i*boxSize+boxSize);
            }
        }

    //当前方块
    boxPaint(&paint);
    //绘制下一个方块
    nextBoxPaint(&paint);

}

void Tetris::boxPaint(QPainter *painter){

    for(int i=0;i<4;i++){

        painter->fillRect(currX+box.getX(i)*boxSize,currY+box.getY(i)*boxSize,boxSize,boxSize,box.getColor());
        painter->drawLine(currX+box.getX(i)*boxSize+boxSize-1,currY+box.getY(i)*boxSize-1,
                          currX+box.getX(i)*boxSize+boxSize-1,currY+box.getY(i)*boxSize+boxSize-1);
        painter->drawLine(currX+box.getX(i)*boxSize-1,currY+box.getY(i)*boxSize+boxSize-1,
                          currX+box.getX(i)*boxSize+boxSize-1,currY+box.getY(i)*boxSize+boxSize-1);
    }
}

void Tetris::gameMessage(QPainter *paint){
    paint->setPen(QPen(Qt::black));
    paint->drawLine(width,0,width,height);
    paint->drawRect(240,200,6*boxSize,5*boxSize);
    paint->setFont(QFont("Times", 12, QFont::Bold));
    paint->drawText(240,20,4*boxSize,boxSize,Qt::AlignBottom,QString("SCORES"));
    paint->drawText(240,100,4*boxSize,boxSize,Qt::AlignBottom,QString("RANK"));

    if(rank==1){
        paint->drawText(240,130,5*boxSize,boxSize,Qt::AlignCenter,QString("Simple"));
    }else if(rank==2){
        paint->drawText(240,130,5*boxSize,boxSize,Qt::AlignCenter,QString("Force"));
    }else{
        paint->drawText(240,130,5*boxSize,boxSize,Qt::AlignCenter,QString("Death"));
    }

    paint->drawText(240,178,4*boxSize,boxSize,Qt::AlignBottom,QString("NEXT"));

}

void Tetris::nextBoxPaint(QPainter *paint){
    for(int i=0;i<4;i++){
        paint->fillRect(nextBoxX+nextBox.getX(i)*boxSize,nextBoxY+nextBox.getY(i)*boxSize,boxSize,
                       boxSize,nextBox.getColor());
        paint->drawLine(nextBoxX+nextBox.getX(i)*boxSize-1,nextBoxY+nextBox.getY(i)*boxSize+boxSize-1,
                       nextBoxX+nextBox.getX(i)*boxSize+boxSize-1,nextBoxY+nextBox.getY(i)*boxSize+boxSize-1);
        paint->drawLine(nextBoxX+nextBox.getX(i)*boxSize+boxSize-1,nextBoxY+nextBox.getY(i)*boxSize-1,
                       nextBoxX+nextBox.getX(i)*boxSize+boxSize-1,nextBoxY+nextBox.getY(i)*boxSize+boxSize-1);
    }
}

void Tetris::cleanAllFullLine(){
    for(int i=boxMaxHeight;i<16;i++){
        if(fullLine(i)==20){
            for(int j=i;j>boxMaxHeight;j--){
                for(int z=0;z<10;z++)
                    boxArea[j][z]=boxArea[j-1][z];
            }
            setEmptyFlag(boxMaxHeight);
            scores+=reward;
            if(scores==5000){
                emit  rankAndSpeedchange(2);
            }else if(scores==19000){
                emit  rankAndSpeedchange(3);
            }
            boxMaxHeight--;
        }

    }

    scoreShow->display(scores);
}

void Tetris::checkFullLine(){

    //获取方块下落后,方块的最高层
    for(int i=0;i<16;i++){
        if(fullLine(i)!=0){
            boxMaxHeight=i;
            break;}
    }

    //将所有的满行的方块标记为2
    for(int i=boxMaxHeight;i<16;i++){
        if(fullLine(i)==10){
            setFullFlag(i);
            }
    }
}

void Tetris::dropBox(){

    int newY=currY+boxSize;
    for(int i=0;i<4;i++){

        if(checkIsBOx(newY/boxSize+box.getY(i),currX/boxSize+box.getX(i)) &&
                newY+boxSize*box.getY(i)>=0 || newY+box.getMaxY()*boxSize>=height){

           for(int y=0;y<4;y++){
               boxArea[currY/boxSize+box.getY(y)][currX/boxSize+box.getX(y)]=1;
            }

           if(newY+box.getMinY()*boxSize<0){
               gameOver=true;
               update();
               return;
           }

           box=nextBox;
           nextBox.setShape();
           currX=5*boxSize;
           currY=-2*boxSize;
           checkFullLine();
           cleanAllFullLine();
           update();
           return;
       }
    }

    currY=newY;
    update();
}

void Tetris::changeRankAndSpeed(int i){
    if(i==2){
        if(rank!=i){
            reward=r2;
            rank=i;
            boxColor=state_2;
            dropSpeed=dropSpeed_2;
            time_1->stop();
            time_1->start(dropSpeed);
        }
    }else{
        if(rank!=i){
            reward=r3;
            rank=i;
            boxColor=state_3;
            dropSpeed=dropSpeed_3;
            time_1->stop();
            time_1->start(dropSpeed);
        }
    }

}



void Tetris::keyPressEvent(QKeyEvent *e){
    switch(e->key()){

    case Qt::Key_Space:
    {
        if(gameOver || gamePause){
            return;
        }
        rotate();

        break;
    }
    case Qt::Key_Left:
    {
        if(gameOver || gamePause){
            return;
        }
        move(-boxSize,0);
        break;
    }
    case Qt::Key_Right:
    {
        if(gameOver || gamePause){
            return;
        }
        move(boxSize,0);
        break;
    }
    case Qt::Key_Down:
    {
        if(gameOver || gamePause){
            return;
        }
        move(0,boxSize);
        break;
    }

}


}

void Tetris::rotate(){

    //绕(0,0)旋转90度,坐标变成(-y,x)
    if(box.getShape()==4){
        return;
    }
    //判断旋转是否越界或旋转后周围是否有方块
    for(int i=0;i<4;i++){
        int x=-(box.getY(i));
        int y=box.getX(i);
        if(currX+x*boxSize>=width || currX+x*boxSize<0 || currY+y*boxSize>=height ||
                boxArea[currY/boxSize+y][currX/boxSize+x]==1){
            return;
        }

    }

    for(int i=0;i<4;i++){
        int x=-(box.getY(i));
        int y=box.getX(i);
        box.setX(i,x);
        box.setY(i,y);

    }
    update();

}

void Tetris::move(int x,int y){
    for(int i=0;i<4;i++)
    {
        newX=currX+x;
        newY=currY+y;
        if(newX+box.getX(i)*boxSize>=width ||newX+box.getX(i)*boxSize<0
           || newY+box.getY(i)*boxSize>=height ||
           boxArea[newY/boxSize+box.getY(i)][newX/boxSize+box.getX(i)]==1){
        return;
        }
    }
    currX+=x;
    currY+=y;
    update();
}


Tetris::~Tetris()
{
    delete ui;
}
