#include "mainwindow.h"
#include "picshow.h"
#include "protree.h"
#include "protreeitem.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <wizard.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _protree(nullptr), _dbManager(new DatabaseManager())
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

    _picshow = new PicShow();
    ui->picLayout->addWidget(_picshow);
    auto * pro_pic_show = dynamic_cast<PicShow*>(_picshow);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdateSelected, pro_pic_show, &PicShow::SlotDoubleClickItem);

    connect(pro_pic_show, &PicShow::SigPreClicked,pro_tree_widget,&ProTreeWidget::SlotPreShow);
    connect(pro_pic_show, &PicShow::SigNextClicked,pro_tree_widget,&ProTreeWidget::SlotNextShow);
    connect(pro_tree_widget,&ProTreeWidget::SigUpdatePic,pro_pic_show,&PicShow::SlotUpdatePic);
    connect(pro_tree_widget, &ProTreeWidget::SigClearSelected, pro_pic_show, &PicShow::SlotDeleteItem);

    if (_dbManager->connectToDatabase("pic_file_info.db")) {
        QStringList fileList = _dbManager->loadFileInfo();
        // 加载文件信息到界面中
        loadFiles(fileList);
    }
}

MainWindow::~MainWindow()
{
    // 保存当前文件信息到数据库
    QStringList fileList = getCurrentFiles();
    _dbManager->saveFileInfo(fileList);

    delete _dbManager;
    delete ui;
}

void MainWindow::loadFiles(QStringList files_path)
{
    for(QString file_path : files_path){
        emit SigOpenPro(file_path);
    }
}

QStringList MainWindow::getCurrentFiles()
{
    QStringList files_path;

    QTreeWidget* tree_widget = static_cast<ProTree*>(_protree)->GetTreeWidget();
    auto* pro_tree_widget = static_cast<ProTreeWidget*>(tree_widget);

    int topLevelCount = pro_tree_widget->topLevelItemCount();

    for(int i=0; i<topLevelCount; i++){
        QTreeWidgetItem* topLevelItem = pro_tree_widget->topLevelItem(i);
        auto* pro_topLevelItem = static_cast<ProTreeItem*>(topLevelItem);

        if (!topLevelItem) continue;

        files_path.append(pro_topLevelItem->GetPath());
    }
    return files_path;
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto * pro_pic_show = dynamic_cast<PicShow*>(_picshow);
    pro_pic_show->ReloadPic();
    QMainWindow::resizeEvent(event);
}
