#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include<protreewidget.h>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();

private:
    Ui::ProTree *ui;

public slots:
    void AddProtoTree(const QString name, const QString path);
};

#endif // PROTREE_H
