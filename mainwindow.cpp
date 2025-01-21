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

    // Now print the template object for debugging
    qDebug() << templateObject;

    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Label");
    model->setHeaderData(1, Qt::Horizontal, "Value");

    QStandardItem *rootItem = model->invisibleRootItem();
    populateModel(templateObject, rootItem);

    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
}

/*
void MainWindow::populateModel(const QJsonObject &jsonObject, QStandardItem *parentItem) {
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        if (it.value().isArray()) {
            QStandardItem *categoryItem = new QStandardItem(it.key());
            parentItem->appendRow(categoryItem);
            populateArray(it.value().toArray(), categoryItem);
        }
    }
}
*/

void MainWindow::populateModel(const QJsonObject &jsonObject, QStandardItem *parentItem) {
    
    QStandardItem *robotItem = nullptr;
    QStandardItem *jointsItem = nullptr;
    QStandardItem *kinematicsItem = nullptr;
    QStandardItem *dynamicsItem = nullptr;
    
    
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        if (it.key() == "Robot") {
            robotItem = new QStandardItem("Robot");
            
            populateArray(it.value().toArray(), robotItem);
        } else if (it.key() == "Joint") {
            jointsItem = new QStandardItem("Joints");
            
            populateArray(it.value().toArray(), jointsItem);
        } else if (it.key() == "JointKinematics") {
            kinematicsItem = new QStandardItem("JointKinematics");
           
            populateArray(it.value().toArray(), kinematicsItem);
        } else if (it.key() == "JointDynamics") {
            dynamicsItem = new QStandardItem("Joint Dynamics");
            
            populateArray(it.value().toArray(), dynamicsItem);
        }
    }

    //print all pointers values
    // qDebug() << "Robot Item: " << robotItem;
    // qDebug() << "Joints Item: " << jointsItem;
    // qDebug() << "Kinematics Item: " << kinematicsItem;
    // qDebug() << "Dynamics Item: " << dynamicsItem;


    if (robotItem) {
        parentItem->appendRow(robotItem);
        if (jointsItem) {
            robotItem->appendRow(jointsItem);
            if (kinematicsItem) {
                jointsItem->appendRow(kinematicsItem);
            }
            if (dynamicsItem) {
                jointsItem->appendRow(dynamicsItem);
            }
        }
    }


}

void MainWindow::populateArray(const QJsonArray &jsonArray, QStandardItem *parentItem) {
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
                    populateArray(it.value().toArray(), labelItem);
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


