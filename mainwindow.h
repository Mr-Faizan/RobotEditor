#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

// Qt Libraries
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTreeView>
#include <QStandardItemModel>


// 3D Libraries
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/qforwardrenderer.h>
#include <QWidget>
#include <QVBoxLayout>
#include <QColor>

using namespace std;


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
    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;

    void addRobotDataTemplate();

    // 3D Window Playground
    void setup3DPlayground();
    
};
#endif // MAINWINDOW_H
