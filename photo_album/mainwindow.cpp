#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建文件菜单
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)"));
    QAction *act_creat_pro = new QAction(QIcon(""), tr("创建项目"), this);
    act_creat_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    QAction *act_open_pro = new QAction(QIcon(""), tr("打开文件"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_creat_pro);
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));
    QAction* act_music = new QAction(QIcon(""), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    connect(act_creat_pro, &QAction::triggered, this, &MainWindow::SlotCreatePro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::SlotOpenPro);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro()
{
    qDebug()<< " create file is triggered" <<Qt::endl;
}

void MainWindow::SlotOpenPro()
{
    qDebug()<< " open file is triggered" <<Qt::endl;
}
