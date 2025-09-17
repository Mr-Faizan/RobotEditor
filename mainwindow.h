#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*!
 * \file mainwindow.h
 * \brief Declaration of the MainWindow class for the RobotEditor application.
 *
 * This header defines the MainWindow class, which provides the main user interface for RobotEditor.
 * It manages robot configuration, 3D visualization, file operations, and user interaction via a tree view and context menus.
 *
 * Main features:
 * - Create, import, open, save, and delete robot configurations.
 * - Visualize robot models and joints in a 3D scene using Qt3D.
 * - Edit robot and joint properties via a tree view.
 * - Export and import robot data in .re package format.
 * - Support for joint dynamics and kinematics editing.
 * - Context menu actions for adding/removing joints and payloads.
 * - JSON serialization and deserialization for robot data.
 *
 * \author Faizan Ahmed
 * \date 2025-09-15
 *
 * \see mainwindow.cpp
 * \see RobotLib
 */

 // QT Libraries
#include <QMainWindow>
#include <QWidget>
#include <QDebug>
#include <iostream>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>

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
/*!
 * \class MainWindow
 * \brief Main application window for RobotEditor.
 *
 * Handles UI, robot management, 3D visualization, and file operations.
 */

	class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:

	/*!
 * \brief Sets the selected robot as the active robot and loads its 3D model.
 */
	void on_actionActiveRobot_triggered();

	/*!
	 * \brief Saves the currently selected robot to a .re package.
	 */
	void on_actionSave_triggered();

	/*!
	 * \brief Saves all robots in the tree view to .re packages.
	 */
	void on_actionSaveAll_triggered();

	/*!
	 * \brief Deletes all robots from the tree view and removes their 3D models.
	 */
	void on_actionDeleteAll_triggered();

	/*!
	 * \brief Creates a new robot and adds it to the tree view.
	 */
	void on_actionNewRobot_triggered();

	/*!
	 * \brief Opens a robot from a .re package file and loads its data and 3D model.
	 */
	void on_actionOpenFromDevice_triggered();

	/*!
	 * \brief Imports a robot from a VCMX file and adds it to the tree view.
	 */
	void on_actionImportFromVisualComponent_triggered();

	/*!
	 * \brief Imports a robot from a folder containing VCMX data and adds it to the tree view.
	 */
	void on_actionImportFromVisualComponentFolder_triggered();

	/*!
	 * \brief Resets the 3D model view to its original camera position and orientation.
	 */
	void on_actionResetModel_triggered();

	/*!
	 * \brief Toggles rotation animation of the 3D model.
	 */
	void on_actionRotateModel_triggered();

	/*!
	 * \brief Allows the user to select an OBJ file for joint visualization and loads it into the 3D scene.
	 */
	void on_actionJointVisualization_triggered();

	/*!
	 * \brief Adds a new joint to the currently selected robot's joints item.
	 */
	void addNewJoint();

	/*!
	 * \brief Adds a new dynamics payload to the currently selected joint dynamics item.
	 */
	void addNewDynamics();

	/*!
	 * \brief Deletes the currently selected robot from the tree view and removes its 3D model.
	 */
	void deleteCurrentRobot();

	/*!
	 * \brief Deletes the last joint from the currently selected joints item.
	 */
	void deleteCurrentJoint();

	/*!
	 * \brief Deletes the last dynamics payload from the currently selected joint dynamics item.
	 */
	void deleteCurrentPayload();

private:
	/*! \brief UI object for the main window. */
	Ui::MainWindow* ui;
	/*! \brief Model to store the tree view data. */
	QStandardItemModel* model;
	/*! \brief Template object to store the template data. */
	QJsonObject templateObject;
	/*! \brief Pointer to the currently active robot item in the tree view. */
	QStandardItem* activeRobotItem = nullptr;
	/*! \brief 3D window for robot visualization. */
	Qt3DExtras::Qt3DWindow* view;
	/*! \brief Root entity for the 3D scene. */
	Qt3DCore::QEntity* rootEntity;
	/*! \brief Camera for the 3D scene. */
	Qt3DRender::QCamera* camera;
	/*! \brief Timer for rotation animation. */
	QTimer* rotationTimer;
	/*! \brief Map to store the entities for each OBJ file. */
	QMap<QString, Qt3DCore::QEntity*> entityMap;
	/*! \brief Map from robotId to list of OBJ file paths. */
	QMap<int, QStringList> robotObjFiles; // robotId -> list of OBJ file paths
	/*! \brief Map from robotId to robot data directory. */
	QMap<int, QString> robotDataDirs;     // robotId -> robot data directory

	/*! \brief Current rotation angle for 3D model animation. */
	float rotationAngle = 0.0f;
	/*! \brief Flag to indicate if rotation is enabled. */
	bool rotating = false;
	/*! \brief RobotLib object for robot operations. */
	RobotLib robotLib;


	// void loadTemplate();
	// Custom Functions
	/*!
	 * \brief Shows the context menu for the tree view at the specified position.
	 * \param pos The position in the tree view where the menu should be shown.
	 */
	void showContextMenu(const QPoint& pos);

	/*!
	 * \brief Populates the tree view with the data from the specified robot.
	 * \param robot The Robot object whose data will be displayed.
	 */
	void populateTreeView(const Robot& robot);
	/*!
	 * \brief Adds a new joint to the selected robot.
	 * \param jointsItem Pointer to the parent QStandardItem representing the joints.
	 * \param joint The Joint object to add to the tree view.
	 */
	void addJoint(QStandardItem* jointsItem, const Joint& joint);
	/*!
	 * \brief Adds a dynamics payload to the specified dynamics item in the tree view.
	 * \param dynamicsItem Pointer to the parent QStandardItem representing the dynamics.
	 * \param dynamics The JointDynamics object to add.
	 */
	void addDynamicsPayload(QStandardItem* dynamicsItem, const JointDynamics& dynamics);
	// void addDynamicsPayload(QStandardItem *dynamicsItem, const QString &payloadKey, const QJsonObject &payload);
	/*!
	 * \brief Adds a key-value item to the specified parent item in the tree view.
	 * \param parent Pointer to the parent QStandardItem.
	 * \param key The key string for the item.
	 * \param value The value to display.
	 */
	void addItem(QStandardItem* parent, const QString& key, const QVariant& value);
	/*!
	 * \brief Adds a combo box item to the specified parent item in the tree view.
	 * \param parent Pointer to the parent QStandardItem.
	 * \param key The key string for the item.
	 * \param value The default value for the combo box.
	 */
	void addComboBoxItem(QStandardItem* parent, const QString& key, const QString& value);
	/*!
	 * \brief Saves the robot data to a JSON file.
	 * \param filePath The path to the JSON file.
	 * \param currentItem Pointer to the QStandardItem representing the robot.
	 */
	void saveToJson(const QString& filePath, QStandardItem* currentItem);
	/*!
	 * \brief Converts the robot item and its children to a QJsonObject for serialization.
	 * \param robotItem Pointer to the QStandardItem representing the robot.
	 * \return QJsonObject containing the robot's data.
	 */
	QJsonObject modelToJson(QStandardItem* robotItem);
	/*!
	 * \brief Recursively copies the contents of a directory.
	 * \param srcPath Source directory path.
	 * \param dstPath Destination directory path.
	 * \return True if the copy was successful, false otherwise.
	 */
	bool copyDirectoryRecursively(const QString& srcPath, const QString& dstPath);

	/*!
	 * \brief Saves the current robot data into a .re package.
	 * \param robotItem Pointer to the QStandardItem representing the robot.
	 * \param targetPath Target path for the .re package file.
	 * \return True if the save was successful, false otherwise.
	 */
	bool saveRobot(QStandardItem* robotItem, const QString& targetPath = QString());

	/*!
	 * \brief Loads OBJ files for a robot into the 3D viewer.
	 * \param robotId The ID of the robot whose OBJ files should be loaded.
	 */
	void loadRobotObjFiles(int robotId);
	/*!
	 * \brief Removes OBJ files for a robot from disk and internal structures.
	 * \param robotId The ID of the robot whose OBJ files should be removed.
	 */
	void removeRobotObjFiles(int robotId);
	/*!
	 * \brief Gets the robot ID from the specified QStandardItem.
	 * \param robotItem Pointer to the QStandardItem representing the robot.
	 * \return The robot ID as an integer, or -1 if not found.
	 */
	int getRobotId(QStandardItem* robotItem) const;
	/*!
	 * \brief Gets the robot data directory from the specified QStandardItem.
	 * \param robotItem Pointer to the QStandardItem representing the robot.
	 * \return The robot data directory as a QString.
	 */
	QString getRobotDataDir(QStandardItem* robotItem) const;



	// 3D Window Playground
	/*!
	 * \brief Sets up the 3D playground for robot model visualization.
	 */
	void setup3DPlayground();
	/*!
	 * \brief Loads the 3D model for the active robot into the scene.
	 */
	void load3DModel();
	/*!
	 * \brief Removes the 3D model from the scene.
	 */
	void remove3DModel();
	/*!
	 * \brief Loads all OBJ files from the specified directory into the 3D scene.
	 * \param directoryPath The path to the directory containing OBJ files.
	 */
	void loadObjFiles(const QString& directoryPath);
	/*!
	 * \brief Loads a single OBJ file and adds it to the 3D scene.
	 * \param jsonObject QJsonObject containing the file path and optional transformation data.
	 */
	void loadSingleObjFile(const QJsonObject& jsonObject);
	/*!
	 * \brief Deletes the current OBJ file from the 3D scene.
	 * \param currentItem Pointer to the QStandardItem representing the visualization item.
	 */
	void deleteCurrentObjFile(QStandardItem* currentItem);
	/*!
	 * \brief Parses an MTL file and extracts material properties.
	 * \param mtlFilePath Path to the MTL file.
	 * \param ambient Reference to store the ambient color.
	 * \param diffuse Reference to store the diffuse color.
	 * \param specular Reference to store the specular color.
	 * \param shininess Reference to store the shininess value.
	 * \param transparency Reference to store the transparency value.
	 * \param illumModel Reference to store the illumination model.
	 * \return True if parsing was successful, false otherwise.
	 */
	bool parseMtlFile(const QString& mtlFilePath, QColor& ambient, QColor& diffuse, QColor& specular, float& shininess, float& transparency, int& illumModel);
	/*!
	 * \brief Updates the rotation and translation of the 3D model for animation.
	 */
	void updateRotation();

	/*!
	 * \brief Checks if the currently selected robot is the active robot.
	 * \return True if the current robot is active, false otherwise.
	 */
	bool isCurrentRobotActive();
	/*!
	 * \brief Returns the parent robot item of the specified item.
	 * \param item Pointer to the QStandardItem whose parent robot item is to be found.
	 * \return Pointer to the parent robot QStandardItem, or nullptr if not found.
	 */
	QStandardItem* getParentRobotItem(QStandardItem* item);
	/*!
	 * \brief Collects all visualization file paths for the specified robot.
	 * \param robotItem Pointer to the QStandardItem representing the robot.
	 * \return QStringList of file paths for visualization.
	 */
	QStringList collectVisualizationPaths(QStandardItem* robotItem);

};
#endif // MAINWINDOW_H
