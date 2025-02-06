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

    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // Not define model here because it will not be accessible in other functions.
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Robot Specifications");
    model->setHeaderData(1, Qt::Horizontal, "User Values");
    ui->treeView->setModel(model);


    // Configurations Panel part.
    // addRobotDataTemplate();

    // 3D Model Visualization part.
    setup3DPlayground();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************** Slots Implementation ******************/

void MainWindow::addNewJoint()
{
    qDebug() << "Add New Joint action triggered";
}

void MainWindow::addNewDynamics()
{
    qDebug() << "Add New Dynamics action triggered";
}

void MainWindow::on_actionSave_triggered()
{

    QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty())
    {

        qDebug() << "Saving to: " << filePath;
        saveToJson(filePath);
    }
}

void MainWindow::on_actionNewRobot_triggered()
{
    // here we have to clear the model and add the new robot data template. from the template file.
    // model->clear();
    addRobotDataTemplate();
}

void MainWindow::on_actionOpenFromDevice_triggered()
{
    // Open the JSON file from the device and populate the data in the TreeView.

    QString filePath = QFileDialog::getOpenFileName(this, "Open JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty())
    {
        qDebug() << "Opening from: " << filePath;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning("Failed to open file for reading");
            return;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = jsonDocument.object();

        // Using one common function to load JSON data
        loadJsonData(jsonObject);

    }
}

/****************** Custom Function Implementation ******************/
void MainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid())
        return;

    QStandardItem *item = model->itemFromIndex(index);
    if (!item)
        return;

    QMenu contextMenu(this);

    if (item->text() == RobotKeys::Robot)
    {
        contextMenu.addAction("Save Robot", this, SLOT(on_actionSave_triggered()));
        contextMenu.addAction("New Robot", this, SLOT(on_actionNewRobot_triggered()));
        contextMenu.addAction("Open from Device...", this, SLOT(on_actionOpenFromDevice_triggered()));
    }
    else if (item->text() == RobotKeys::Joints)
    {
        contextMenu.addAction("Add New Joint", this, SLOT(addNewJoint()));
    }
    else if (item->text() == JointKeys::JointDynamics)
    {
        contextMenu.addAction("Add New Dynamics", this, SLOT(addNewDynamics()));
    }

    contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

// Add the Robot Data Template
// This function will read the JSON file and populate the TreeView with the data.
// This function is part of Populating the template of Robot Data.
void MainWindow::addRobotDataTemplate()
{

    QFile file(":/Resources/Json/Template.json");

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

    // Using one common function to load JSON data
    loadJsonData(templateObject);

}


// This function will get the JSON Object and populate the TreeView with the data.
void MainWindow::loadJsonData(const QJsonObject &jsonObject)
{

    if (jsonObject.isEmpty())
    {
        qWarning() << "JSON object is empty";
        return;
    }

    if (!jsonObject.contains(RobotKeys::Robot) || !jsonObject[RobotKeys::Robot].isObject())
    {
        qWarning() << "Invalid JSON: Missing or invalid 'Robot' object";
        return;
    }

    // Clear the model first
    //model->clear();

    // Initialize the model
    //model = new QStandardItemModel(0, 2, this);
    //model->setHeaderData(0, Qt::Horizontal, "Robot Specifications");
    //model->setHeaderData(1, Qt::Horizontal, "User Values");

    // Populate data in the model
    populateTreeView(model, jsonObject);

    // Present the data in the view
    //ui->treeView->setModel(model);
    ui->treeView->expandAll();
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
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
    QStandardItem *robotItem = new QStandardItem(QIcon(":/Resources/Icons/robotic-arm.png"), RobotKeys::Robot);
    robotItem->setFlags(robotItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable
    // Set the font to bold
    QFont font = robotItem->font();
    font.setBold(true);
    robotItem->setFont(font);

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
    QStandardItem *jointsItem = new QStandardItem(QIcon(":/Resources/Icons/robot-joint.png"), RobotKeys::Joints);
    jointsItem->setFlags(jointsItem->flags() & ~Qt::ItemIsEditable);
    robotItem->appendRow(jointsItem);

    foreach (const QString &jointKey, joints.keys())
    {
        QJsonObject joint = joints[jointKey].toObject();
        QStandardItem *singleJointItem = new QStandardItem(JointKeys::JointName + ": " + joint[JointKeys::JointName].toString());
        jointsItem->appendRow(singleJointItem);

        addItem(singleJointItem, JointKeys::MotionRangeMax, QString::number(joint[JointKeys::MotionRangeMax].toDouble()));
        addItem(singleJointItem, JointKeys::MotionRangeMin, QString::number(joint[JointKeys::MotionRangeMin].toDouble()));
        addItem(singleJointItem, JointKeys::JointSpeedLimit, QString::number(joint[JointKeys::JointSpeedLimit].toDouble()));
        addItem(singleJointItem, JointKeys::FrictionCoefficient, QString::number(joint[JointKeys::FrictionCoefficient].toDouble()));
        addItem(singleJointItem, JointKeys::StiffnessCoefficient, QString::number(joint[JointKeys::StiffnessCoefficient].toDouble()));
        addItem(singleJointItem, JointKeys::DampingCoefficient, QString::number(joint[JointKeys::DampingCoefficient].toDouble()));

        // Loading kinematics
        QJsonObject kinematics = joint[JointKeys::JointKinematics].toObject();
        QStandardItem *kinematicsItem = new QStandardItem(JointKeys::JointKinematics);
        kinematicsItem->setFlags(kinematicsItem->flags() & ~Qt::ItemIsEditable);
        singleJointItem->appendRow(kinematicsItem);

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
        QStandardItem *dynamicsItem = new QStandardItem(QIcon(":/Resources/Icons/settings.png"), JointKeys::JointDynamics);
        dynamicsItem->setFlags(dynamicsItem->flags() & ~Qt::ItemIsEditable);
        singleJointItem->appendRow(dynamicsItem);

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
        singleJointItem->appendRow(visualizationItem);
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

QJsonObject MainWindow::modelToJson()
{
    QJsonObject json;
    QJsonObject robotObject;

    // make sure model exists
    if (!model)
    {
        qWarning() << "Model does not exist";
        return json;
    }

    QStandardItem *rootItem = model->invisibleRootItem();

    // rootItem must exist and have at least one child
    if (rootItem && rootItem->rowCount() > 0)
    {
        for (int i = 0; i < rootItem->rowCount(); ++i)
        {
            QStandardItem *robotItem = rootItem->child(i);
            QStandardItem *jointsItem = nullptr;

            // Print everything in robotItem
            // qDebug() << "Robot Item:" << robotItem->text();

            for (int j = 0; j < robotItem->rowCount(); ++j)
            {
                QStandardItem *propertyItem = robotItem->child(j, 0);
                QStandardItem *valueItem = robotItem->child(j, 1);

                // property is mandatory but value is optional
                if (propertyItem)
                {
                    // Two cases here , Either it just have value or ValueItem has inner structure.
                    if (propertyItem->text() == RobotKeys::Joints)
                    {
                        // just take the jointsItem for further processing
                        jointsItem = robotItem->child(j);
                    }
                    else
                    {
                        robotObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                    }
                }
            }

            // As we took JointsItem in above loop, now we will process it.
            QJsonObject jointsObject;

            if (jointsItem)
            {
                for (int j = 0; j < jointsItem->rowCount(); ++j)
                {
                    // Here we will process each Joint item.
                    QStandardItem *singleJointItem = jointsItem->child(j);
                    QJsonObject singleJointObject;
                    // Also declare the inner objects and pointers here
                    QStandardItem *kinematicsItem = nullptr;
                    QStandardItem *dynamicsItem = nullptr;
                    QStandardItem *visualizationItem = nullptr;

                    QJsonObject kinematicsObject;
                    QJsonObject dynamicsObject;
                    QJsonObject visualizationObject;

                    // Its better to handle all the inner structure like kinematics, dynamics and visualization of singleJointItem separately.
                    for (int k = 0; k < singleJointItem->rowCount(); ++k)
                    {
                        QStandardItem *propertyItem = singleJointItem->child(k, 0);
                        QStandardItem *valueItem = singleJointItem->child(k, 1);

                        if (propertyItem)
                        {
                            // Two cases here , Either it just have value or ValueItem has inner structure.
                            if (propertyItem->text() == JointKeys::JointKinematics)
                            {
                                // Keep the KinematicsObject for further processing
                                kinematicsItem = singleJointItem->child(k);
                            }
                            else if (propertyItem->text() == JointKeys::JointDynamics)
                            {
                                dynamicsItem = singleJointItem->child(k);
                            }
                            else if (propertyItem->text() == JointKeys::Visualization)
                            {
                                visualizationItem = singleJointItem->child(k);
                            }
                            else
                            {
                                singleJointObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                            }
                        }
                    }

                    // Now we will process the KinematicsItem
                    if (kinematicsItem)
                    {
                        for (int k = 0; k < kinematicsItem->rowCount(); ++k)
                        {
                            QStandardItem *propertyItem = kinematicsItem->child(k, 0);
                            QStandardItem *valueItem = kinematicsItem->child(k, 1);

                            if (propertyItem)
                            {
                                if (propertyItem->text() == KinematicsKeys::DhParameters)
                                {
                                    QStandardItem *dhParametersItem = kinematicsItem->child(k);
                                    QJsonObject dhParametersObject;

                                    if (dhParametersItem)
                                    {
                                        for (int l = 0; l < dhParametersItem->rowCount(); ++l)
                                        {
                                            QStandardItem *propertyItem = dhParametersItem->child(l, 0);
                                            QStandardItem *valueItem = dhParametersItem->child(l, 1);

                                            if (propertyItem)
                                            {
                                                dhParametersObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                                            }
                                        }
                                        kinematicsObject[KinematicsKeys::DhParameters] = dhParametersObject;
                                    }
                                }
                                else if (propertyItem->text() == KinematicsKeys::RotationalValues)
                                {
                                    // Handle RotationalValues here
                                    QStandardItem *rotationalValuesItem = kinematicsItem->child(k);
                                    QJsonObject rotationalValuesObject;

                                    if (rotationalValuesItem)
                                    {
                                        for (int l = 0; l < rotationalValuesItem->rowCount(); ++l)
                                        {
                                            QStandardItem *propertyItem = rotationalValuesItem->child(l, 0);
                                            QStandardItem *valueItem = rotationalValuesItem->child(l, 1);

                                            if (propertyItem)
                                            {
                                                rotationalValuesObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                                            }
                                        }
                                        kinematicsObject[KinematicsKeys::RotationalValues] = rotationalValuesObject;
                                    }
                                }
                                else
                                {
                                    kinematicsObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                                }
                            }
                        }
                        singleJointObject[JointKeys::JointKinematics] = kinematicsObject;
                    }

                    // Now we will process the DynamicsItem

                    if (dynamicsItem)
                    {
                        for (int k = 0; k < dynamicsItem->rowCount(); ++k)
                        {
                            QStandardItem *payloadItem = dynamicsItem->child(k);
                            QJsonObject payloadObject;

                            for (int l = 0; l < payloadItem->rowCount(); ++l)
                            {
                                QStandardItem *propertyItem = payloadItem->child(l, 0);
                                QStandardItem *valueItem = payloadItem->child(l, 1);

                                if (propertyItem)
                                {
                                    payloadObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                                }
                            }
                            dynamicsObject[payloadItem->text()] = payloadObject;
                        }
                        singleJointObject[JointKeys::JointDynamics] = dynamicsObject;
                    }

                    // Now we will process the VisualizationItem
                    if (visualizationItem)
                    {
                        for (int k = 0; k < visualizationItem->rowCount(); ++k)
                        {
                            QStandardItem *propertyItem = visualizationItem->child(k, 0);
                            QStandardItem *valueItem = visualizationItem->child(k, 1);
                            if (propertyItem)
                            {
                                visualizationObject[propertyItem->text()] = valueItem ? valueItem->text() : "";
                            }
                        }
                        singleJointObject[JointKeys::Visualization] = visualizationObject;
                    }
                    jointsObject[singleJointItem->text()] = singleJointObject;
                }
                robotObject[RobotKeys::Joints] = jointsObject;
            }
        }
        json[RobotKeys::Robot] = robotObject;
        // qDebug() << "Robot JSON" << json;
    }
    return json;
}

void MainWindow::saveToJson(const QString &filePath)
{
    QJsonObject json = modelToJson();

    // Before saving to file, make sure the JSON object has valid Json data
    if (json.isEmpty())
    {
        qWarning() << "Failed to convert model to JSON";
        return;
    }
    // verify that the existing json has valid foramtted data.

    QJsonDocument doc(json);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
    }
    else
    {
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
