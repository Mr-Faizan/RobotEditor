#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Counters for Joints and Dynamics
static int jointCount = 0;
static int jointDynamicsCount = 0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configurations Panel part.
    addRobotDataTemplate();

    // Testing
    populateNewJoint();
    populateNewJoint();
    populateNewJointDynamics();
    populateNewJointDynamics();

    //3D Model Visualization part.
    setup3DPlayground();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Add the Robot Data Template
// This function will read the JSON file and populate the TreeView with the data.
// This function is part of Populating the template of Robot Data.
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
// This function is part of Populating the template of Robot Data.
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
            // This is to combine all Joint under one tab.
            jointsCategory = new QStandardItem("Joints");
            jointItem = new QStandardItem(QString("Joint %1").arg(++jointCount));
            jointItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointItemKeysArray, jointItem);
        } else if (it.key() == "JointKinematics") {
            jointKinematicsItem = new QStandardItem("Joint Kinematics");
            jointKinematicsItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointKinematicsItemKeysArray, jointKinematicsItem);
        } else if (it.key() == "JointDynamics") {
            // This is to combine all Dynamics under one tab.
            jointDynamicsCategory = new QStandardItem("Joint Dynamics");
            jointDynamicsItem = new QStandardItem(QString("Payload %1").arg(++jointDynamicsCount));
            jointDynamicsItemKeysArray = it.value().toArray();
            populateTreeViewNodes(jointDynamicsItemKeysArray, jointDynamicsItem);
        }
    }

    // Until now all the Items are created separately,
    // Like we have Robot Item, Joints Item, Kinematics Item and Dynamics Item
    // Now we need to append them to make a tree structure.

    if (robotItem) {
        parentItem->appendRow(robotItem);
        if(jointsCategory) {
            robotItem->appendRow(jointsCategory);
        }
        if (jointItem) {
            jointsCategory->appendRow(jointItem);
            if (jointKinematicsItem) {
                jointItem->appendRow(jointKinematicsItem);
            }
            if (jointDynamicsItem) {
                jointItem->appendRow(jointDynamicsCategory);
                jointDynamicsCategory->appendRow(jointDynamicsItem);

            }
        }
    }

    //print all pointers values
    //qDebug() << "Robot Item: " << &robotItem;
    // qDebug() << "Joints Item: " << jointsItem;
    // qDebug() << "Kinematics Item: " << kinematicsItem;
    // qDebug() << "Dynamics Item: " << dynamicsItem;


}



// This function will populate the Sub  Nodes of the TreeView like Joints, Kinematics, Dynamics etc.
// This function is part of Populating the template of Robot Data.
void MainWindow::populateTreeViewNodes(const QJsonArray &jsonArray, QStandardItem *parentItem) {
    for (const QJsonValue &value : jsonArray) {
        if (value.isString()) {
            QStandardItem *labelItem = new QStandardItem(value.toString());
            QStandardItem *valueItem = new QStandardItem();

            // Set labelItem as non-editable
            labelItem->setFlags(labelItem->flags() & ~Qt::ItemIsEditable);

            // Set valueItem as editable
            valueItem->setFlags(valueItem->flags() | Qt::ItemIsEditable);

            QList<QStandardItem*> rowItems;
            rowItems << labelItem << valueItem;
            parentItem->appendRow(rowItems);
        } else if (value.isObject()) {
            QJsonObject obj = value.toObject();
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                QStandardItem *labelItem = new QStandardItem(it.key());
                QStandardItem *valueItem = new QStandardItem();

            // Set labelItem as non-editable
            labelItem->setFlags(labelItem->flags() & ~Qt::ItemIsEditable);
            // Set valueItem as editable
            valueItem->setFlags(valueItem->flags() | Qt::ItemIsEditable);
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



// This function will populate the new Joint for the Robot Model.
void MainWindow::populateNewJoint() {
    QStandardItem *newJointItem = new QStandardItem(QString("Joint %1").arg(++jointCount));
    populateTreeViewNodes(jointItemKeysArray, newJointItem);
    if (jointsCategory) {
        jointsCategory->appendRow(newJointItem);
    }
}

// This function will populate the new Dynamics of the Joint for the Robot Model.
void MainWindow::populateNewJointDynamics() {
    
    QStandardItem *newDynamicsItem = new QStandardItem(QString("Payload %1").arg(++jointDynamicsCount));
    populateTreeViewNodes(jointDynamicsItemKeysArray, newDynamicsItem);
    if (jointDynamicsCategory) {
        jointDynamicsCategory->appendRow(newDynamicsItem);
    }
}


// Setup the Main Playground for 3D Viewer
// This function will setup the 3D Playground for the Robot Model Visualization.
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
  //  QHBoxLayout *layout = new QHBoxLayout();
  //  layout->addWidget(container);
  //  ui->viewContainer->setLayout(layout);

  ui->viewContainer->layout()->addWidget(container);
}


