#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QProgressDialog>
#include <QTreeWidget>
#include "const.h"
#include "opentreethread.h"
#include "protreethread.h"
#include "threadpool.h"

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
    QProgressDialog* _open_progressdlg;
    std::shared_ptr<ProTreeThread> _thread_create_pro;
    std::shared_ptr<OpenTreeThread> _thread_open_pro;
    ThreadPool *_thread_pool;

private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotImport();
    void SlotSetActive();
    void SlotClosePro();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotUpOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelOpenProgress();
    void SlotDoubleClickItem(QTreeWidgetItem *item, int column);
public slots:
    void SlotOpenPro(const QString& path);
    void SlotPreShow();
    void SlotNextShow();
signals:
    void SigCancelProgress();
    void SigClearSelected();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString&);
    void SigUpdatePic(const QString &);
};

#endif // PROTREEWIDGET_H
