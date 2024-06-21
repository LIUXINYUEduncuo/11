#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include<QPainter>
#include<QPaintEvent>
#include<QList>
#include<QTimer>
#include<QRandomGenerator> //随机数
#include<QtGlobal>
#include<QDebug>
#include<QKeyEvent>
#include<QTime>
#include<QColorDialog>
#include<QColor>
#include<QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
enum game_state //游戏的状态
{
    WAIT,
    START,
    PAUSE,
    OVER
};

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void food_create();//食物生成函数

    //蛇移动函数
    void move_up();
    void move_down();
    void move_left();
    void move_right();

    void Is_eat_food();//判断蛇是否吃到食物
    void Is_game_over();//判断是否结束游戏
    void update_message();//更新游戏信息


protected:
    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event)override;

private slots:
    void on_action_S_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_10_triggered();

    void on_action_13_triggered();


private:
    Ui::MainWindow *ui;
    QList<int> food_x; //食物x轴坐标
    QList<int> food_y; //食物y轴坐标
    QList<int> snake_x;//蛇x轴坐标
    QList<int> snake_y;//蛇y轴坐标
    int Width;//窗口的宽
    int Height;//窗口的高
    QList<int> food_r = {6,7,8,9,10};//食物的半径
    QList<int> snake_body_r = {6,7,8,9,10};//蛇身半径
    QList<int> snake_head_r = {8,9,11,12,13};//蛇头半径
    int snake_body_num = 5;//初始蛇身数量
    int score = 0;//当前得分
    int level = 0;//控制食物和蛇身规模的变量
    int eat_food_collision_offset = 3*(snake_head_r.at(level) - food_r.at(level));//蛇吃食物碰撞检测的偏移量
    int snake_collision_body_offset = 3*(snake_head_r.at(level) - snake_body_r.at(level));
    int snake_collision_wall_offset = snake_head_r.at(level);
    int snake_collision_up_offset = snake_collision_wall_offset+snake_collision_wall_offset/4;
    char move_pos = 'U';//蛇移动的方向
    const int food_maxnum = 40;//食物的最大数量
    int Game_State = WAIT; //游戏状态
    QColor snake_head_color = Qt::gray; //蛇头颜色
    QColor snake_body_color = Qt::gray; //蛇身颜色
    QColor food_color = Qt::red;//食物颜色
    QString map = ":/new/prefix1/src/none.png"; //地图
    QTimer* food_create_timer;//食物生成定时器
    QTimer* painter_timer;//画家定时器

};
#endif // MAINWINDOW_H
