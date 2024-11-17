#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget* view, const QString& name, const QString& path, int type = Type);
    ProTreeItem(QTreeWidgetItem* parent, const QString& name, const QString& path, QTreeWidgetItem* root, int type = Type);
    void SetPreItem(QTreeWidgetItem* item);
    void SetNextItem(QTreeWidgetItem* item);
    QTreeWidgetItem* GetRoot();
    const QString& GetPath();
    const QString& GetName();
    QTreeWidgetItem* GetPreItem();
    QTreeWidgetItem* GetNextItem();
    QTreeWidgetItem* GetLastPicChild();
    QTreeWidgetItem* GetFirstPicChild();
private:
    QString _name;
    QString _path;
    QTreeWidgetItem* _root;
    QTreeWidgetItem* _next_item;
    QTreeWidgetItem* _pre_item;
};

#endif // PROTREEITEM_H
