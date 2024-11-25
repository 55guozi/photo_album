#include "protreewidget.h"

#include "protreeitem.h"
#include "const.h"
#include "qheaderview.h"
#include "removeprodialog.h"

#include <QApplication>
#include <QDir>
#include <QGuiApplication>
#include <QMenu>
#include <qfiledialog.h>

ProTreeWidget::ProTreeWidget(QWidget *parent )
    :QTreeWidget(parent), _right_item(nullptr), _active_item(nullptr), _selected_item(nullptr)
    , _dialog_progress(nullptr), _thread_create_pro(nullptr), _thread_pool(new ThreadPool()){
    this->header()->hide();

    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    _action_import = new QAction(QIcon(":/icon/import.png"),tr("导入文件"));
    _action_setstart = new QAction(QIcon(":/icon/core.png"),tr("设置活动项目"));
    _action_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"));
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"));

    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_action_setstart, &QAction::triggered, this, &ProTreeWidget::SlotSetActive);
    connect(_action_closepro, &QAction::triggered, this, &ProTreeWidget::SlotClosePro);
    connect(_action_slideshow, &QAction::triggered, this, &ProTreeWidget::SlotSlideShow);

    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotDoubleClickItem);
}

void ProTreeWidget::AddProToTree(const QString& name, const QString& path){
    qDebug() << "ProTreeWidget::AddProToTree name is " << name << " path is " << path << Qt::endl;
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    //检测重名，判断路径和名字都一样则拒绝加入
    if(_set_path.find(file_path) != _set_path.end()){
        qDebug() << "file has loaded" << Qt::endl;
        return;
    }
    //构造项目用的文件夹
    QDir pro_dir(file_path);
    //如果文件夹不存在则创建
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){
            qDebug() << "pro_dir make path failed" << Qt::endl;
            return;
        }
    }

    _set_path.insert(file_path);
    auto * item = new ProTreeItem(this, name, file_path,  TreeItemPro);
    item->setData(0,Qt::DisplayRole, name);
    item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole, file_path);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *item, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        QMenu menu(this);
        _right_item = item;
        if(item->type() == TreeItemPro){
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_slideshow);
            menu.addAction(_action_closepro);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog fileDialog;
    //fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setWindowTitle(tr("选择导入文件"));

    QString path = "";
    if(!_right_item){
        path = QDir::currentPath();
        return ;
    }

    path = static_cast<ProTreeItem*>(_right_item)->GetPath();

    fileDialog.setDirectory(path);
    fileDialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(fileDialog.exec()){
        fileNames = fileDialog.selectedFiles();
    }

    if(fileNames.length() <= 0){
        return;
    }

    QString import_path = fileNames.at(0);
    int file_count = 0;
     _dialog_progress = new QProgressDialog(this);

    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path), path, _right_item, std::ref(file_count), this, _right_item, nullptr);

    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelProgress);
    connect(this, &ProTreeWidget::SigCancelProgress, _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress);

    auto b = _thread_pool->enqueue([this](){
        this->_thread_create_pro.get()->run();
    });

    _dialog_progress->setWindowTitle("Please Wait...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_MAX);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotSetActive(){
    if(!_right_item){
        return;
    }

    QFont font;
    font.setBold(false);
    if(_active_item){
        _active_item->setFont(0, font);
    }

    _active_item = _right_item;
    font.setBold(true);
    _active_item->setFont(0, font);
}

void ProTreeWidget::SlotClosePro(){
    if(!_right_item){
        return;
    }

    RemoveProDIalog remove_dialog(this);
    if(remove_dialog.exec() != QDialog::Accepted){
        return;
    }

    bool is_remove = remove_dialog.isRemove();
    auto index_right_btn = this->indexOfTopLevelItem(_right_item);
    auto* protreeitem = static_cast<ProTreeItem*>(_right_item);
    auto* selecedItem = static_cast<ProTreeItem*>(_selected_item);
    QString delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(is_remove){
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if(_active_item == _right_item){
        _active_item = nullptr;
    }

    if(selecedItem && protreeitem == selecedItem->GetRoot()){
        _selected_item = nullptr;
        emit SigClearSelected();
    }
    delete this->takeTopLevelItem(index_right_btn);
    _right_item = nullptr;

}

void ProTreeWidget::SlotUpdateProgress(int count){
    qDebug() << "count is " << count;
    if(!_dialog_progress){
        qDebug() << "dialog_progress is empty!!!" << Qt::endl;
        return;
    }

    _dialog_progress->setValue(count%PROGRESS_MAX);
}
void ProTreeWidget::SlotFinishProgress(){
    if (_dialog_progress) {
        _dialog_progress->setValue(PROGRESS_MAX);
        _dialog_progress->deleteLater();
        _dialog_progress = nullptr;
    }
}
void ProTreeWidget::SlotCancelProgress(){
    if (_dialog_progress) {
        emit SigCancelProgress();
        _dialog_progress->deleteLater();
        _dialog_progress = nullptr;
    }
}

void ProTreeWidget::SlotOpenPro(const QString& path){
    if(_set_path.find(path) != _set_path.end()){
        qDebug() << "file has loaded" << Qt::endl;
        return;
    }

    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    const QString& proname = pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count, this, nullptr);

    _open_progressdlg = new QProgressDialog(this);

    //连接更新进度框操作
    connect(_thread_open_pro.get(), &OpenTreeThread::SigUpdateProgress,
            this, &ProTreeWidget::SlotUpOpenProgress);

    connect(_thread_open_pro.get(), &OpenTreeThread::SigFinishProgress, this,
            &ProTreeWidget::SlotFinishOpenProgress);

    connect(_open_progressdlg, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelOpenProgress);

    connect(this, &ProTreeWidget::SigCancelOpenProgress, _thread_open_pro.get(), &OpenTreeThread::SlotCancelOpenProgress);

    auto b = _thread_pool->enqueue([this](){
        this->_thread_open_pro.get()->run();
    });
    _open_progressdlg->setWindowTitle("Please wait...");
    _open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progressdlg->setRange(0, PROGRESS_MAX);
    _open_progressdlg->exec();
}

void ProTreeWidget::SlotUpOpenProgress(int count){
    qDebug() << "count is " << count;
    if(!_open_progressdlg){
        qDebug() << "dialog_progress is empty!!!" << Qt::endl;
        return;
    }

    _open_progressdlg->setValue(count%PROGRESS_MAX);
}

void ProTreeWidget::SlotFinishOpenProgress(){
    if (_open_progressdlg) {
        _open_progressdlg->setValue(PROGRESS_MAX);
        _open_progressdlg->deleteLater();
        _open_progressdlg = nullptr;
    }
}
void ProTreeWidget::SlotCancelOpenProgress(){
    if (_open_progressdlg) {
        emit SigCancelOpenProgress();
        _open_progressdlg->deleteLater();
        _open_progressdlg = nullptr;
    }
}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *item, int column){
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto *tree_doubleItem = static_cast<ProTreeItem*>(item);
    if(!tree_doubleItem){
        return;
    }

    int item_type = tree_doubleItem->type();
    if(item_type == TreeItemPic){
        emit SigUpdateSelected(tree_doubleItem->GetPath());
        _selected_item = tree_doubleItem;
    }
}

void ProTreeWidget::SlotSlideShow()
{
    if(!_right_item){
        return;
    }

    auto* _right_pro_item = static_cast<ProTreeItem*>(_right_item);

    auto* last_child_item = _right_pro_item->GetLastPicChild();
    if(!last_child_item){
        return;
    }

    auto* first_child_item = _right_pro_item->GetFirstPicChild();
    if(!first_child_item){
        return;
    }

    _slide_show_dlg = std::make_shared<SlideShowDlg>(this, first_child_item, last_child_item);
    _slide_show_dlg->setModal(true);
    _slide_show_dlg->setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //_slide_show_dlg->setStyleSheet("QDialog { border: none; }");
    _slide_show_dlg->showFullScreen();
}

void ProTreeWidget::SlotPreShow(){
    if(!_selected_item){
        return;
    }

    auto * curItem = dynamic_cast<ProTreeItem*>(dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem());
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotNextShow(){
    if(!_selected_item){
        return;
    }

    auto * curItem = dynamic_cast<ProTreeItem*>(dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem());
    if(!curItem){
        return;
    }
    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("选择音频文件"));
    file_dialog.setDirectory(QDir::current());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    if (file_dialog.exec()) {
        // 获取用户选择的文件
        QStringList filenames = file_dialog.selectedFiles();

        // 清空当前播放列表，并添加新的文件路径
        _playlist.clear();
        _playlist = filenames;

        // 设置 QMediaPlayer 的源为第一个文件
        if (!_playlist.isEmpty()) {
            _player->setSource(QUrl::fromLocalFile(_playlist.first()));
        }
    }
}

void ProTreeWidget::SlotStartMusic()
{
    if(!_playlist.isEmpty()){
        qDebug("music playing");
        _player->play();
    }
}

void ProTreeWidget::SlotStopMusic()
{
    if(!_playlist.isEmpty()){
        qDebug("music stop");
        _player->stop();
    }
}
