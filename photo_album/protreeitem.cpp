#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget *treeview,const QString& name, const QString& path, int type)
    :QTreeWidgetItem(treeview, type), _name(name), _path(path), _root(this), _pre_item(nullptr), _next_item(nullptr) {

}
