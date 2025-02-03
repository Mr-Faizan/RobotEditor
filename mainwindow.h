#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// QT Libraries
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <iostream>
#include <QIcon>

// Qt JSON and File Libraries
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileDialog>


// QT TreeView Libraries
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLineEdit>
#include <QHBoxLayout>


// QT 3D Libraries
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

private slots:
    void on_actionSave_triggered();
    void on_actionNewRobot_triggered();
    void on_actionOpenFromDevice_triggered();

    // defining the slots for the context menu.
    void addNewJoint();
    void addNewDynamics();
    

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;

    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;


    // Custom Functions
    void showContextMenu(const QPoint &pos);
    void addRobotDataTemplate();
    void loadJsonData(const QJsonObject &jsonObject);
    void populateTreeView(QStandardItemModel *model, const QJsonObject &json);
    void addItem(QStandardItem *parent, const QString &key, const QString &value);
    void saveToJson(const QString &filePath);

    QJsonObject modelToJson();



    // 3D Window Playground
    void setup3DPlayground();
    
};
#endif // MAINWINDOW_H
