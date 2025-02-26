#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "jsonkeys.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rotationAngle(0.0f),
      rotating(false),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &MainWindow::updateRotation);

    // Load the JSON template
    loadTemplate();

    // Not define model here because it will not be accessible in other functions.
    model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Robot Specifications");
    model->setHeaderData(1, Qt::Horizontal, "User Values");
    ui->treeView->setModel(model);

    // 3D Model Visualization part.
    setup3DPlayground();
    // Set the root Entity
    view->setRootEntity(rootEntity);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************** Slots Implementation ******************/

void MainWindow::addNewJoint()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No Joints item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    // Ensure the selected item is the Joints item
    if (currentItem->text() != RobotKeys::Joints)
    {
        qWarning() << "Selected item is not the Joints item.";
        return;
    }

    // Create a new joint
    // QString jointKey = JointKeys::Joint + " " + QString::number(currentItem->rowCount() + 1);
    QString jointKey = JointKeys::Joint;
    QJsonObject joint = templateObject[RobotKeys::Robot].toObject()[RobotKeys::Joints].toObject()[jointKey].toObject();

    addJoint(currentItem, jointKey, joint);

    ui->treeView->expand(currentIndex);
}

void MainWindow::addNewDynamics()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No Joint Dynamics selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    // Check the selected item is a Joint Dynamics item
    if (currentItem->text() != JointKeys::JointDynamics)
    {
        qWarning() << "Selected item is not a Joint Dynamics.";
        return;
    }

    // Create a new payload
    QString payloadKey = DynamicsKeys::Payload + " " + QString::number(currentItem->rowCount() + 1);
    QJsonObject payload = templateObject[RobotKeys::Robot].toObject()[RobotKeys::Joints].toObject()[JointKeys::JointDynamics].toObject()[payloadKey].toObject();

    // Create generic function.
    addDynamicsPayload(currentItem, payloadKey, payload);

    ui->treeView->expand(currentIndex);
}

// We need to mark one Robot as Active, so that we can only load the 3D Model of the Active Robot.
void MainWindow::on_actionActiveRobot_triggered()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    // Ensure the selected item is a Robot item
    if (currentItem->text() != RobotKeys::Robot)
    {
        qWarning() << "Selected item is not a Robot item.";
        return;
    }

    // Reset the font of the previously active robot to normal and collapse its tree view
    if (activeRobotItem && model->indexFromItem(activeRobotItem).isValid())
    {
        QFont font = activeRobotItem->font();
        font.setBold(false);
        activeRobotItem->setFont(font);
        ui->treeView->collapse(model->indexFromItem(activeRobotItem));
    }

    // Set the font of the selected robot to bold and expand its tree view
    QFont font = currentItem->font();
    font.setBold(true);
    currentItem->setFont(font);
    ui->treeView->expand(currentIndex);

    // Update the active robot item
    activeRobotItem = currentItem;
}

void MainWindow::on_actionSave_triggered()
{

    QString filePath = QFileDialog::getSaveFileName(this, "Save JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty())
    {
        QModelIndex currentIndex = ui->treeView->currentIndex();
        if (!currentIndex.isValid())
        {
            qWarning() << "No item selected.";
            return;
        }

        QStandardItem *currentItem = model->itemFromIndex(currentIndex);
        if (!currentItem)
        {
            qWarning() << "Invalid item selected.";
            return;
        }

        // Ensure the selected item is the Robot item
        if (currentItem->text() != RobotKeys::Robot)
        {
            qWarning() << "Selected item is not the Robot item.";
            return;
        }

        qDebug() << "Saving to: " << filePath;
        saveToJson(filePath, currentItem);
    }
}

void MainWindow::on_actionSaveAll_triggered()
{
    if (!model)
    {
        qWarning() << "Model does not exist";
        return;
    }

    QStandardItem *rootItem = model->invisibleRootItem();

    if (rootItem && rootItem->rowCount() > 0)
    {
        for (int i = 0; i < rootItem->rowCount(); ++i)
        {
            QStandardItem *robotItem = rootItem->child(i);

            QString filePath = QFileDialog::getSaveFileName(this, "Save JSON for " + robotItem->text(), "", "JSON Files (*.json)");
            if (!filePath.isEmpty())
            {
                qDebug() << "Saving to: " << filePath;
                saveToJson(filePath, robotItem);
            }
            else
            {
                qWarning() << "No file name provided for robot: " << robotItem->text();
            }
        }
    }
}

void MainWindow::on_actionNewRobot_triggered()
{
    if (!templateObject.isEmpty())
    {
        // Populate data in the model
        populateTreeView(templateObject);
    }
}

void MainWindow::on_actionOpenFromDevice_triggered()
{
    // Open the JSON file from the device and populate the data in the TreeView.

    QString filePath = QFileDialog::getOpenFileName(this, "Open JSON", "", "JSON Files (*.json)");
    if (!filePath.isEmpty())
    {
        qDebug() << "Opening from: " << filePath;

        QFileInfo fileInfo(filePath);
        if (fileInfo.fileName() == "FaizanTest.json")
        {
            show3dModel = true;
        }

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

        if (jsonObject.isEmpty())
        {
            qWarning() << "Given JSON object is empty";
            return;
        }

        if (!jsonObject.contains(RobotKeys::Robot) || !jsonObject[RobotKeys::Robot].isObject())
        {
            qWarning() << "Invalid JSON: Missing or invalid 'Robot' object";
            return;
        }

        // Populate data in the model
        populateTreeView(jsonObject);

        if (show3dModel)
        {
            // Load the 3D Model
            load3DModel();
        }
        else
        {
            // Remove the 3D Model
            // remove3DModel();
        }
    }
}

void MainWindow::on_actionResetModel_triggered()
{
    // commenting this because it creates issue with setting up to original position.
    // Reset the perspective projection (FOV)
    // camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // Reset the camera position
    camera->setPosition(QVector3D(0.159012, -504.859, -10.8127));

    // Reset the view center (where the camera is looking)
    camera->setViewCenter(QVector3D(0, 0, 0));

    // Optional: Reset the camera's up vector to ensure proper orientation
    camera->setUpVector(QVector3D(0, 1, 0));
}

void MainWindow::on_actionRotateModel_triggered()
{

    rotating = !rotating;
    if (rotating)
    {
        rotationTimer->start(16); // 60 FPS
    }
    else
    {
        rotationTimer->stop();
    }
}

void MainWindow::on_actionJointVisualization_triggered()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }


    QString filePath = QFileDialog::getOpenFileName(this, "Select OBJ File", "", "OBJ Files (*.obj)");
    if (!filePath.isEmpty())
    {
        // First Delete the current OBJ file if it exists
        deleteCurrentObjFile(currentItem);
        
        QStandardItem *filePathItem = model->itemFromIndex(currentItem->index().sibling(currentItem->row(), 1));
        if (filePathItem)
        {
            filePathItem->setText(filePath);
        }

        // Call the loadSingleObjFile function with the selected file path
        QJsonObject jsonObject;
        loadSingleObjFile(filePath, jsonObject, rootEntity);
    }
}

void MainWindow::on_actionDeleteAll_triggered()
{

    QStandardItem *rootItem = model->invisibleRootItem();
    if (!rootItem)
    {
        qWarning() << "Root item does not exist.";
        return;
    }

    // Reset the active robot item
    activeRobotItem = nullptr;

    // Remove all rows from the root item
    rootItem->removeRows(0, rootItem->rowCount());
    qDebug() << "Deleted all robots.";
}

void MainWindow::deleteCurrentRobot()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    if (currentItem->text() != RobotKeys::Robot)
    {
        qWarning() << "Selected item is not the Robot item.";
        return;
    }

    if (currentItem == activeRobotItem)
    {
        activeRobotItem = nullptr;
    }

    // Remove the selected robot item
    model->removeRow(currentItem->row(), currentItem->parent() ? currentItem->parent()->index() : QModelIndex());
    qDebug() << "Deleted robot: " << currentItem->text();
}

void MainWindow::deleteCurrentJoint()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    // if (!currentItem->text().startsWith(JointKeys::Joint))
    if (currentItem->text() != RobotKeys::Joints)
    {
        qWarning() << "Selected item is not a Joint item.";
        return;
    }

    // Get the last child of the selected joint
    int lastRow = currentItem->rowCount() - 1;
    if (lastRow >= 0)
    {
        // First delete the OBJ file from the 3D scene
        QStandardItem *jointItem = currentItem->child(lastRow);

        // Access the JointVisualization item directly by its name
        QStandardItem *visualizationItem = nullptr;
        for (int i = 0; i < jointItem->rowCount(); ++i)
        {
            QStandardItem *childItem = jointItem->child(i, 0);
            if (childItem && childItem->text() == JointKeys::Visualization)
            {
                visualizationItem = childItem;
                break;
            }
        }

        if (visualizationItem)
        {
            // Pass the JointVisualization item to deleteCurrentObjFile
            deleteCurrentObjFile(visualizationItem);
        }

        currentItem->removeRow(lastRow);
        qDebug() << "Deleted last child of joint: " << currentItem->text();
    }
}

void MainWindow::deleteCurrentPayload()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        qWarning() << "Invalid item selected.";
        return;
    }

    // Ensure the selected item is a Payload item
    // if (!currentItem->text().startsWith(DynamicsKeys::Payload))
    if (currentItem->text() != JointKeys::JointDynamics)
    {
        qWarning() << "Selected item is not a Payload item.";
        return;
    }
    // Get the last child of the selected joint
    int lastRow = currentItem->rowCount() - 1;
    if (lastRow >= 0)
    {
        currentItem->removeRow(lastRow);
        qDebug() << "Deleted last child of joint: " << currentItem->text();
    }
}

// This function will delete the current OBJ file from the 3D scene
void MainWindow::deleteCurrentObjFile(QStandardItem *currentItem)
{
    if (currentItem)
    {
        QStandardItem *filePathItem = model->itemFromIndex(currentItem->index().sibling(currentItem->row(), 1));
        if (filePathItem)
        {
            QString filePath = filePathItem->text();
            if (!filePath.isEmpty())
            {
                // Remove the entity from the 3D scene
                if (entityMap.contains(filePath))
                {
                    Qt3DCore::QEntity *entity = entityMap[filePath];
                    delete entity; // This removes the entity from the scene
                    entityMap.remove(filePath);
                    qDebug() << "Removed entity for OBJ file:" << filePath;
                }

            }
        }
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
        contextMenu.addAction("Set as Active Robot", this, SLOT(on_actionActiveRobot_triggered()));
        contextMenu.addAction("Save Robot", this, SLOT(on_actionSave_triggered()));
        contextMenu.addAction("New Robot", this, SLOT(on_actionNewRobot_triggered()));
        contextMenu.addAction("Open from Device...", this, SLOT(on_actionOpenFromDevice_triggered()));
        contextMenu.addAction("Delete Robot", this, SLOT(deleteCurrentRobot()));
    }
    else if (item->text() == RobotKeys::Joints)
    {
        contextMenu.addAction("Add New Joint", this, SLOT(addNewJoint()));
        contextMenu.addAction("Delete Last Joint", this, SLOT(deleteCurrentJoint()));
    }
    else if (item->text() == JointKeys::JointDynamics)
    {
        contextMenu.addAction("Add New Dynamics", this, SLOT(addNewDynamics()));
        contextMenu.addAction("Delete Last Dynamics", this, SLOT(deleteCurrentPayload()));
    }
    else if (item->text() == JointKeys::Visualization)
    {
        contextMenu.addAction("Select OBJ File", this, SLOT(on_actionJointVisualization_triggered()));
    }

    contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
}

// Creating a generic function that will load the Template data as an object and we will use this object throughout the application.
// Setting Global Template Object
void MainWindow::loadTemplate()
{

    QFile file(":/Resources/Json/Template.json");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Template file is missing. Please add the template file.");
        templateObject = QJsonObject(); // Initialize an empty JSON object to avoid application crashing.
        return;
    }

    QByteArray templateJson = file.readAll();
    file.close();

    QJsonDocument templateDocument = QJsonDocument::fromJson(templateJson);
    templateObject = templateDocument.object();

    if (templateObject.isEmpty())
    {
        qWarning() << "JSON Template object is empty";
        return;
    }

    if (!templateObject.contains(RobotKeys::Robot) || !templateObject[RobotKeys::Robot].isObject())
    {
        qWarning() << "Invalid JSON: Missing or invalid 'Robot' Template object";
        return;
    }
}

// I tired my best not to define the structue of the application in the model and to load the structure from the template file, to make it Dynamic.
// But I am unable to do so, because JSON Object is unordered list of key value pairs and for us Order is very important.
// So for now I am defining the Structure of the application in this function using TreeView Standard Model.
// I don't like this approach but I also tried ten other methods :)

void MainWindow::populateTreeView(const QJsonObject &json)
{

    if (!json.contains(RobotKeys::Robot) || !json[RobotKeys::Robot].isObject())
    {
        qWarning() << "Invalid JSON: Missing or invalid 'Robot' object";
        return;
    }

    QStandardItem *rootItem = model->invisibleRootItem();
    QJsonObject robot = json[RobotKeys::Robot].toObject();
    // qDebug() << robot;
    QStandardItem *robotItem = new QStandardItem(QIcon(":/Resources/Icons/robotic-arm.png"), RobotKeys::Robot);
    robotItem->setFlags(robotItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable
    
    if (!activeRobotItem)
    {
        QFont font = robotItem->font();
        font.setBold(true); // Set the font to bold for the first robot
        robotItem->setFont(font);
    }

    // Create a non-editable item for the second column
    QStandardItem *nonEditableItem = new QStandardItem();
    nonEditableItem->setFlags(nonEditableItem->flags() & ~Qt::ItemIsEditable);
    rootItem->appendRow(QList<QStandardItem *>() << robotItem << nonEditableItem);

    // Loading robot properties
    addItem(robotItem, RobotKeys::RobotName, robot[RobotKeys::RobotName].toString());
    addItem(robotItem, RobotKeys::RobotManufacturer, robot[RobotKeys::RobotManufacturer].toString());
    addItem(robotItem, RobotKeys::RobotPayload, robot[RobotKeys::RobotPayload].toString());
    addItem(robotItem, RobotKeys::RobotFootprint, robot[RobotKeys::RobotFootprint].toString());
    addItem(robotItem, RobotKeys::RobotMaxReach, robot[RobotKeys::RobotMaxReach].toString());
    addItem(robotItem, RobotKeys::RobotRepeatability, robot[RobotKeys::RobotRepeatability].toString());
    addItem(robotItem, RobotKeys::RobotWeight, robot[RobotKeys::RobotWeight].toString());
    addItem(robotItem, RobotKeys::DOF, robot[RobotKeys::DOF].toString());

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

    // Create a new non-editable item for the second column of the Joints item
    QStandardItem *jointsNonEditableItem = new QStandardItem();
    jointsNonEditableItem->setFlags(jointsNonEditableItem->flags() & ~Qt::ItemIsEditable);
    robotItem->appendRow(QList<QStandardItem *>() << jointsItem << jointsNonEditableItem);

    foreach (const QString &jointKey, joints.keys())
    {
        QJsonObject joint = joints[jointKey].toObject();
        addJoint(jointsItem, jointKey, joint);
    }

    // Present the data in the view
    ui->treeView->setModel(model);
   
    // Set the first robot as the active robot
    if (!activeRobotItem)
    {
        activeRobotItem = robotItem;
        ui->treeView->expandAll();
    }
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
}

// This function will add the joint in the TreeView.
void MainWindow::addJoint(QStandardItem *jointsItem, const QString &jointKey, const QJsonObject &joint)
{

    int jointNumber = jointsItem->rowCount() + 1;
    QStandardItem *singleJointItem = new QStandardItem(QString("Joint %1").arg(jointNumber));
    singleJointItem->setFlags(singleJointItem->flags() & ~Qt::ItemIsEditable);
    jointsItem->appendRow(singleJointItem);

    addItem(singleJointItem, JointKeys::JointName, joint[JointKeys::JointName].toString());
    addItem(singleJointItem, JointKeys::MotionRangeMax, joint[JointKeys::MotionRangeMax].toString());
    addItem(singleJointItem, JointKeys::MotionRangeMin, joint[JointKeys::MotionRangeMin].toString());
    addItem(singleJointItem, JointKeys::JointSpeedLimit, joint[JointKeys::JointSpeedLimit].toString());
    addItem(singleJointItem, JointKeys::FrictionCoefficient, joint[JointKeys::FrictionCoefficient].toString());
    addItem(singleJointItem, JointKeys::StiffnessCoefficient, joint[JointKeys::StiffnessCoefficient].toString());
    addItem(singleJointItem, JointKeys::DampingCoefficient, joint[JointKeys::DampingCoefficient].toString());

    // Loading kinematics
    QJsonObject kinematics = joint[JointKeys::JointKinematics].toObject();
    QStandardItem *kinematicsItem = new QStandardItem(JointKeys::JointKinematics);
    kinematicsItem->setFlags(kinematicsItem->flags() & ~Qt::ItemIsEditable);
       
    // Create a non-editable item for the second column
    QStandardItem *kinematicsNonEditableItem = new QStandardItem();
    kinematicsNonEditableItem->setFlags(kinematicsNonEditableItem->flags() & ~Qt::ItemIsEditable);
    singleJointItem->appendRow(QList<QStandardItem *>() << kinematicsItem << kinematicsNonEditableItem);


    if (kinematics.contains(KinematicsKeys::DhParameters) && kinematics[KinematicsKeys::DhParameters].isObject())
    {

        QJsonObject dhParameters = kinematics[KinematicsKeys::DhParameters].toObject();
        QStandardItem *dhParametersItem = new QStandardItem(KinematicsKeys::DhParameters);
        dhParametersItem->setFlags(dhParametersItem->flags() & ~Qt::ItemIsEditable);
        kinematicsItem->appendRow(dhParametersItem);
        addItem(dhParametersItem, DhParametersKeys::Alpha, dhParameters[DhParametersKeys::Alpha].toString());
        addItem(dhParametersItem, DhParametersKeys::D, dhParameters[DhParametersKeys::D].toString());
        addItem(dhParametersItem, DhParametersKeys::Theta, dhParameters[DhParametersKeys::Theta].toString());
        addItem(dhParametersItem, DhParametersKeys::A, dhParameters[DhParametersKeys::A].toString());
        addComboBoxItem(dhParametersItem, DhParametersKeys::DHType, dhParameters[DhParametersKeys::DHType].toString());
        //addItem(dhParametersItem, DhParametersKeys::DHType, dhParameters[DhParametersKeys::DHType].toString());
    }

    if (kinematics.contains(KinematicsKeys::RotationalValues) && kinematics[KinematicsKeys::RotationalValues].isObject())
    {
        QJsonObject rotationalValues = kinematics[KinematicsKeys::RotationalValues].toObject();
        QStandardItem *rotationalValuesItem = new QStandardItem(KinematicsKeys::RotationalValues);
        rotationalValuesItem->setFlags(rotationalValuesItem->flags() & ~Qt::ItemIsEditable);
        kinematicsItem->appendRow(rotationalValuesItem);
        addItem(rotationalValuesItem, RotationalValuesKeys::Ixx, rotationalValues[RotationalValuesKeys::Ixx].toString());
        addItem(rotationalValuesItem, RotationalValuesKeys::Ixy, rotationalValues[RotationalValuesKeys::Ixy].toString());
        addItem(rotationalValuesItem, RotationalValuesKeys::Ixz, rotationalValues[RotationalValuesKeys::Ixz].toString());
        addItem(rotationalValuesItem, RotationalValuesKeys::Iyy, rotationalValues[RotationalValuesKeys::Iyy].toString());
        addItem(rotationalValuesItem, RotationalValuesKeys::Iyz, rotationalValues[RotationalValuesKeys::Iyz].toString());
        addItem(rotationalValuesItem, RotationalValuesKeys::Izz, rotationalValues[RotationalValuesKeys::Izz].toString());
    }

    // Loading dynamics

    if (joint.contains(JointKeys::JointDynamics) && joint[JointKeys::JointDynamics].isObject())
    {

        QJsonObject dynamics = joint[JointKeys::JointDynamics].toObject();
        QStandardItem *dynamicsItem = new QStandardItem(QIcon(":/Resources/Icons/settings.png"), JointKeys::JointDynamics);
        dynamicsItem->setFlags(dynamicsItem->flags() & ~Qt::ItemIsEditable);
        
        // Create a new non-editable item for the second column of the Joints item
        QStandardItem *dynamicsNonEditableItem = new QStandardItem();
        dynamicsNonEditableItem->setFlags(dynamicsNonEditableItem->flags() & ~Qt::ItemIsEditable);
        singleJointItem->appendRow(QList<QStandardItem *>() << dynamicsItem << dynamicsNonEditableItem);

        foreach (const QString &payloadKey, dynamics.keys())
        {
            QJsonObject payload = dynamics[payloadKey].toObject();
            // Create generic function.
            addDynamicsPayload(dynamicsItem, payloadKey, payload);
        }
    }

    // Loading visualization
    if (joint.contains(JointKeys::Visualization) && joint[JointKeys::Visualization].isString())
    {
        QString visualizationPath = joint[JointKeys::Visualization].toString();
        QStandardItem *visualizationItem = new QStandardItem(QIcon(":/Resources/Icons/robot-dynamics.png"), JointKeys::Visualization);
        visualizationItem->setFlags(visualizationItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable
        QStandardItem *visualizationPathItem = new QStandardItem(visualizationPath);
        visualizationPathItem->setFlags(visualizationPathItem->flags() & ~Qt::ItemIsEditable);
        singleJointItem->appendRow(QList<QStandardItem *>() << visualizationItem << visualizationPathItem);

        if (!visualizationPath.isEmpty())
        {
            // Call the loadSingleObjFile function with the selected file path
            QJsonObject jsonObject;
            loadSingleObjFile(visualizationPath, jsonObject, rootEntity);
        }
    }
}

// This function will add the Payload in the TreeView.
void MainWindow::addDynamicsPayload(QStandardItem *dynamicsItem, const QString &payloadKey, const QJsonObject &payload)
{
    QStandardItem *payloadItem = new QStandardItem(payloadKey);
    payloadItem->setFlags(payloadItem->flags() & ~Qt::ItemIsEditable);
    dynamicsItem->appendRow(payloadItem);

    addItem(payloadItem, DynamicsKeys::TestPayload, payload[DynamicsKeys::TestPayload].toString());
    addItem(payloadItem, DynamicsKeys::PayloadPercentage, payload[DynamicsKeys::PayloadPercentage].toString());
    addItem(payloadItem, DynamicsKeys::RepeatabilityPercentage, payload[DynamicsKeys::RepeatabilityPercentage].toString());
    addItem(payloadItem, DynamicsKeys::SpeedPercentage, payload[DynamicsKeys::SpeedPercentage].toString());
    addItem(payloadItem, DynamicsKeys::BreakingDistance, payload[DynamicsKeys::BreakingDistance].toString());
    addItem(payloadItem, DynamicsKeys::BreakingTime, payload[DynamicsKeys::BreakingTime].toString());
}

// Using this function to handle each row of the TreeView
void MainWindow::addItem(QStandardItem *parent, const QString &key, const QVariant &value)
{
    QStandardItem *keyItem = new QStandardItem(key);
    keyItem->setEditable(false);
    QStandardItem *valueItem = new QStandardItem();
    valueItem->setEditable(true);
    valueItem->setData(value, Qt::DisplayRole);

    parent->appendRow(QList<QStandardItem *>() << keyItem << valueItem);
}

// This function is used to add the ComboBox Item in the TreeView.
void MainWindow::addComboBoxItem(QStandardItem *parent, const QString &key, const QString &value)
{
    QStandardItem *keyItem = new QStandardItem(key);
    keyItem->setEditable(false);

    // First Creating a QComboBox with the two options
    QComboBox *comboBox = new QComboBox();
    comboBox->addItem("Classic DH");
    comboBox->addItem("Modified DH");

    // Want to set default value to "Classic DH"
    int index = comboBox->findText(value);
    if (index != -1)
    {
        comboBox->setCurrentIndex(index);
    }
    else
    {
        comboBox->setCurrentIndex(0); // Default to "Classic DH"
    }

    // Now Create a QStandardItem to hold the QComboBox
    QStandardItem *comboBoxItem = new QStandardItem();
    comboBoxItem->setEditable(false);

    parent->appendRow(QList<QStandardItem *>() << keyItem << comboBoxItem);

    // Add the QComboBox to the tree view
    // ui->treeView->setIndexWidget(model->indexFromItem(comboBoxItem), comboBox);

    // Facing issue with loading the ComboBox in the TreeView, so for now i am not calling this function.
    QModelIndex indexFromItem = parent->model()->indexFromItem(comboBoxItem);
    qDebug() << "Adding QComboBox to index:" << indexFromItem;
    ui->treeView->setIndexWidget(indexFromItem, comboBox);
}

void MainWindow::addButtonItem(QStandardItem *parent, const QString &buttonText)
{
    QStandardItem *buttonItem = new QStandardItem(buttonText);
    buttonItem->setEditable(false);

    parent->appendRow(buttonItem);

    QPushButton *button = new QPushButton(buttonText);
    QModelIndex index = parent->model()->indexFromItem(buttonItem);
    ui->treeView->setIndexWidget(index, button);

    connect(button, &QPushButton::clicked, this, &MainWindow::onAddFilesButtonClicked);
}

void MainWindow::onAddFilesButtonClicked()
{
    QString objFilePath = QFileDialog::getOpenFileName(this, "Select OBJ File", "", "OBJ Files (*.obj)");
    if (!objFilePath.isEmpty())
    {
        QString mtlFilePath = QFileDialog::getOpenFileName(this, "Select MTL File", "", "MTL Files (*.mtl)");
        if (!mtlFilePath.isEmpty())
        {
            // Handle the selected OBJ and MTL file paths
            qDebug() << "Selected OBJ File:" << objFilePath;
            qDebug() << "Selected MTL File:" << mtlFilePath;

            // Update the model with the selected file paths
        }
    }
}

// Now trickies part starts with saving JSON in such a way that I can access it later on and able to load the data in TreeView.
// Not to make any mistake with structure, otherwise it will be surprice for you on loading data.

QJsonObject MainWindow::modelToJson(QStandardItem *robotItem)
{
    QJsonObject json;
    QJsonObject robotObject;
    QStandardItem *jointsItem = nullptr;

    if (robotItem)
    {
        for (int j = 0; j < robotItem->rowCount(); ++j)
        {
            QStandardItem *propertyItem = robotItem->child(j, 0);

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
                    QStandardItem *valueItem = robotItem->child(j, 1);
                    robotObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
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

                QJsonObject kinematicsObject;
                QJsonObject dynamicsObject;

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
                        else
                        {
                            singleJointObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
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
                                            //Need to pick DH Type from ComboBox
                                            if (propertyItem->text() == DhParametersKeys::DHType)
                                            {
                                                QModelIndex index = model->indexFromItem(valueItem);
                                                QComboBox *comboBox = qobject_cast<QComboBox *>(ui->treeView->indexWidget(index));
                                                if (comboBox)
                                                {
                                                    dhParametersObject[propertyItem->text()] = comboBox->currentText();
                                                }
                                            }
                                            else
                                            {
                                            // Call the generic function to convert the item to JSON
                                            dhParametersObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
                                            }
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
                                            // Call the generic function to convert the item to JSON
                                            rotationalValuesObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
                                        }
                                    }
                                    kinematicsObject[KinematicsKeys::RotationalValues] = rotationalValuesObject;
                                }
                            }
                            else
                            {
                                kinematicsObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
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
                                // Call the generic function to convert the item to JSON
                                payloadObject[propertyItem->text()] = valueItem ? QJsonValue::fromVariant(valueItem->data(Qt::DisplayRole)) : QJsonValue("");
                            }
                        }
                        dynamicsObject[payloadItem->text()] = payloadObject;
                    }
                    singleJointObject[JointKeys::JointDynamics] = dynamicsObject;
                }
                jointsObject[singleJointItem->text()] = singleJointObject;
            }
            robotObject[RobotKeys::Joints] = jointsObject;
        }
        json[RobotKeys::Robot] = robotObject;
    }

    // print json data in proper format.

    qDebug() << "JSON Data : " << json;

    return json;
}

void MainWindow::saveToJson(const QString &filePath, QStandardItem *currentItem)
{

    // Check if the file path and current item are valid

    if (!filePath.isEmpty() && currentItem)
    {
        // Get the current Robot JSON
        QJsonObject json = modelToJson(currentItem);

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

    // Set up the camera
    camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.159012, -504.859, -10.8127));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera control and setting the Zooming speed
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed(500.0f);
    camController->setLookSpeed(180.0f);
    camController->setCamera(camera);

    // Set up the scene

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.0f);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(0.159012, -504.859, -10.8127)); // Position light at the same height as the camera
    lightEntity->addComponent(lightTransform);

    // Create a directional light
    Qt3DCore::QEntity *directionalLightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QDirectionalLight *directionalLight = new Qt3DRender::QDirectionalLight(directionalLightEntity);
    directionalLight->setColor("white");
    directionalLight->setIntensity(1.0f);
    // directionalLight->setWorldDirection(QVector3D(0, 0, -1));
    directionalLightEntity->addComponent(directionalLight);

    Qt3DCore::QTransform *directionalLightTransform = new Qt3DCore::QTransform(directionalLightEntity);
    directionalLightTransform->setRotation(QQuaternion::fromEulerAngles(45, 45, 0)); // Will adjust as needed
    directionalLightEntity->addComponent(directionalLightTransform);

    // Create a container for the 3D view and add it to the UI
    QWidget *container = QWidget::createWindowContainer(view, this);
    ui->viewContainer->layout()->addWidget(container);
}

// Add the 3D Model to the Scene
// This function will load the 3D Model from the Resources and add it to the Scene.
void MainWindow::load3DModel()
{
    // Load the Mesh files from the Resources
    QString resourcesDir = ":/Resources/Models/Robot2";
    loadObjFiles(resourcesDir, rootEntity);

    // Set the root Entity
    view->setRootEntity(rootEntity);
}

// Remove the 3D Model from the Scene
// This function will remove the 3D Model from the Scene.
void MainWindow::remove3DModel()
{
    // Remove the root entity
    view->setRootEntity(nullptr);
}

// Load the Mesh files from the Resources
void MainWindow::loadObjFiles(const QString &directoryPath, Qt3DCore::QEntity *rootEntity)
{

    // Load JSON file containing transformation data

    QFile jsonFile(":/Resources/Json/dhParameters.json"); // Load JSON file here
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open JSON file.";
    }

    QByteArray jsonData = jsonFile.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(jsonData));
    QJsonObject jsonObject = doc.object();
    qDebug() << "JSON file loaded successfully with keys:" << jsonObject.keys(); // Debugging JSON keys

    // Create a QDir object for the specified directory
    QDir dir(directoryPath);
    if (!dir.exists())
    {
        qWarning() << "Directory does not exist: " << directoryPath;
        return;
    }

    // Set up the filter to find OBJ files
    dir.setNameFilters(QStringList() << "*.obj");
    dir.setFilter(QDir::Files);

    // Get the list of OBJ files
    QFileInfoList fileList = dir.entryInfoList();

    // Iterate through the list of OBJ files
    for (const QFileInfo &fileInfo : fileList)
    {
        const QString &filePath = fileInfo.absoluteFilePath();
        loadSingleObjFile(filePath, jsonObject, rootEntity);
    }
}

// This function will load the single OBJ file and add it to the Scene.
void MainWindow::loadSingleObjFile(const QString &filePath, const QJsonObject &jsonObject, Qt3DCore::QEntity *rootEntity)
{
    QString geometryName = QFileInfo(filePath).baseName(); // Get the base name without the file extension

    // Create an entity to hold the 3D model
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(rootEntity);

    // Load the OBJ file
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl::fromLocalFile(filePath)); // Ensure this loads the .obj file with .mtl references

    // Apply transformations (translation, rotation)
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();

    // Check if the JSON object contains transformation data for this geometry
    if (!jsonObject.isEmpty() && jsonObject.contains(geometryName))
    {
        QJsonObject geomData = jsonObject.value(geometryName).toObject();

        // Extract translation and rotation values
        float Tx = geomData.value("Tx").toDouble();
        float Tz = geomData.value("Tz").toDouble();
        float Rx = geomData.value("Rx").toDouble();
        float Rz = geomData.value("Rz").toDouble();

        // Set the translation and rotation values
        // transform->setTranslation(QVector3D(Tx, 0.0f, Tz));
        transform->setRotationX(Rx);
        transform->setRotationZ(Rz);
    }
    else
    {
        // Default transformations if no data in JSON
        transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
        transform->setRotationX(0.0f);
        transform->setRotationZ(0.0f);
    }

    transform->setScale(0.1f); // Example scale

    // Create a material to hold the properties from the MTL file
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();

    // Check for the existence of the corresponding MTL file
    QString mtlFilePath = QFileInfo(filePath).absolutePath() + "/" + geometryName + ".obj.mtl";
    QFile mtlFile(mtlFilePath);
    if (mtlFile.exists())
    {
        // Load the MTL file
        QColor ambientColor, diffuseColor, specularColor;
        float shininess = 0.0f, transparency = 0.0f;
        int illumModel = 2; // Default illumination model to Phong

        if (parseMtlFile(mtlFilePath, ambientColor, diffuseColor, specularColor, shininess, transparency, illumModel))
        {
            material->setAmbient(ambientColor);
            material->setDiffuse(diffuseColor);
            material->setSpecular(specularColor);
            material->setShininess(shininess);

            // Handle transparency by adjusting the alpha channel
            QColor diffuseWithAlpha = diffuseColor;
            diffuseWithAlpha.setAlphaF(1.0f - transparency); // Adjust alpha based on transparency
            material->setDiffuse(diffuseWithAlpha);
        }
        else
        {
            // Set a default material if parsing fails
            material->setAmbient(QColor::fromRgbF(0.2, 0.2, 0.2));
            material->setDiffuse(QColor::fromRgbF(0.498039, 0.498039, 0.498039));
            material->setSpecular(QColor::fromRgbF(1.0, 1.0, 1.0));
            material->setShininess(0.0);
        }
    }
    else
    {
        // Set a default material if MTL file does not exist
        material->setAmbient(QColor::fromRgbF(0.2, 0.2, 0.2));
        material->setDiffuse(QColor::fromRgbF(0.498039, 0.498039, 0.498039));
        material->setSpecular(QColor::fromRgbF(1.0, 1.0, 1.0));
        material->setShininess(0.0);
    }

    // Add the components (mesh, transformation, material) to the entity
    entity->addComponent(mesh);
    entity->addComponent(transform);
    entity->addComponent(material);

    // Store the entity in the map so that we can delete them later.
    entityMap[filePath] = entity;
}

// Sample function to parse the MTL file and return colors
bool MainWindow::parseMtlFile(const QString &mtlFilePath, QColor &ambient, QColor &diffuse, QColor &specular, float &shininess, float &transparency, int &illumModel)
{
    QFile mtlFile(mtlFilePath);
    if (!mtlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open MTL file:" << mtlFilePath;
        return false;
    }

    QTextStream in(&mtlFile);
    QString line;

    while (in.readLineInto(&line))
    {
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty())
            continue;

        if (tokens[0] == "Ka")
        { // Ambient color
            ambient = QColor::fromRgbF(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat());
        }
        else if (tokens[0] == "Kd")
        { // Diffuse color
            diffuse = QColor::fromRgbF(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat());
        }
        else if (tokens[0] == "Ks")
        { // Specular color
            specular = QColor::fromRgbF(tokens[1].toFloat(), tokens[2].toFloat(), tokens[3].toFloat());
        }
        else if (tokens[0] == "Ns")
        { // Shininess
            shininess = tokens[1].toFloat();
        }
        else if (tokens[0] == "Tr")
        { // Transparency
            transparency = tokens[1].toFloat();
        }
        else if (tokens[0] == "illum")
        { // Illumination model
            illumModel = tokens[1].toInt();
        }
    }

    return true; // Successfully parsed the MTL file
}

// Update the rotation and translation of the 3D model
void MainWindow::updateRotation()
{
    rotationAngle += 1.0f;
    float translationAmount = 0.01f;
    for (const auto &child : rootEntity->children())
    {
        auto entity = qobject_cast<Qt3DCore::QEntity *>(child);
        if (entity)
        {
            auto transform = entity->findChild<Qt3DCore::QTransform *>();
            if (transform)
            {
                // Rotate around the Z-axis for vertical rotation
                QQuaternion rotation = QQuaternion::fromEulerAngles(0.0f, 0.0f, rotationAngle);
                transform->setRotation(rotation);
                // Translate along the X-axis
                QVector3D currentTranslation = transform->translation();
                QVector3D newTranslation = currentTranslation + QVector3D(translationAmount, 0.0f, 0.0f); // Move along X
                transform->setTranslation(newTranslation);
            }
        }
    }
}
