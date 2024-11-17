#include "protreewidget.h"

#include "protreeitem.h"
#include "const.h"
#include "qheaderview.h"

#include <QDir>

ProTreeWidget::ProTreeWidget(QWidget *parent ) {
    this->header()->hide();
}

void ProTreeWidget::AddProToTree(const QString& name, const QString& path){
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);

    if(_set_path.find(file_path) != _set_path.end()){
        qDebug()<<"file is loaded"<<Qt::endl;
        return;
    }

    QDir pro_dir(file_path);
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path);
        if(!enable){
            qDebug()<<"make file failed"<<Qt::endl;
        }
    }

    _set_path.insert(file_path);
    auto *item = new ProTreeItem(this, name, path, TreeItemPro);
    item->setData(0,Qt::DisplayRole, name);
    item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole, file_path);
}
