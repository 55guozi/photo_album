#ifndef REMOVEPRODIALOG_H
#define REMOVEPRODIALOG_H

#include <QDialog>

namespace Ui {
class RemoveProDIalog;
}

class RemoveProDIalog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveProDIalog(QWidget *parent = nullptr);
    bool isRemove();
    ~RemoveProDIalog();

private:
    Ui::RemoveProDIalog *ui;
};

#endif // REMOVEPRODIALOG_H
