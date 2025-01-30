#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "jsonkeys.h"

// Counters for Joints and Dynamics
// static int jointCount = 0;
// static int jointDynamicsCount = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Not define model here because it will not be accessible in other functions.
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Robot Specifications");
    model->setHeaderData(1, Qt::Horizontal, "User Values");

    // Configurations Panel part.
    addRobotDataTemplate();

    // Now We have data in the model we can just present that data in the view.
    ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);

    // Testing
    QString filePath = "C:/Users/fahmed/WorkFolder/Projects/testing.json";
   // QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->treeView->model());
    // saveToJson(model, filePath);

        // Print the data to the terminal
    QJsonObject json = modelToJson(model);
    QJsonDocument doc(json);
    //cout << doc.toJson().toStdString() << endl;



    // 3D Model Visualization part.
    // setup3DPlayground();
    // connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_actionSave_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/****************** Slots Implementation ******************/

void MainWindow::on_actionSave_triggered() {

    QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty()) {

        qDebug() << "Saving to: " << filePath;
        QStandardItemModel *model = static_cast<QStandardItemModel*>(ui->treeView->model());
        saveToJson(model, filePath);
    }
    
}












// Add the Robot Data Template
// This function will read the JSON file and populate the TreeView with the data.
// This function is part of Populating the template of Robot Data.
void MainWindow::addRobotDataTemplate()
{

    QFile file(":/Resources/Json/FaizanTest.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Template file is missing. Please add the template file.");
        return;
    }

    QByteArray templateJson = file.readAll();
    file.close();

    QJsonDocument templateDocument = QJsonDocument::fromJson(templateJson);
    QJsonObject templateObject = templateDocument.object();

    // Print the template object for debugging
    // qDebug() << templateObject;

    // First Step is to populate data in the model.
    populateTreeView(model, templateObject);

}

// I tired my best not to define the structue of the application in the model and to load the structure from the template file, to make it Dynamic.
// But I am unable to do so, because JSON Object is unordered list of key value pairs and for us Order is very important.
// So for now I am defining the Structure of the application in this function using TreeView Standard Model.
// I don't like this approach but I also tried ten other methods :)

void MainWindow::populateTreeView(QStandardItemModel *model, const QJsonObject &json)
{

    // print json
    // qDebug() << json;

    QStandardItem *rootItem = model->invisibleRootItem();

    if (!json.contains(RobotKeys::Robot) || !json[RobotKeys::Robot].isObject())
    {
        qWarning() << "Invalid JSON: Missing or invalid 'Robot' object";
        return;
    }

    QJsonObject robot = json[RobotKeys::Robot].toObject();
    // qDebug() << robot;
    QStandardItem *robotItem = new QStandardItem(QIcon(":/Resources/Icons/robot2.png"), RobotKeys::Robot);
    robotItem->setFlags(robotItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable
    rootItem->appendRow(robotItem);

    // Loading robot properties
    addItem(robotItem, RobotKeys::RobotName, robot[RobotKeys::RobotName].toString());
    addItem(robotItem, RobotKeys::RobotManufacturer, robot[RobotKeys::RobotManufacturer].toString());
    addItem(robotItem, RobotKeys::RobotPayload, QString::number(robot[RobotKeys::RobotPayload].toDouble()));
    addItem(robotItem, RobotKeys::RobotFootprint, QString::number(robot[RobotKeys::RobotFootprint].toDouble()));
    addItem(robotItem, RobotKeys::RobotMaxReach, QString::number(robot[RobotKeys::RobotMaxReach].toDouble()));
    addItem(robotItem, RobotKeys::RobotRepeatability, QString::number(robot[RobotKeys::RobotRepeatability].toDouble()));
    addItem(robotItem, RobotKeys::RobotWeight, QString::number(robot[RobotKeys::RobotWeight].toDouble()));
    addItem(robotItem, RobotKeys::DOF, QString::number(robot[RobotKeys::DOF].toInt()));

    // Loading joints

    // Loading joints
    if (!robot.contains(RobotKeys::Joints) || !robot[RobotKeys::Joints].isObject())
    {
        qWarning() << "Invalid JSON: Missing or invalid 'Joints' object";
        return;
    }

    QJsonObject joints = robot[RobotKeys::Joints].toObject();
    // qDebug() << joints;
    QStandardItem *jointsItem = new QStandardItem(RobotKeys::Joints);
    jointsItem->setFlags(jointsItem->flags() & ~Qt::ItemIsEditable);
    robotItem->appendRow(jointsItem);

    foreach (const QString &jointKey, joints.keys())
    {
        QJsonObject joint = joints[jointKey].toObject();
        QStandardItem *jointItem = new QStandardItem(JointKeys::JointName + ": " + joint[JointKeys::JointName].toString());
        jointsItem->appendRow(jointItem);

        addItem(jointItem, JointKeys::MotionRangeMax, QString::number(joint[JointKeys::MotionRangeMax].toDouble()));
        addItem(jointItem, JointKeys::MotionRangeMin, QString::number(joint[JointKeys::MotionRangeMin].toDouble()));
        addItem(jointItem, JointKeys::JointSpeedLimit, QString::number(joint[JointKeys::JointSpeedLimit].toDouble()));
        addItem(jointItem, JointKeys::FrictionCoefficient, QString::number(joint[JointKeys::FrictionCoefficient].toDouble()));
        addItem(jointItem, JointKeys::StiffnessCoefficient, QString::number(joint[JointKeys::StiffnessCoefficient].toDouble()));
        addItem(jointItem, JointKeys::DampingCoefficient, QString::number(joint[JointKeys::DampingCoefficient].toDouble()));

        // Loading kinematics
        QJsonObject kinematics = joint[JointKeys::JointKinematics].toObject();
        QStandardItem *kinematicsItem = new QStandardItem(JointKeys::JointKinematics);
        kinematicsItem->setFlags(kinematicsItem->flags() & ~Qt::ItemIsEditable);
        jointItem->appendRow(kinematicsItem);

        if (kinematics.contains(KinematicsKeys::DhParameters) && kinematics[KinematicsKeys::DhParameters].isObject())
        {

            QJsonObject dhParameters = kinematics[KinematicsKeys::DhParameters].toObject();
            QStandardItem *dhParametersItem = new QStandardItem(KinematicsKeys::DhParameters);
            dhParametersItem->setFlags(dhParametersItem->flags() & ~Qt::ItemIsEditable);
            kinematicsItem->appendRow(dhParametersItem);
            addItem(dhParametersItem, DhParametersKeys::Alpha, QString::number(dhParameters[DhParametersKeys::Alpha].toDouble()));
            addItem(dhParametersItem, DhParametersKeys::D, QString::number(dhParameters[DhParametersKeys::D].toDouble()));
            addItem(dhParametersItem, DhParametersKeys::Theta, QString::number(dhParameters[DhParametersKeys::Theta].toDouble()));
            addItem(dhParametersItem, DhParametersKeys::A, QString::number(dhParameters[DhParametersKeys::A].toDouble()));
            addItem(dhParametersItem, DhParametersKeys::DHType, dhParameters[DhParametersKeys::DHType].toString());
        }

        if (kinematics.contains(KinematicsKeys::RotationalValues) && kinematics[KinematicsKeys::RotationalValues].isObject())
        {
            QJsonObject rotationalValues = kinematics[KinematicsKeys::RotationalValues].toObject();
            QStandardItem *rotationalValuesItem = new QStandardItem(KinematicsKeys::RotationalValues);
            rotationalValuesItem->setFlags(rotationalValuesItem->flags() & ~Qt::ItemIsEditable);
            kinematicsItem->appendRow(rotationalValuesItem);
            addItem(rotationalValuesItem, RotationalValuesKeys::Ixx, QString::number(rotationalValues[RotationalValuesKeys::Ixx].toDouble()));
            addItem(rotationalValuesItem, RotationalValuesKeys::Ixy, QString::number(rotationalValues[RotationalValuesKeys::Ixy].toDouble()));
            addItem(rotationalValuesItem, RotationalValuesKeys::Ixz, QString::number(rotationalValues[RotationalValuesKeys::Ixz].toDouble()));
            addItem(rotationalValuesItem, RotationalValuesKeys::Iyy, QString::number(rotationalValues[RotationalValuesKeys::Iyy].toDouble()));
            addItem(rotationalValuesItem, RotationalValuesKeys::Iyz, QString::number(rotationalValues[RotationalValuesKeys::Iyz].toDouble()));
            addItem(rotationalValuesItem, RotationalValuesKeys::Izz, QString::number(rotationalValues[RotationalValuesKeys::Izz].toDouble()));
        }

        // Loading dynamics

        if (!joint.contains(JointKeys::JointDynamics) || !joint[JointKeys::JointDynamics].isObject())
        {
            qWarning() << "Invalid JSON: Missing or invalid 'JointDynamics' object";
            continue;
        }
        QJsonObject dynamics = joint[JointKeys::JointDynamics].toObject();
        QStandardItem *dynamicsItem = new QStandardItem(JointKeys::JointDynamics);
        dynamicsItem->setFlags(dynamicsItem->flags() & ~Qt::ItemIsEditable);
        jointItem->appendRow(dynamicsItem);

        foreach (const QString &payloadKey, dynamics.keys())
        {
            QJsonObject payload = dynamics[payloadKey].toObject();
            QStandardItem *payloadItem = new QStandardItem("Payload: " + payloadKey);
            dynamicsItem->appendRow(payloadItem);

            addItem(payloadItem, DynamicsKeys::TestPayload, QString::number(payload[DynamicsKeys::TestPayload].toDouble()));
            addItem(payloadItem, DynamicsKeys::PayloadPercentage, QString::number(payload[DynamicsKeys::PayloadPercentage].toDouble()));
            addItem(payloadItem, DynamicsKeys::RepeatabilityPercentage, QString::number(payload[DynamicsKeys::RepeatabilityPercentage].toDouble()));
            addItem(payloadItem, DynamicsKeys::SpeedPercentage, QString::number(payload[DynamicsKeys::SpeedPercentage].toDouble()));
            addItem(payloadItem, DynamicsKeys::BreakingDistance, QString::number(payload[DynamicsKeys::BreakingDistance].toDouble()));
            addItem(payloadItem, DynamicsKeys::BreakingTime, QString::number(payload[DynamicsKeys::BreakingTime].toDouble()));
        }

        // Loading visualization
        if (!joint.contains(JointKeys::Visualization) || !joint[JointKeys::Visualization].isObject())
        {
            qWarning() << "Invalid JSON: Missing or invalid 'Visualization' object";
            continue;
        }
        QJsonObject visualization = joint[JointKeys::Visualization].toObject();
        QStandardItem *visualizationItem = new QStandardItem(JointKeys::Visualization);
        visualizationItem->setFlags(visualizationItem->flags() & ~Qt::ItemIsEditable);
        jointItem->appendRow(visualizationItem);
        addItem(visualizationItem, VisualizationKeys::PathToObjFile, visualization[VisualizationKeys::PathToObjFile].toString());
        addItem(visualizationItem, VisualizationKeys::PathToMltFile, visualization[VisualizationKeys::PathToMltFile].toString());
    }
}

// Using this function to handle each row of the TreeView
void MainWindow::addItem(QStandardItem *parent, const QString &key, const QString &value)
{
    QStandardItem *keyItem = new QStandardItem(key);
    keyItem->setEditable(false);
    QStandardItem *valueItem = new QStandardItem(value);
    valueItem->setEditable(true);
    parent->appendRow(QList<QStandardItem *>() << keyItem << valueItem);
}

// Now trickies part starts with saving JSON in such a way that I can access it later on and able to load the data in TreeView.
// Not to make any mistake with structure, otherwise it will be surprice for you on loading data.

QJsonObject MainWindow::modelToJson(QStandardItemModel *model) {
    QJsonObject json;
    QJsonObject robotObject;

    QStandardItem *rootItem = model->invisibleRootItem();
    for (int i = 0; i < rootItem->rowCount(); ++i) {
        QStandardItem *robotItem = rootItem->child(i);

        // Print everything in robotItem
        qDebug() << "Robot Item:" << robotItem->text();

        for (int j = 0; j < robotItem->rowCount(); ++j) {
            QStandardItem *propertyItem = robotItem->child(j, 0);
            QStandardItem *valueItem = robotItem->child(j, 1);
            if (propertyItem && valueItem) {
                robotObject[propertyItem->text()] = valueItem->text();
            }
        }

        QJsonObject jointsObject;
        QStandardItem *jointsItem = robotItem->child(robotItem->rowCount() - 1);
        for (int j = 0; j < jointsItem->rowCount(); ++j) {
            QStandardItem *jointItem = jointsItem->child(j);
            QJsonObject jointObject;

            for (int k = 0; k < jointItem->rowCount(); ++k) {
                QStandardItem *propertyItem = jointItem->child(k, 0);
                QStandardItem *valueItem = jointItem->child(k, 1);
                if (propertyItem && valueItem) {
                    jointObject[propertyItem->text()] = valueItem->text();
                }
            }

            QJsonObject kinematicsObject;
            // very bad appraoch , will change it after testing
            QStandardItem *kinematicsItem = jointItem->child(jointItem->rowCount() - 3);
            for (int k = 0; k < kinematicsItem->rowCount(); ++k) {
                QStandardItem *propertyItem = kinematicsItem->child(k, 0);
                QStandardItem *valueItem = kinematicsItem->child(k, 1);
                if (propertyItem && valueItem) {
                    kinematicsObject[propertyItem->text()] = valueItem->text();
                }
            }
            jointObject[JointKeys::JointKinematics] = kinematicsObject;

            QJsonObject dynamicsObject;
            QStandardItem *dynamicsItem = jointItem->child(jointItem->rowCount() - 2);
            for (int k = 0; k < dynamicsItem->rowCount(); ++k) {
                QStandardItem *payloadItem = dynamicsItem->child(k);
                QJsonObject payloadObject;

                for (int l = 0; l < payloadItem->rowCount(); ++l) {
                    QStandardItem *propertyItem = payloadItem->child(l, 0);
                    QStandardItem *valueItem = payloadItem->child(l, 1);
                    if (propertyItem && valueItem) {
                        payloadObject[propertyItem->text()] = valueItem->text();
                    }
                }
                dynamicsObject[payloadItem->text()] = payloadObject;
            }
            jointObject[JointKeys::JointDynamics] = dynamicsObject;

            QJsonObject visualizationObject;
            QStandardItem *visualizationItem = jointItem->child(jointItem->rowCount() - 1);
            for (int k = 0; k < visualizationItem->rowCount(); ++k) {
                QStandardItem *propertyItem = visualizationItem->child(k, 0);
                QStandardItem *valueItem = visualizationItem->child(k, 1);
                if (propertyItem && valueItem) {
                    visualizationObject[propertyItem->text()] = valueItem->text();
                }
            }
            jointObject[JointKeys::Visualization] = visualizationObject;

            jointsObject[jointItem->text()] = jointObject;
        }
        robotObject[RobotKeys::Joints] = jointsObject;
    }
    json[RobotKeys::Robot] = robotObject;
    QDebug() << "Robot JSON" << json;
    return json;
}



void MainWindow::saveToJson(QStandardItemModel *model, const QString &filePath) {
    QJsonObject json = modelToJson(model);
    QJsonDocument doc(json);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
    else {
        qWarning() << "Failed to open file for writing";
    }
}



// Setup the Main Playground for 3D Viewer
// This function will setup the 3D Playground for the Robot Model Visualization.
void MainWindow::setup3DPlayground()
{

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

