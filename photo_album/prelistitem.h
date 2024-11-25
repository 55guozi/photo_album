#ifndef PRELISTITEM_H
#define PRELISTITEM_H

#include <QListWidgetItem>

class PreListItem : public QListWidgetItem
{
public:
    PreListItem(const QIcon &icon, const QString& text, const int& index,
                QListWidget *listview = nullptr, int type = Type);
    virtual ~PreListItem();
    QString getPath();
    int getIndex();
private:
    QString _path;
    int _index;
};

#endif // PRELISTITEM_H
