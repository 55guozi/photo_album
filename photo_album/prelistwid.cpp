#include "prelistwid.h"
#include "const.h"
#include "protreeitem.h"
#include <QApplication>
#include <QPainter>
#include "prelistitem.h"

PreListWid::PreListWid(QWidget *parent):QListWidget(parent), _global(0),_last_index(15)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);
    connect(this, &PreListWid::itemDoubleClicked, this, &PreListWid::SlotItemClicked);
}

PreListWid::~PreListWid()
{

}

void PreListWid::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dst_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dst_pixmap.fill(QColor(220,220,220,50));
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();

    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();

    auto x = (dst_width-src_width)/2;
    auto y = (dst_height-src_height)/2;
    painter.drawPixmap(x, y, src_pixmap);

    _global++;
    PreListItem* pItem = new PreListItem(QIcon(dst_pixmap), path, _global, this);
    pItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(pItem);
    _set_items[path] = pItem;
    if(_global == 1){
        _pos_orgin = this->pos();
    }
}

void PreListWid::SlotUpPreList(QTreeWidgetItem *tree_item)
{
    if(!tree_item){
        return;
    }

    auto* pro_item = static_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    if(_set_items.find(path) != _set_items.end()){
        return;
    }

    AddListItem(path);
}

void PreListWid::SlotUpSelcct(QTreeWidgetItem *tree_item)
{
    if(!tree_item){
        return;
    }

    auto* pro_item = static_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    if(_set_items.find(path) == _set_items.end()){
        return;
    }

    auto* list_item = static_cast<PreListItem*>(_set_items[path]);
    auto index = list_item->getIndex();
    if(index > 15){
        auto pos_cur =  this->pos();
        this->move(pos_cur.x()-(index-_last_index)*100,pos_cur.y());
        _last_index = index;
    }else{
        this->move(_pos_orgin);
        _last_index = 15;
    }

    this->setCurrentItem(_set_items[path]);
}

void PreListWid::SlotItemClicked(QListWidgetItem *item)
{
    if(QApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto* list_item = static_cast<PreListItem*>(item);
    auto cur_index = list_item->getIndex();
    auto path = list_item->getPath();
    setCurrentItem(item);
    emit SigUpSelectShow(path);
}
