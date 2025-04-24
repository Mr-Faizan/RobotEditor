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
#include <Qt3DCore/QTransform>



#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QDirectionalLight>

#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qorbitcameracontroller.h>




#include <QVBoxLayout>
#include <QColor>
#include <QTimer>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>

// RobotLib Libraries
#include "robotlib.h"



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
    void on_actionActiveRobot_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAll_triggered();
    void on_actionDeleteAll_triggered();
    void on_actionNewRobot_triggered();
    void on_actionOpenFromDevice_triggered();
    void on_actionImportFromVisualComponent_triggered();
    void on_actionResetModel_triggered();
    void on_actionRotateModel_triggered();
    void on_actionJointVisualization_triggered();
    

    // defining the slots for the context menu.
    void addNewJoint();
    void addNewDynamics();

    void deleteCurrentRobot();
    void deleteCurrentJoint();
    void deleteCurrentPayload();
    

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model; // Model to store the data
    QJsonObject templateObject; // Template object to store the template data.
    QStandardItem *activeRobotItem = nullptr;
    
    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QEntity *rootEntity;
    Qt3DRender::QCamera *camera;
    QTimer *rotationTimer;      // Timer for rotation
    QMap<QString, Qt3DCore::QEntity*> entityMap; // Map to store the entities for each OBJ file

    float rotationAngle = 0.0f;        // Current rotation angle
    bool rotating = false;      // Flag to indicate if rotation is enabled 

    // RobotLib Object
    RobotLib robotLib;

    // Custom Functions
    // void loadTemplate();
    void showContextMenu(const QPoint &pos);
    // void populateTreeView(const QJsonObject &json);
    void populateTreeView(const Robot &robot);
    // void addJoint(QStandardItem *jointsItem, const QJsonObject &joint);
    void addJoint(QStandardItem *jointsItem, const Joint &joint);
    void addDynamicsPayload(QStandardItem *dynamicsItem, const JointDynamics &dynamics);
    // void addDynamicsPayload(QStandardItem *dynamicsItem, const QString &payloadKey, const QJsonObject &payload);
    void addItem(QStandardItem *parent, const QString &key, const QVariant &value);
    void addComboBoxItem(QStandardItem *parent, const QString &key, const QString &value);
    void saveToJson(const QString &filePath, QStandardItem *currentItem);

    QJsonObject modelToJson(QStandardItem *robotItem);



    // 3D Window Playground
    void setup3DPlayground();
    void load3DModel();
    void remove3DModel();
    void loadObjFiles(const QString& directoryPath, Qt3DCore::QEntity* rootEntity);
    void loadSingleObjFile(const QJsonObject &jsonObject, Qt3DCore::QEntity *rootEntity);
    void deleteCurrentObjFile(QStandardItem *currentItem);
    bool parseMtlFile(const QString& mtlFilePath, QColor& ambient, QColor& diffuse, QColor& specular, float& shininess, float& transparency, int& illumModel);
    void updateRotation();
    bool isCurrentRobotActive();
    QStandardItem* getParentRobotItem(QStandardItem *item);
    QStringList collectVisualizationPaths(QStandardItem *robotItem);
    
};
#endif // MAINWINDOW_H
