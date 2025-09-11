#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "jsonkeys.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rotationAngle(0.0f),
      rotating(false),
      ui(new Ui::MainWindow),
      view(nullptr),
      rootEntity(nullptr)
{
    ui->setupUi(this);

    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);
    rotationTimer = new QTimer(this);
    connect(rotationTimer, &QTimer::timeout, this, &MainWindow::updateRotation);


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
        int prevId = getRobotId(activeRobotItem);
        remove3DModel();

        QFont font = activeRobotItem->font();
        font.setBold(false);
        activeRobotItem->setFont(font);
        ui->treeView->collapse(model->indexFromItem(activeRobotItem));
    }

    //  check that the index is still in the model
    if (!currentIndex.isValid() || currentIndex.row() >= model->rowCount(currentIndex.parent()))
        return;


    // Set the font of the selected robot to bold and expand its tree view
    QFont font = currentItem->font();
    font.setBold(true);
    currentItem->setFont(font);
    ui->treeView->expand(currentIndex);

    // Update the active robot itemA breakpoint instruction (__debugbreak() statement or a similar call) was executed in RobotEditor.exe.
    activeRobotItem = currentItem;

    // Show new robot model
    int robotId = getRobotId(currentItem);
    loadRobotObjFiles(robotId);

    // Load Model of this Robot
    //load3DModel();
}


void MainWindow::on_actionSave_triggered()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);

    saveRobot(currentItem);
}

void MainWindow::on_actionSaveAll_triggered()
{
    if (!model)
    {
        qWarning() << "Model does not exist";
        return;
    }

    QStandardItem* rootItem = model->invisibleRootItem();
    if (!rootItem || rootItem->rowCount() == 0)
        return;

    for (int i = 0; i < rootItem->rowCount(); ++i)
    {
        QStandardItem* robotItem = rootItem->child(i);

        // Ask for file path for each robot
        QString filePath = QFileDialog::getSaveFileName(
            this,
            "Save Robot Editor Package for " + robotItem->text(),
            QDir::homePath() + "/MyRobot",
            "Robot Editor Package (*.re)"
        );

        if (!filePath.isEmpty())
        {
            if (!filePath.endsWith(".re", Qt::CaseInsensitive))
                filePath += ".re";

            saveRobot(robotItem, filePath);
        }
        else
        {
            qWarning() << "Skipped saving robot:" << robotItem->text();
        }
    }
}


/*

void MainWindow::on_actionSave_triggered()
{

    // 1. Ask user for the .re file path (location + name) in one dialog
    QString reFilePath = QFileDialog::getSaveFileName(
        this,
        "Save Robot Editor Package",
        QDir::homePath() + "/MyRobot",
        "Robot Editor Package (*.re)"
    );
    if (reFilePath.isEmpty())
        return;

    // Ensure the file has the .re extension
    if (!reFilePath.endsWith(".re", Qt::CaseInsensitive))
        reFilePath += ".re";


    // 3. Save JSON and data to the temp folder using your existing function
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        qWarning() << "No item selected.";
        return;
    }
    QStandardItem* currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem || currentItem->text() != RobotKeys::Robot)
    {
        qWarning() << "Invalid or non-robot item selected.";
        return;
    }

   

    const QString srcRobotDataDir = currentItem->data(Qt::UserRole + 2).toString();

   
    if (reFilePath.isEmpty())
        return;


    QString robotDataFilePath = srcRobotDataDir + '/' + "RobotData.json";

    // This function will create JSON File of RobotData in the temp folder.
    saveToJson(robotDataFilePath, currentItem);

    // 4. Zip the temp folder using RobotLib (returns .re file path as std::string)
   // std::string tempReFileStd = robotLib.zipRobotPackage(tempFolder.toStdString());
    std::string tempReFileStd = robotLib.zipRobotPackage(srcRobotDataDir.toStdString());
    QString tempReFile = QString::fromStdString(tempReFileStd);

    // 5. Move the .re file to the user-selected location
    if (QFile::exists(reFilePath)) {
        QFile::remove(reFilePath);
    }
    bool moved = QFile::rename(tempReFile, reFilePath);

    // 6. Clean up the temp folder
    QDir tempDir(srcRobotDataDir);
    tempDir.removeRecursively(); 

    // 7. Show result
    if (moved && QFile::exists(reFilePath))
        QMessageBox::information(this, "Success", "Robot Editor package saved as: " + reFilePath);
    else
        QMessageBox::warning(this, "Error", "Failed to create Robot Editor package.");

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

*/

void MainWindow::on_actionNewRobot_triggered()
{

    try {

   
    // Create new Robot.
    Robot newRobot = robotLib.initializeNewRobot();

    if (newRobot.getId()) {

        QString robotIdStr = QString::number(newRobot.getId());


    // 2. Create a temporary folder. I am making this temp folder because when we save the .re file , we will use this folder and also when processing multiple robots.
    QString tempFolder = QDir::temp().absoluteFilePath(robotIdStr + "_re_temp");
    QDir tempDir(tempFolder);
    tempDir.removeRecursively(); // Clean up if exists
    QDir().mkpath(tempFolder);

    newRobot.setRobotDataDir(tempFolder.toStdString());
    robotDataDirs[newRobot.getId()] = tempFolder;


    populateTreeView(newRobot);
    }

    }
    catch (const std::runtime_error& e) {
        qWarning() << "Failed to Create new robot :" << e.what();
    }


}

void MainWindow::on_actionOpenFromDevice_triggered()
{

    // 1. Let user select a .re file
    QString reFilePath = QFileDialog::getOpenFileName(this, "Open Robot Editor Package", QDir::homePath(), "Robot Editor Package (*.re)");
    if (reFilePath.isEmpty())
        return;

    // 2. Extract .re (zip) to a temp folder using RobotLib
    QString tempFolder = QDir::temp().absoluteFilePath(QFileInfo(reFilePath).baseName() + "_re_open_temp");
    QDir tempDir(tempFolder);
    tempDir.removeRecursively(); // Clean up if exists
    QDir().mkpath(tempFolder);

    // Call RobotLib to extract the .re file
    bool extracted = robotLib.extractRobotPackage(reFilePath.toStdString(), tempFolder.toStdString());
    if (!extracted) {
        QMessageBox::warning(this, "Error", "Failed to extract Robot Editor package.");
        return;
    }

    // 3. Load RobotData.json
    QString robotDataFilePath = tempFolder + "/RobotData.json";
    if (!QFile::exists(robotDataFilePath)) {
        QMessageBox::warning(this, "Error", "RobotData.json not found in package.");
        return;
    }

    try
    {
        // Load the robot from the extracted JSON file
        Robot robot = robotLib.loadFromFile(robotDataFilePath.toStdString());

        // Set the RobotDataDir so OBJ/MTL files can be found
        robot.setRobotDataDir(tempFolder.toStdString());

        // Populate data in the model
        populateTreeView(robot);

        // 4. Load all OBJ files in the extracted folder
        loadObjFiles(tempFolder);
    }
    catch (const std::runtime_error& e)
    {
        qWarning() << "Failed to load robot data from JSON file: " << e.what();
        QMessageBox::warning(this, "Error", "Failed to load robot data from JSON file.");
        return;
    }

}

void MainWindow::on_actionImportFromVisualComponent_triggered()
{
    // Open the VCMX file and extract the data to create a new Robot.

    
    QString initialDir = QDir::homePath();


    QString filePath = QFileDialog::getOpenFileName(this, "Open VCMX", initialDir, "VCMX Files (*.vcmx)");
    if (!filePath.isEmpty())
    {
        QFileInfo fileInfo(filePath);
        QString filePath = fileInfo.absoluteFilePath();

		try
		{
            
			// Run the data extractor
			Robot newRobot = robotLib.importRobotFromVCMX(filePath.toStdString());
			
			if (newRobot.getId()) {

				QString baseName = QFileInfo(filePath).completeBaseName();
				QString parentDir = QFileInfo(filePath).absolutePath();
				QString outputDir = parentDir + "/" + baseName;
				QString robotDataDir = outputDir + "/RobotData";



				// 2. Create a temporary folder. I am making this temp folder because when we save the .re file , we will use this folder and also when processing multiple robots.
                QString tempFolder = QDir::temp().absoluteFilePath(QFileInfo(baseName).baseName() + "_re_temp");
                QDir tempDir(tempFolder);
                tempDir.removeRecursively(); // Clean up if exists
                QDir().mkpath(tempFolder);




                qDebug() << "Temp folder path:" << tempFolder;

                if (!robotDataDir.isEmpty() && QDir(robotDataDir).exists()) {

                    if (!copyDirectoryRecursively(robotDataDir, tempFolder)) {
                        qWarning() << "Failed to copy RobotData from" << robotDataDir << "to" << tempFolder;
                    }
                }


                // Copy all OBJ/MTL files to robotDataDir
                // (Assume robotLib provides a way to get the list of OBJ files, or scan the extracted folder)
                QStringList objFiles;
                QDir srcDir(parentDir + "/" + baseName + "/RobotData");
                for (const QFileInfo& fi : srcDir.entryInfoList(QStringList() << "*.obj", QDir::Files)) {
                    QString dest = robotDataDir + "/" + fi.fileName();
                    objFiles << dest;
                 
                }
                newRobot.setRobotDataDir(tempFolder.toStdString());
                robotObjFiles[newRobot.getId()] = objFiles;
                robotDataDirs[newRobot.getId()] = tempFolder;

				// Remove RobotDataDir
                QDir robotDir(robotDataDir);
                robotDir.removeRecursively(); // Clean up if exists


				if (QDir(tempFolder).exists()) {
					loadObjFiles(tempFolder);
				}

			}

            populateTreeView(newRobot);


		}
        catch (const std::runtime_error &e)
        {
            qWarning() << "Failed to load robot data from VCMX file: " << e.what();
            return;
        }

    }
}

// this function is temporary function for testing
void MainWindow::on_actionImportFromVisualComponentFolder_triggered()
{
    QString initialDir = QDir::homePath();

    QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder", initialDir);
 
    if (!folderPath.isEmpty())
    {


        try
        {
 

            // Run the data extractor
            Robot newRobot = robotLib.importRobotFromVCMX(folderPath.toStdString());


            if (newRobot.getId()) {

                QFileInfo dirInfo(folderPath);

                QString baseName = dirInfo.baseName();      // Folder name only
                
                QString robotDataDir = folderPath + "/RobotData";


                // 2. Create a temporary folder. I am making this temp folder because when we save the .re file , 
                // we will use this folder and also when processing multiple robots.
                QString tempFolder = QDir::temp().absoluteFilePath(QFileInfo(baseName).baseName() + "_re_temp");
                QDir tempDir(tempFolder);
                tempDir.removeRecursively(); // Clean up if exists
                QDir().mkpath(tempFolder);




                qDebug() << "Temp folder path:" << tempFolder;

                if (!robotDataDir.isEmpty() && QDir(robotDataDir).exists()) {

                    if (!copyDirectoryRecursively(robotDataDir, tempFolder)) {
                        qWarning() << "Failed to copy RobotData from" << robotDataDir << "to" << tempFolder;
                    }
                }


                // Copy all OBJ/MTL files to robotDataDir
                // (Assume robotLib provides a way to get the list of OBJ files, or scan the extracted folder)
                QStringList objFiles;
                QDir srcDir(robotDataDir);
                for (const QFileInfo& fi : srcDir.entryInfoList(QStringList() << "*.obj", QDir::Files)) {
                    QString dest = robotDataDir + "/" + fi.fileName();
                    objFiles << dest;

                }
                newRobot.setRobotDataDir(tempFolder.toStdString());
                robotObjFiles[newRobot.getId()] = objFiles;
                robotDataDirs[newRobot.getId()] = tempFolder;

                // Remove RobotDataDir
                QDir robotDir(robotDataDir);
                robotDir.removeRecursively(); // Clean up if exists


                if (QDir(tempFolder).exists()) {
                    loadObjFiles(tempFolder);
                }

            }

            populateTreeView(newRobot);

            //

        }
        catch (const std::runtime_error& e)
        {
            qWarning() << "Failed to load robot data from VCMX file: " << e.what();
            return;
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


    QStandardItem* parentRobotItem = getParentRobotItem(currentItem);
    if (!parentRobotItem) return;
    int robotId = getRobotId(parentRobotItem);
    QString robotDataDir = getRobotDataDir(parentRobotItem);
    if (robotId < 0 || robotDataDir.isEmpty()) return;


    QString filePath = QFileDialog::getOpenFileName(this, "Select OBJ File", "", "OBJ Files (*.obj)");

    if (filePath.isEmpty()) return;

    // Copy OBJ/MTL to robotDataDir
    QDir().mkpath(robotDataDir);
    QString destObj = robotDataDir + "/" + QFileInfo(filePath).fileName();
    QFile::copy(filePath, destObj);
    QString mtlSrc = QFileInfo(filePath).absolutePath() + "/" + QFileInfo(filePath).baseName() + ".mtl";
    QString mtlDest = robotDataDir + "/" + QFileInfo(filePath).baseName() + ".mtl";
    if (QFile::exists(mtlSrc)) QFile::copy(mtlSrc, mtlDest);

    robotObjFiles[robotId] << destObj;

   if (parentRobotItem == activeRobotItem)
   {

        QStandardItem* meshNameItem = new QStandardItem(destObj);
        meshNameItem->setFlags(meshNameItem->flags() & ~Qt::ItemIsEditable);
        QStandardItem* meshPathItem = new QStandardItem(destObj);
        meshPathItem->setFlags(meshPathItem->flags() & ~Qt::ItemIsEditable);
        currentItem->appendRow(QList<QStandardItem*>() << meshNameItem << meshPathItem);


        // Call the loadSingleObjFile function with the selected file path
        QJsonObject jsonObject;
        jsonObject["filePath"] = destObj;
        loadSingleObjFile(jsonObject);

            
        ui->treeView->expand(model->indexFromItem(currentItem));
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

    // Remove the existing 3D model
    remove3DModel();

    // Remove all rows from the root item
    rootItem->removeRows(0, rootItem->rowCount());
    qDebug() << "Deleted all robots.";
}

/****************** Add Section ******************/

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

    //int robotId = currentItem->data(Qt::UserRole + 1).toInt();
    //Robot &robot = robotLib.getRobotById(robotId);
    //Joint newJoint = robot.addJoint();
    // call createJoint function from robotlib
    Joint newJoint = robotLib.createJoint();

    addJoint(currentItem, newJoint);

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

    // Create a new dynamics
    JointDynamics newDynamics = robotLib.createDynamics();
    //JointDynamics newDynamics;
    addDynamicsPayload(currentItem, newDynamics);
    ui->treeView->expand(currentIndex);
}



void MainWindow::addJoint(QStandardItem *jointsItem, const Joint &joint)
{

    //QStandardItem *singleJointItem = new QStandardItem(QString::fromStdString(joint.getJointNumber()));

    int jointNumber = jointsItem->rowCount() + 1;
    QStandardItem *singleJointItem = new QStandardItem(QString("Joint %1").arg(jointNumber));
    singleJointItem->setFlags(singleJointItem->flags() & ~Qt::ItemIsEditable);
    jointsItem->appendRow(singleJointItem);

    addItem(singleJointItem, JointKeys::JointName, QString::fromStdString(joint.getName()));
    addItem(singleJointItem, JointKeys::MotionRangeMax, QString::number(joint.getMotionRangeMax()));
    addItem(singleJointItem, JointKeys::MotionRangeMin, QString::number(joint.getMotionRangeMin()));
    addItem(singleJointItem, JointKeys::JointSpeedLimit, QString::number(joint.getJointSpeedLimit()));
    addItem(singleJointItem, JointKeys::FrictionCoefficient, QString::number(joint.getFrictionCoefficient()));
    addItem(singleJointItem, JointKeys::StiffnessCoefficient, QString::number(joint.getStiffnessCoefficient()));
    addItem(singleJointItem, JointKeys::DampingCoefficient, QString::number(joint.getDampingCoefficient()));

    // Loading kinematics
    const auto &kinematics = joint.getKinematics();
    QStandardItem *kinematicsItem = new QStandardItem(JointKeys::JointKinematics);
    kinematicsItem->setFlags(kinematicsItem->flags() & ~Qt::ItemIsEditable);

    // Create a non-editable item for the second column
    QStandardItem *kinematicsNonEditableItem = new QStandardItem();
    kinematicsNonEditableItem->setFlags(kinematicsNonEditableItem->flags() & ~Qt::ItemIsEditable);
    singleJointItem->appendRow(QList<QStandardItem *>() << kinematicsItem << kinematicsNonEditableItem);

    const auto &dhParameters = kinematics.getDhParameters();
    QStandardItem *dhParametersItem = new QStandardItem(KinematicsKeys::DhParameters);
    dhParametersItem->setFlags(dhParametersItem->flags() & ~Qt::ItemIsEditable);
    kinematicsItem->appendRow(dhParametersItem);
    addItem(dhParametersItem, DhParametersKeys::Alpha, QString::number(dhParameters.getAlpha()));
    addItem(dhParametersItem, DhParametersKeys::D, QString::number(dhParameters.getD()));
    addItem(dhParametersItem, DhParametersKeys::Theta, QString::number(dhParameters.getTheta()));
    addItem(dhParametersItem, DhParametersKeys::A, QString::number(dhParameters.getA()));
    addComboBoxItem(dhParametersItem, DhParametersKeys::DHType, QString::fromStdString(kinematics.getDhType()));

    const auto &rotationalValues = kinematics.getRotationalValues();
    QStandardItem *rotationalValuesItem = new QStandardItem(KinematicsKeys::RotationalValues);
    rotationalValuesItem->setFlags(rotationalValuesItem->flags() & ~Qt::ItemIsEditable);
    kinematicsItem->appendRow(rotationalValuesItem);
    addItem(rotationalValuesItem, RotationalValuesKeys::Ixx, QString::number(rotationalValues.getIxx()));
    addItem(rotationalValuesItem, RotationalValuesKeys::Ixy, QString::number(rotationalValues.getIxy()));
    addItem(rotationalValuesItem, RotationalValuesKeys::Ixz, QString::number(rotationalValues.getIxz()));
    addItem(rotationalValuesItem, RotationalValuesKeys::Iyy, QString::number(rotationalValues.getIyy()));
    addItem(rotationalValuesItem, RotationalValuesKeys::Iyz, QString::number(rotationalValues.getIyz()));
    addItem(rotationalValuesItem, RotationalValuesKeys::Izz, QString::number(rotationalValues.getIzz()));

    // Loading dynamics
    QStandardItem *dynamicsItem = new QStandardItem(QIcon(":/Resources/Icons/settings.png"), JointKeys::JointDynamics);
    dynamicsItem->setFlags(dynamicsItem->flags() & ~Qt::ItemIsEditable);

    // Create a new non-editable item for the second column of the Joints item
    QStandardItem *dynamicsNonEditableItem = new QStandardItem();
    dynamicsNonEditableItem->setFlags(dynamicsNonEditableItem->flags() & ~Qt::ItemIsEditable);
    singleJointItem->appendRow(QList<QStandardItem *>() << dynamicsItem << dynamicsNonEditableItem);


     // Check if joint has dynamics
    if (joint.getDynamics().empty())
    {
        // Create a new dynamics if there are no existing dynamics
        JointDynamics newDynamics = robotLib.createDynamics();
        addDynamicsPayload(dynamicsItem, newDynamics);
    }
    else
    {
        for (const auto &dynamics : joint.getDynamics())
        {
            addDynamicsPayload(dynamicsItem, dynamics);
        }
    }


    // Loading visualization
    QStandardItem* visualizationItem = new QStandardItem(QIcon(":/Resources/Icons/robot-dynamics.png"), JointKeys::Visualization);
    visualizationItem->setFlags(visualizationItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable

    // Create a non-editable item for the second column
    QStandardItem* visualizationNonEditableItem = new QStandardItem();
    visualizationNonEditableItem->setFlags(visualizationNonEditableItem->flags() & ~Qt::ItemIsEditable);
    singleJointItem->appendRow(QList<QStandardItem*>() << visualizationItem << visualizationNonEditableItem);

    
    const auto& visualizations = joint.getVisualizations();
    for (const auto& vis : visualizations) {
        QStandardItem* meshNameItem = new QStandardItem(QString::fromStdString(vis.first));
        meshNameItem->setFlags(meshNameItem->flags() & ~Qt::ItemIsEditable);
        QStandardItem* meshPathItem = new QStandardItem(QString::fromStdString(vis.second));
        meshPathItem->setFlags(meshPathItem->flags() & ~Qt::ItemIsEditable);
        visualizationItem->appendRow(QList<QStandardItem*>() << meshNameItem << meshPathItem);

        
        QJsonObject jsonObject;
        jsonObject["filePath"] = QString::fromStdString(vis.second);
        loadSingleObjFile(jsonObject);
    }

}


// This function will add the Payload in the TreeView.
void MainWindow::addDynamicsPayload(QStandardItem *dynamicsItem, const JointDynamics &dynamics)
{
    //QStandardItem *payloadItem = new QStandardItem(QString::fromStdString(dynamics.getPayloadNumber()));

    int payloadNumber = dynamicsItem->rowCount() + 1;
    QStandardItem *payloadItem = new QStandardItem(QString("Payload %1").arg(payloadNumber));
    payloadItem->setFlags(payloadItem->flags() & ~Qt::ItemIsEditable);
    dynamicsItem->appendRow(payloadItem);

    addItem(payloadItem, DynamicsKeys::TestPayload, QString::number(dynamics.getTestPayload()));
    addItem(payloadItem, DynamicsKeys::PayloadPercentage, QString::number(dynamics.getPayloadPercentage()));
    addItem(payloadItem, DynamicsKeys::RepeatabilityPercentage, QString::number(dynamics.getReachabilityPercentage()));
    addItem(payloadItem, DynamicsKeys::SpeedPercentage, QString::number(dynamics.getSpeedPercentage()));
    addItem(payloadItem, DynamicsKeys::BreakingDistance, QString::number(dynamics.getBreakingDistance()));
    addItem(payloadItem, DynamicsKeys::BreakingTime, QString::number(dynamics.getBreakingTime()));
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

/****************** Delete Section ******************/

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

    int robotId = getRobotId(currentItem);
    if (currentItem == activeRobotItem)
    {
        activeRobotItem = nullptr;
        // Remove the existing 3D model
        remove3DModel();
    }

    // Remove the selected robot item
    removeRobotObjFiles(robotId);
    model->removeRow(currentItem->row(), currentItem->parent() ? currentItem->parent()->index() : QModelIndex());
    
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

/****************** Show / Load Section ******************/

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



void MainWindow::populateTreeView(const Robot &robot)
{

    QStandardItem *rootItem = model->invisibleRootItem();

    QStandardItem *robotItem = new QStandardItem(QIcon(":/Resources/Icons/robotic-arm.png"), RobotKeys::Robot);
    robotItem->setFlags(robotItem->flags() & ~Qt::ItemIsEditable); // Make the item non-editable

    // Store the robot ID in the item
    robotItem->setData(robot.getId(), Qt::UserRole + 1);
    robotItem->setData(QString::fromStdString(robot.getRobotDataDir()), Qt::UserRole + 2);

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
    addItem(robotItem, RobotKeys::RobotName, QString::fromStdString(robot.getName()));
    addItem(robotItem, RobotKeys::RobotManufacturer, QString::fromStdString(robot.getManufacturer()));
    addItem(robotItem, RobotKeys::RobotPayload, QString::number(robot.getPayload()));
    addItem(robotItem, RobotKeys::RobotFootprint, QString::number(robot.getFootprint()));
    addItem(robotItem, RobotKeys::RobotMaxReach, QString::number(robot.getMaxReach()));
    addItem(robotItem, RobotKeys::RobotRepeatability, QString::number(robot.getRepeatability()));
    addItem(robotItem, RobotKeys::RobotWeight, QString::number(robot.getWeight()));
    addItem(robotItem, RobotKeys::DOF, QString::number(robot.getDof()));

    // Loading joints
    QStandardItem *jointsItem = new QStandardItem(QIcon(":/Resources/Icons/robot-joint.png"), RobotKeys::Joints);
    jointsItem->setFlags(jointsItem->flags() & ~Qt::ItemIsEditable);
    // setting Robot ID to the Joints Item
    jointsItem->setData(robot.getId(), Qt::UserRole + 1);

    // Create a new non-editable item for the second column of the Joints item
    QStandardItem *jointsNonEditableItem = new QStandardItem();
    jointsNonEditableItem->setFlags(jointsNonEditableItem->flags() & ~Qt::ItemIsEditable);
    robotItem->appendRow(QList<QStandardItem *>() << jointsItem << jointsNonEditableItem);

    // Get the joints from the robot and pass them to the addJoint function
    const std::vector<Joint> &joints = robot.getJoints();
    for (const auto &joint : joints)
    {
        addJoint(jointsItem, joint);
    }
  

    // Set the first robot as the active robot
    if (!activeRobotItem)
    {
        activeRobotItem = robotItem;
        ui->treeView->expandAll();
    }

    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
}



/****************** JSON Related Function Implementation ******************/

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
                                            // Need to pick DH Type from ComboBox
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
        QJsonObject qJson = modelToJson(currentItem);

        // Before saving to file, make sure the JSON object has valid Json data
        if (qJson.isEmpty())
        {
            qWarning() << "Failed to convert model to JSON";
            return;
        }
        // verify that the existing json has valid foramtted data.



         // Convert QString to std::string
        std::string stdFilePath = filePath.toStdString();

        // Convert QJsonObject to nlohmann::json
        nlohmann::json json = nlohmann::json::parse(QString(QJsonDocument(qJson).toJson(QJsonDocument::Compact)).toStdString());
         
         // Pass the JSON data to RobotLib to update the robot values
         if (!robotLib.updateAndSaveRobotData(stdFilePath, json))
         {
             qWarning() << "Failed to update robot data in RobotLib";

             return;
         }

    }
}

/****************** 3D Related Function Implementation ******************/

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
    camera->setViewCenter(QVector3D(0, -2.0f, 0));
    camera->setUpVector(QVector3D(0, 0, 1));         // Z is up

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
    if (!activeRobotItem)
    {
        qWarning() << "No active robot item to load 3D model.";
        return;
    }

    // Get the file paths from joint visualizations
    QStringList filePaths = collectVisualizationPaths(activeRobotItem);

    // Remove the existing 3D model
    remove3DModel();

    // Load the collected OBJ files
    for (const QString &filePath : filePaths)
    {
        QJsonObject jsonObject;
        jsonObject["filePath"] = filePath; 
        loadSingleObjFile(jsonObject);
    }

    // Set the root Entity
    view->setRootEntity(rootEntity);
}

// Remove the 3D Model from the Scene
// This function will remove the 3D Model from the Scene.
void MainWindow::remove3DModel()
{
    if (!rootEntity)
        return;
    // Remove the root entity
    for (auto entity : rootEntity->findChildren<Qt3DCore::QEntity *>(QString("ModelEntity"), Qt::FindDirectChildrenOnly))
    {
        delete entity;
    }
    entityMap.clear();
}

// Load the Mesh files from the Resources
void MainWindow::loadObjFiles(const QString &directoryPath)
{
    QJsonObject jsonObject;

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
        jsonObject["filePath"] = filePath; // Set the file path in the JSON object
        loadSingleObjFile(jsonObject);
    }
}

// This function will load the single OBJ file and add it to the Scene.
void MainWindow::loadSingleObjFile(const QJsonObject &jsonObject)
{
    QString filePath = jsonObject["filePath"].toString();
    if (filePath.isEmpty()) {
        qWarning() << "File path is empty. Cannot load OBJ file.";
        return;
    }

    QString geometryName = QFileInfo(filePath).baseName(); // Get the base name without the file extension

    // Create an entity to hold the 3D model
    Qt3DCore::QEntity *entity = new Qt3DCore::QEntity(rootEntity);

    // Load the OBJ file
    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl::fromLocalFile(filePath)); // Ensure this loads the .obj file with .mtl references

    // Apply transformations (translation, rotation)
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform();

    // Check if the JSON object contains transformation data for this geometry
    /*
    if (jsonObject.contains("translation") && jsonObject["translation"].isArray()) {
        QJsonArray translationArray = jsonObject["translation"].toArray();
        if (translationArray.size() == 3) {
            float Tx = translationArray[0].toDouble();
            float Ty = translationArray[1].toDouble();
            float Tz = translationArray[2].toDouble();
            transform->setTranslation(QVector3D(Tx, Ty, Tz));

            // print translation values for debugging
            qDebug() << "Translation values:" << Tx << Ty << Tz;
        } else {
            qWarning() << "Invalid translation array size. Using default translation.";
            transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
        }
    } else {
        qWarning() << "Translation not found in JSON. Using default translation.";
        transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));
    }

    // Extract rotation values from the JSON object
    if (jsonObject.contains("rotation") && jsonObject["rotation"].isArray()) {
        QJsonArray rotationArray = jsonObject["rotation"].toArray();
        if (rotationArray.size() == 3) {
            float Rx = rotationArray[0].toDouble();
            float Ry = rotationArray[1].toDouble();
            float Rz = rotationArray[2].toDouble();
            QQuaternion rotation = QQuaternion::fromEulerAngles(Rx, Ry, Rz);
            transform->setRotation(rotation);

            // print rotation values for debugging
            qDebug() << "Rotation values:" << Rx << Ry << Rz;
        } else {
            qWarning() << "Invalid rotation array size. Using default rotation.";
            transform->setRotation(QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f));
        }
    } 
    else
    {
        qWarning() << "Rotation not found in JSON. Using default rotation.";
        transform->setRotation(QQuaternion::fromEulerAngles(0.0f, 0.0f, 0.0f));
    }
    */

    transform->setScale(0.1f); // Example scale

    // Create a material to hold the properties from the MTL file
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial();

    // Check for the existence of the corresponding MTL file
    QString mtlFilePath = QFileInfo(filePath).absolutePath() + "/" + geometryName + ".mtl";
    QFile mtlFile(mtlFilePath);
    if (mtlFile.exists())
    {
        // Load the MTL file
        QColor ambientColor, diffuseColor, specularColor;
        float shininess = 0.0f, transparency = 0.0f;
        int illumModel = 2; // Default illumination model to Phong

        if (parseMtlFile(mtlFilePath, ambientColor, diffuseColor, specularColor, shininess, transparency, illumModel))
        {

            // Check if any diffuse color channel is less than 1.0
            if (diffuseColor.redF() < 1.0 || diffuseColor.greenF() < 1.0 || diffuseColor.blueF() < 1.0) {
                // Set a default color (e.g., light gray or any color you prefer)
                ambientColor = QColor::fromRgbF(0.2, 0.2, 0.2);
                diffuseColor = QColor::fromRgbF(0.498039, 0.498039, 0.498039);
                specularColor = QColor::fromRgbF(1.0, 1.0, 1.0);
                shininess = 0.0f;
            }

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

    // Adding tags so that i can only remove those entities.
    entity->setObjectName("ModelEntity");

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

/****************** Utility Function Implementation ******************/

// This function will check if the current robot is active or not.
bool MainWindow::isCurrentRobotActive()
{
    QModelIndex currentIndex = ui->treeView->currentIndex();
    if (!currentIndex.isValid())
    {
        return false;
    }

    QStandardItem *currentItem = model->itemFromIndex(currentIndex);
    if (!currentItem)
    {
        return false;
    }

    // Check if the current item is the active robot item
    return currentItem == activeRobotItem;
}

// This function will return the parent Robot item of the selected item.
QStandardItem *MainWindow::getParentRobotItem(QStandardItem *item)
{
    while (item)
    {
        if (item->text() == RobotKeys::Robot)
        {
            return item;
        }
        item = item->parent();
    }
    return nullptr;
}


// This function will return the path of all visualization files for the selected robot.
QStringList MainWindow::collectVisualizationPaths(QStandardItem* robotItem)
{
    QStringList filePaths;

    if (!robotItem)
        return filePaths;

    for (int i = 0; i < robotItem->rowCount(); ++i)
    {
        QStandardItem* jointsItem = robotItem->child(i);
        if (jointsItem && jointsItem->text() == RobotKeys::Joints)
        {
            for (int j = 0; j < jointsItem->rowCount(); ++j)
            {
                QStandardItem* singleJointItem = jointsItem->child(j);
                for (int k = 0; k < singleJointItem->rowCount(); ++k)
                {
                    QStandardItem* visualizationItem = singleJointItem->child(k, 0);
                    if (visualizationItem && visualizationItem->text() == JointKeys::Visualization)
                    {
                        // Iterate over all mesh children under the visualization node
                        for (int m = 0; m < visualizationItem->rowCount(); ++m)
                        {
                            QStandardItem* meshNameItem = visualizationItem->child(m, 0);
                            QStandardItem* meshPathItem = visualizationItem->child(m, 1);
                            if (meshPathItem)
                            {
                                QString filePath = meshPathItem->text();
                                if (!filePath.isEmpty())
                                {
                                    filePaths.append(filePath);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return filePaths;
}


/****************** Helper functions ******************/



bool MainWindow::copyDirectoryRecursively(const QString& srcPath, const QString& dstPath)
{
    QDir src(srcPath);
    if (!src.exists())
        return false;

    QDir().mkpath(dstPath);

    for (const QFileInfo& entry : src.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        const QString dst = dstPath + '/' + entry.fileName();
        if (entry.isDir()) {
            if (!copyDirectoryRecursively(entry.absoluteFilePath(), dst))
                return false;
        }
        else {
            QDir().mkpath(QFileInfo(dst).absolutePath());
            QFile::remove(dst);
            if (!QFile::copy(entry.absoluteFilePath(), dst))
                return false;
        }
    }
    return true;
}


// This Helper function is responsible to save the current robot data into a .re package.

bool MainWindow::saveRobot(QStandardItem* robotItem, const QString& targetPath)
{
    if (!robotItem || robotItem->text() != RobotKeys::Robot)
    {
        qWarning() << "Invalid or non-robot item provided.";
        return false;
    }

    // Ask user if no path provided
    QString reFilePath = targetPath;
    if (reFilePath.isEmpty())
    {
        reFilePath = QFileDialog::getSaveFileName(
            this,
            "Save Robot Editor Package",
            QDir::homePath() + "/MyRobot",
            "Robot Editor Package (*.re)"
        );
        if (reFilePath.isEmpty())
            return false;

        if (!reFilePath.endsWith(".re", Qt::CaseInsensitive))
            reFilePath += ".re";
    }

    const QString srcRobotDataDir = robotItem->data(Qt::UserRole + 2).toString();
    if (srcRobotDataDir.isEmpty())
    {
        qWarning() << "Source robot data directory missing.";
        return false;
    }

    QString robotDataFilePath = srcRobotDataDir + '/' + "RobotData.json";

    // Save JSON data
    saveToJson(robotDataFilePath, robotItem);

    // Zip package
    std::string tempReFileStd = robotLib.zipRobotPackage(srcRobotDataDir.toStdString());
    QString tempReFile = QString::fromStdString(tempReFileStd);

    // Move final .re
    if (QFile::exists(reFilePath)) {
        QFile::remove(reFilePath);
    }
    bool moved = QFile::rename(tempReFile, reFilePath);

    // Clean up
    QDir tempDir(srcRobotDataDir);
    tempDir.removeRecursively();

    if (moved && QFile::exists(reFilePath)) {
        QMessageBox::information(this, "Success", "Robot package saved as: " + reFilePath);
        return true;
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to create Robot package.");
        return false;
    }
}





// Helper: Get robot ID from QStandardItem
int MainWindow::getRobotId(QStandardItem* robotItem) const {
    return robotItem ? robotItem->data(Qt::UserRole + 1).toInt() : -1;
}

// Helper: Get robot data dir from QStandardItem
QString MainWindow::getRobotDataDir(QStandardItem* robotItem) const {
    return robotItem ? robotItem->data(Qt::UserRole + 2).toString() : QString();
}



// Load OBJ files for a robot into the 3D viewer
void MainWindow::loadRobotObjFiles(int robotId) {
    remove3DModel(); // Remove any previous entities for this robot
    if (!robotObjFiles.contains(robotId)) return;
    for (const QString& filePath : robotObjFiles[robotId]) {
        QJsonObject jsonObject;
        jsonObject["filePath"] = filePath;
        loadSingleObjFile(jsonObject);
    }
    view->setRootEntity(rootEntity);
}

// Remove OBJ files from disk and internal structures
void MainWindow::removeRobotObjFiles(int robotId) {
    if (!robotObjFiles.contains(robotId)) return;
    for (const QString& filePath : robotObjFiles[robotId]) {
        QFile::remove(filePath);
        QString mtlPath = QFileInfo(filePath).absolutePath() + "/" + QFileInfo(filePath).baseName() + ".mtl";
        QFile::remove(mtlPath);
    }
    robotObjFiles.remove(robotId);
    if (robotDataDirs.contains(robotId)) {
        QDir(robotDataDirs[robotId]).removeRecursively();
        robotDataDirs.remove(robotId);
    }
}

