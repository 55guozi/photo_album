#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>

class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString &src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                  int file_count, QTreeWidget* self, QTreeWidgetItem* root, QObject* parent = nullptr);
    ~ProTreeThread();
protected:
    virtual void run();
private:
    QString _src_path;
    QString _dist_path;
    QTreeWidgetItem* _parent_item;
    int _file_count;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    bool _bstop;
    void CreateProTree(const QString &src_path, const QString &dist_path,
                                      QTreeWidgetItem *parent_item, int &file_count,
                       QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem* preItem);
public slots:
    void SlotCancelProgress();
signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);
};

#endif // PROTREETHREAD_H
