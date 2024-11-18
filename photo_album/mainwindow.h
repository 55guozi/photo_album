#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget* _protree;
    QWidget* _picshow;
protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void SlotCreatePro();
    void SlotOpenPro();
signals:
    void SigOpenPro(const QString& );
};
#endif // MAINWINDOW_H
