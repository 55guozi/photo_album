#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include "const.h"

class ProTreeWidget : public QTreeWidget
{
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString& name, const QString& path);
private:
    QSet<QString> _set_path;
    QTreeWidgetItem *_right_item;
    QTreeWidgetItem *action_item;
    QTreeWidgetItem *selected_item;

};

#endif // PROTREEWIDGET_H
