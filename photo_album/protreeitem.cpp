#include "protreeitem.h"
#include "const.h"

ProTreeItem::ProTreeItem(QTreeWidget *treeview,const QString& name, const QString& path, int type)
    :QTreeWidgetItem(treeview, type), _name(name), _path(path), _root(this), _pre_item(nullptr), _next_item(nullptr) {

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name,
                         const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type), _name(name), _path(path), _root(root), _pre_item(nullptr), _next_item(nullptr){

}

QTreeWidgetItem* ProTreeItem::GetRoot(){
    return _root;
}

const QString& ProTreeItem::GetPath(){
    return _path;
}

const QString& ProTreeItem::GetName(){
    return _name;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem* item){
    _pre_item = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem* item){
    _next_item = item;
}

QTreeWidgetItem* ProTreeItem::GetPreItem(){
    return _pre_item;
}

QTreeWidgetItem* ProTreeItem::GetNextItem(){
    return _next_item;
}

QTreeWidgetItem* ProTreeItem::GetLastPicChild(){
    if(this->type() == TreeItemPic){
        return nullptr;
    }

    auto childcount = this->childCount();
    if(childcount == 0){
        return nullptr;
    }

    for(int i = childcount-1; i>=0; i--){
        auto *last_child = this->child(i);
        auto *last_child_item = static_cast<ProTreeItem*>(last_child);
        int item_type = last_child_item->type();

        if(item_type == TreeItemPic){
            return last_child;
        }

        last_child = last_child_item->GetLastPicChild();

        if(!last_child){
            continue;
        }

        return last_child;
    }
    return nullptr;
}
QTreeWidgetItem* ProTreeItem::GetFirstPicChild(){
    if(this->type() == TreeItemPic){
        return nullptr;
    }

    auto childcount = this->childCount();
    if(childcount == 0){
        return nullptr;
    }

    for(int i=0; i<childcount; i++){
        auto* first_child = this->child(i);
        auto* first_child_item = static_cast<ProTreeItem*>(first_child);

        int item_type = first_child_item->type();
        if(item_type == TreeItemPic){
            return first_child;
        }

        first_child = first_child_item->GetFirstPicChild();

        if(!first_child){
            continue;
        }

        return first_child;
    }
    return nullptr;
}
