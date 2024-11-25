#ifndef PRELISTWID_H
#define PRELISTWID_H

#include <QListWidget>
#include <QTreeWidgetItem>

class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    PreListWid(QWidget *parent = nullptr);
    virtual ~PreListWid();
private:
    QMap<QString, QListWidgetItem*> _set_items;
    void AddListItem(const QString& path);
    int _global;
    QPoint _pos_orgin;
    int _last_index;
public slots:
    void SlotUpPreList(QTreeWidgetItem* tree_item);
    void SlotUpSelcct(QTreeWidgetItem* tree_item);
private slots:
    void SlotItemClicked(QListWidgetItem *item);
signals:
    void SigUpSelectShow(const QString& path);
};

#endif // PRELISTWID_H
