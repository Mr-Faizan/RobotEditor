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
    

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;

    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;

/*
    // I am initializing these pointers here so that I can access them in multiple functions.
    // For Example if I want to add multiple Joints or Dynamics data, I can simply use these pointers.
    
    QStandardItem *robotItem = nullptr;
    QStandardItem *jointsCategory = nullptr;
    QStandardItem *jointItem = nullptr;
    QStandardItem *jointKinematicsItem = nullptr;
    QStandardItem *jointDynamicsCategory = nullptr;
    QStandardItem *jointDynamicsItem = nullptr;

    // I am Initializing these arrays so that I can access them whenever I have to add multiple Joints or Dynamics data.
    QJsonArray robotItemKeysArray;
    QJsonArray jointItemKeysArray;
    QJsonArray jointKinematicsItemKeysArray;
    QJsonArray jointDynamicsItemKeysArray;
*/

    


    void addRobotDataTemplate();
    void populateTreeView(QStandardItemModel *model, const QJsonObject &json);
    void addItem(QStandardItem *parent, const QString &key, const QString &value);
    void saveToJson(const QString &filePath);

    QJsonObject modelToJson();



    // 3D Window Playground
    void setup3DPlayground();
    
};
#endif // MAINWINDOW_H
