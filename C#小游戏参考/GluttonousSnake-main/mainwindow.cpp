#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("贪吃蛇");
    this->setFixedSize(QSize(1080,810));
    this->Width = this->width();
    this->Height = this->height();
    for(int i = 0;i<snake_body_num;i++) //蛇出生地点
    {
        snake_x.append(800);
        snake_y.append(600+i*2*snake_body_r.at(level));
    }
    food_create_timer = new QTimer(this);
    connect(food_create_timer,&QTimer::timeout,[=]
    {
       food_create(); //产生食物
    });
     painter_timer = new QTimer(this);
     connect(painter_timer,&QTimer::timeout,[=]
     {
        this->update();
         Is_eat_food();
         Is_game_over();
         update_message();
     });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::food_create()
{
    if(food_x.size()==food_maxnum) //如果食物数量超过40个,减少10个食物
    {
        for(int i = 0;i<10;i++)
        {
             food_x.removeFirst();
             food_y.removeLast();
        }
    }
    //QRandomGenerator::global()->bounded(3.5);//生成高精度随机数 0~3.4
    qsrand(food_r.at(level)+1);//设置随机数种子,qrand生成随机数的起始值
    int x = QRandomGenerator::global()->bounded(Width - 2*food_r.at(level));
    qsrand(food_r.at(level)+ui->menubar->height()+1);
    int y = QRandomGenerator::global()->bounded(Height - 2*food_r.at(level));
    food_x.append(x);
    food_y.append(y);
}

void MainWindow::move_up()
{
    //首部增加元素
    snake_x.push_front(snake_x.front());
    snake_y.push_front(snake_y.front() - 2*snake_body_r.at(level));
    //尾部删除元素
    snake_x.pop_back();
    snake_y.pop_back();
}

void MainWindow::move_down()
{
    //首部增加元素
    snake_x.push_front(snake_x.front());
    snake_y.push_front(snake_y.front() + 2*snake_body_r.at(level));
    //尾部删除元素
    snake_x.pop_back();
    snake_y.pop_back();
}

void MainWindow::move_left()
{
    //首部增加元素
    snake_x.push_front(snake_x.front() - 2*snake_body_r.at(level));
    snake_y.push_front(snake_y.front());
    //尾部删除元素
    snake_x.pop_back();
    snake_y.pop_back();
}

void MainWindow::move_right()
{
    //首部增加元素
    snake_x.push_front(snake_x.front() + 2*snake_body_r.at(level));
    snake_y.push_front(snake_y.front());
    //尾部删除元素
    snake_x.pop_back();
    snake_y.pop_back();
}

void MainWindow::Is_eat_food()
{

    for(int i = 0;i<food_x.size();i++)
    {
        if((food_x.at(i) - eat_food_collision_offset<=snake_x.at(0)&&food_x.at(i)+eat_food_collision_offset>=snake_x.at(0))
        &&(food_y.at(i) - eat_food_collision_offset<=snake_y.at(0)&&food_y.at(i)+eat_food_collision_offset>=snake_y.at(0)))//蛇吃到食物
        {
            //蛇身加长
            snake_x.append(snake_x.back());
            snake_y.append(snake_y.back());
            //食物消失
            food_x.removeAt(i);
            food_y.removeAt(i);
            snake_body_num++;
            score++;
        }
    }
}

void MainWindow::Is_game_over()
{
    //游戏结束的两种情况
    //蛇头与蛇身相碰撞
    //蛇头撞墙
    for(int i = 1;i<snake_x.size();i++)
    {
        if((snake_x.at(0)<=snake_x.at(i)+snake_collision_body_offset&&snake_x.at(0)>=snake_x.at(i)-snake_collision_body_offset)
           &&(snake_y.at(0)<=snake_y.at(i)+snake_collision_body_offset&&snake_y.at(0)>=snake_y.at(i)-snake_collision_body_offset))
            Game_State = OVER;
    }
    if(snake_x.at(0)-snake_collision_wall_offset<=0||snake_y.at(0)-snake_collision_up_offset-ui->menubar->height()<=0||
            snake_x.at(0)+snake_collision_wall_offset>=Width||snake_y.at(0)+snake_collision_wall_offset>=Height)
        Game_State = OVER;
}

void MainWindow::update_message()
{
    if(snake_body_num>10+level*10)
    {
        level++;
    }
    QString str = QString("当前得分:%1").arg(QString::number(score));
    QString str1 = QString("蛇身长度:%1").arg(QString::number(snake_body_num));
    ui->statusbar->showMessage(str + "      " + str1);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QRect(0,0,Width,Height),QImage(map));
    if(Game_State ==WAIT)
    {
        painter.setPen(QColor(255,0,0));
        painter.setFont(QFont("幼圆",24));
        QString str = QString("请点击左上角的菜单开始游戏!");
        QString str1 = QString("↑ ↓ ← → 控制蛇的移动");
        QString str2 = QString("空格键可暂停/继续游戏");
        painter.drawText(QPointF(Width/2-240,Height/2-40),str);
        painter.drawText(QPointF(Width/2-240,Height/2),str1);
        painter.drawText(QPointF(Width/2-240,Height/2+40),str2);
    }
    else if(Game_State == START)
    {
        //painter.translate(0,ui->menubar->height()); 画板偏移
        QPen pen(food_color,2); //食物画笔
        QBrush brush(food_color); //食物画刷
        painter.setBrush(brush);
        painter.setPen(pen);
        painter.setRenderHints(QPainter::Antialiasing);//抗锯齿
        for(int i = 0;i<food_x.size();i++)
        {
            painter.drawEllipse(QPointF(food_x.at(i),food_y.at(i)),food_r.at(level),food_r.at(level));
        }
        painter.setPen(snake_head_color); //蛇头画笔
        painter.setBrush(snake_head_color);//蛇头画刷
        painter.drawEllipse(QPointF(snake_x.at(0),snake_y.at(0)),snake_head_r.at(level),snake_head_r.at(level));
        painter.setPen(snake_body_color); //蛇身画笔
        painter.setBrush(snake_body_color);//蛇身画刷
        for(int j = 1;j<snake_x.size();j++)
        {
            painter.drawEllipse(QPointF(snake_x.at(j),snake_y.at(j)),snake_body_r.at(level),snake_body_r.at(level));
        }
        switch(move_pos)
        {
        case 'U':move_up();
                 break;
        case 'D':move_down();
                 break;
        case 'L':move_left();
                 break;
        case 'R':move_right();
                 break;
        default:break;
        }
       }
        else if(Game_State == PAUSE)
        {
            painter_timer->stop();
            food_create_timer->stop();
            painter.setPen(QColor(255,0,0));
            painter.setFont(QFont("幼圆",24));
            QString str = QString("游戏暂停!");
            QString str1 = QString("请按空格键继续!");
            painter.drawText(QPointF(Width/2-180,Height/2-40),str);
            painter.drawText(QPointF(Width/2-180,Height/2),str1);
        }
        else if(Game_State == OVER) //游戏结束
        {
            painter_timer->stop();
            food_create_timer->stop();
            painter.setPen(QColor(255,0,0));
            painter.setFont(QFont("幼圆",24));
            QString str = QString("游戏结束!");
            QString str1 = QString("最终得分:%1").arg(score);
            painter.drawText(QPointF(Width/2-60,Height/2-80),str);
            painter.drawText(QPointF(Width/2-60,Height/2-40),str1);

            //游戏结束初始化数据
            food_x.clear();
            food_y.clear();
            snake_x.clear();
            snake_y.clear();
            for(int i = 0;i<snake_body_num;i++) //蛇出生地点
            {
                snake_x.append(800);
                snake_y.append(600+i*2*snake_body_r.at(level));
            }
             snake_body_num = 5;//初始蛇身数量
             score = 0;//当前得分
             level = 0;//控制食物和蛇身规模的变量
             move_pos = 'U';//蛇移动的方向

        }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Up&&move_pos!='D')
        move_pos = 'U';
    else if(event->key()==Qt::Key_Down&&move_pos!='U')
        move_pos = 'D';
    else if(event->key()==Qt::Key_Left&&move_pos!='R')
        move_pos = 'L';
    else if(event->key()==Qt::Key_Right&&move_pos!='L')
        move_pos = 'R';
    else if(event->key()==Qt::Key_Space)
    {
        if(Game_State==START)
             Game_State = PAUSE;
        else if( Game_State == PAUSE)
        {
            Game_State = START;
            painter_timer->start(100);
            food_create_timer->start(1500);
        }
    }
}
void MainWindow::on_action_S_triggered()//开始游戏
{
    Game_State = START;
   food_create_timer->start(1500);
   painter_timer->start(100);
}

void MainWindow::on_action_triggered()//蛇头配色
{
    snake_head_color = QColorDialog::getColor(Qt::gray,this,"蛇头颜色选择");
}

void MainWindow::on_action_2_triggered()//蛇身配色
{
    snake_body_color = QColorDialog::getColor(Qt::gray,this,"蛇身颜色选择");
}

void MainWindow::on_action_3_triggered()//食物配色
{
    food_color = QColorDialog::getColor(Qt::red,this,"食物颜色选择");
}

void MainWindow::on_action_5_triggered() //荒野沙漠
{
    map = ":/new/prefix1/src/desert.png";
}

void MainWindow::on_action_6_triggered()//无尽宇宙
{
    map = ":/new/prefix1/src/yekong.png";
}

void MainWindow::on_action_7_triggered()//蓝色海洋
{
    map = ":/new/prefix1/src/sea.png";
}

void MainWindow::on_action_8_triggered()//蓝天白云
{
    map = ":/new/prefix1/src/tiankong.png";
}

void MainWindow::on_action_9_triggered()//浪漫晚霞
{
    map = ":/new/prefix1/src/langman.png";
}

void MainWindow::on_action_10_triggered()//冰天雪地
{
    map = ":/new/prefix1/src/winter.png";
}

void MainWindow::on_action_13_triggered() //自定义地图
{
    map = QFileDialog::getOpenFileName(this,"选择一个文件","/home");
}

