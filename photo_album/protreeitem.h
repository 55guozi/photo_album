#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget *treeview,const QString& name, const QString& path, int type = Type);
private:
    QString _name;
    QString _path;
    QTreeWidgetItem* _root;
    QTreeWidgetItem* _next_item;
    QTreeWidgetItem* _pre_item;
};

#endif // PROTREEITEM_H
