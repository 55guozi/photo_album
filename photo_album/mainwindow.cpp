#include "mainwindow.h"
#include "protree.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <wizard.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _protree(nullptr)
{
    ui->setupUi(this);

    //创建文件菜单
    QMenu* menu_file = menuBar()->addMenu(tr("文件(&F)"));
    QAction *act_creat_pro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    act_creat_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    QAction *act_open_pro = new QAction(QIcon(":/icon/openpro.png"), tr("打开文件"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    menu_file->addAction(act_creat_pro);
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu* menu_set = menuBar()->addMenu(tr("设置(&S)"));
    QAction* act_music = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M));
    menu_set->addAction(act_music);

    connect(act_creat_pro, &QAction::triggered, this, &MainWindow::SlotCreatePro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::SlotOpenPro);

    _protree = new ProTree(this);
    ui->proLayout->addWidget(_protree);

    QTreeWidget* tree_widget = static_cast<ProTree*>(_protree)->GetTreeWidget();
    auto* pro_tree_widget = static_cast<ProTreeWidget*>(tree_widget);
    connect(this, &MainWindow::SigOpenPro, pro_tree_widget, &ProTreeWidget::SlotOpenPro);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro()
{
    Wizard* wizard = new Wizard(this);
    wizard->setWindowTitle(tr("创建文件"));
    auto* page = wizard->page(0);
    page->setTitle(tr("设置项目配置"));

    connect(wizard, &Wizard::SigProSettings, static_cast<ProTree*>(_protree), &ProTree::AddProtoTree);

    wizard->show();
    wizard->exec();
}

void MainWindow::SlotOpenPro()
{
    QFileDialog file_dialog(this);
    file_dialog.setWindowTitle(tr("选择打开文件"));
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setDirectory(QDir::currentPath());
    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length()<=0){
        return;
    }

    QString file_name = fileNames.at(0);
    emit SigOpenPro(file_name);
}
