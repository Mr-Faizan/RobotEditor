#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // First we need to add the template of Robot data from the file.
    addRobotDataTemplate();


    // Setup the 3D Playground
    setup3DPlayground();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Add the Robot Data Template
void MainWindow::addRobotDataTemplate() {


    QFile file(":/Resources/Json/Template.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Template file is missing. Please add the template file.");
        return;
    }

    QByteArray templateJson = file.readAll();
    file.close();


    QJsonDocument templateDocument = QJsonDocument::fromJson(templateJson);
    QJsonObject templateObject = templateDocument.object();

    // Print the template object for debugging
    // qDebug() << templateObject;

    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Robot Specifications");
    model->setHeaderData(1, Qt::Horizontal, "User Values");

    QStandardItem *rootItem = model->invisibleRootItem();
    populateTreeView(templateObject, rootItem);

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
}

// Populating the Model for the TreeView from the JSON Object
void MainWindow::populateTreeView(const QJsonObject &jsonObject, QStandardItem *parentItem) {
    
    if (jsonObject.isEmpty() || parentItem == nullptr) {
        qWarning("JSON Object is empty or parentItem is null in populateTreeView");
        return;
    }

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        if (it.key() == "Robot") {
            robotItem = new QStandardItem("Robot");
            // get the array of robot data
            robotItemKeysArray = it.value().toArray();
            populateTreeViewNodes(robotItemKeysArray, robotItem);
        } else if (it.key() == "Joint") {
            jointsItem = new QStandardItem("Joints");
            jointItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointItemKeysArray, jointsItem);
        } else if (it.key() == "JointKinematics") {
            jointKinematicsItem = new QStandardItem("Joint Kinematics");
            jointKinematicsItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointKinematicsItemKeysArray, jointKinematicsItem);
        } else if (it.key() == "JointDynamics") {
            jointDynamicsItem = new QStandardItem("Joint Dynamics");
            jointDynamicsItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointDynamicsItemKeysArray, jointDynamicsItem);
        }
    }

    if (robotItem) {
        parentItem->appendRow(robotItem);
        if (jointsItem) {
            robotItem->appendRow(jointsItem);
            if (jointKinematicsItem) {
                jointsItem->appendRow(jointKinematicsItem);
            }
            if (jointDynamicsItem) {
                jointsItem->appendRow(jointDynamicsItem);
            }
        }
    }

        //print all pointers values
     qDebug() << "Robot Item: " << &robotItem;
    // qDebug() << "Joints Item: " << jointsItem;
    // qDebug() << "Kinematics Item: " << kinematicsItem;
    // qDebug() << "Dynamics Item: " << dynamicsItem;


}

void MainWindow::populateTreeViewNodes(const QJsonArray &jsonArray, QStandardItem *parentItem) {
    for (const QJsonValue &value : jsonArray) {
        if (value.isString()) {
            QStandardItem *labelItem = new QStandardItem(value.toString());
            QStandardItem *valueItem = new QStandardItem();
            QList<QStandardItem*> rowItems;
            rowItems << labelItem << valueItem;
            parentItem->appendRow(rowItems);
        } else if (value.isObject()) {
            QJsonObject obj = value.toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QStandardItem *labelItem = new QStandardItem(it.key());
                QStandardItem *valueItem = new QStandardItem();
                QList<QStandardItem*> rowItems;
                rowItems << labelItem << valueItem;
                parentItem->appendRow(rowItems);
                if (it.value().isArray()) {
                    populateTreeViewNodes(it.value().toArray(), labelItem);
                }
            }
        }
    }
}


// Populating the Model for the TreeView from the JSON Object
/*
void MainWindow::populateModel(const QJsonObject &jsonObject, QStandardItem *parentItem) {

    if(jsonObject.isEmpty() || parentItem == nullptr) {
        return;
    }
    
    for(auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        QStandardItem *labelItem = new QStandardItem(it.key());
        QStandardItem *valueItem = new QStandardItem();

        if(it.value().isObject()) {
            // Recursion might be good idea to dig down the children Object of JSON
            QStandardItem *childItem = new QStandardItem(it.key());
            QList<QStandardItem*> rowItems;
            rowItems << childItem << new QStandardItem();
            parentItem->appendRow(rowItems);

            populateModel(it.value().toObject(), childItem);
        } else {

            // Just add key value pair to the model
            QList<QStandardItem*> rowItems;
            rowItems << labelItem << valueItem;
            parentItem->appendRow(rowItems);

            if(it.value().isString()) {
                valueItem->setData(it.value().toString(), Qt::DisplayRole);
            } else if(it.value().isDouble()) {
                valueItem->setData(it.value().toDouble(), Qt::DisplayRole);
            } 
        }
    }


}

*/

// Setup the Main Playground for 3D Viewer
void MainWindow::setup3DPlayground() {

    // First creating 3D window
    view = new Qt3DExtras::Qt3DWindow();
    rootEntity = new Qt3DCore::QEntity();

    // Set the background color
    view->defaultFrameGraph()->setClearColor(QColor(Qt::darkCyan));

    // Set the root Entity
    view->setRootEntity(rootEntity);

    // Create a window container for view
    QWidget *container = QWidget::createWindowContainer(view);

    // Now its time to show this view on frontend.
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(container);
    ui->viewContainer->setLayout(layout);
}


