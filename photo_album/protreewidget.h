#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QProgressDialog>
#include <QTreeWidget>
#include "const.h"
#include "protreethread.h"

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString& name, const QString& path);
private:
    QSet<QString> _set_path;
    QTreeWidgetItem *_right_item;
    QTreeWidgetItem *_active_item;
    QTreeWidgetItem *_selected_item;
    QAction *_action_import;
    QAction *_action_setstart;
    QAction *_action_closepro;
    QAction *_action_slideshow;
    QProgressDialog* _dialog_progress;
    std::shared_ptr<ProTreeThread> _thread_create_pro;

private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotImport();
    void SlotSetActive();
    void SlotClosePro();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
signals:
    void SigCancelProgress();
};

#endif // PROTREEWIDGET_H
