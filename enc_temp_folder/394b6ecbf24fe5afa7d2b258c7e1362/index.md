# RobotEditor User Guide

Welcome to the user guide for **RobotEditor**! This guide will help you get started, explain the main features, and walk you through common tasks step-by-step. Screenshots can be added at the indicated locations for visual reference.

---

## Table of Contents

- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Main Features](#main-features)
- [Step-by-Step Usage](#step-by-step-usage)
    - [Launching the Application](#launching-the-application)
    - [Creating a New Robot](#creating-a-new-robot)
    - [Adding Multiple Robots](#adding-multiple-robots)
    - [Importing Robots](#importing-robots)
    - [Opening Robot Packages](#opening-robot-packages)
    - [Editing Robot and Joint Properties](#editing-robot-and-joint-properties)
    - [Visualizing Robots in 3D](#visualizing-robots-in-3d)
    - [Exporting and Saving Robots](#exporting-and-saving-robots)
    - [Deleting Robots, Joints, and Payloads](#deleting-robots-joints-and-payloads)
- [Troubleshooting & FAQ](#troubleshooting--faq)

---

## Introduction

**RobotEditor** is a Qt-based application for creating, editing, visualizing, and managing robot configurations. It supports 3D visualization, joint and dynamics editing, and import/export of robot data in various formats.

---

## Getting Started

1. **Install RobotEditor** on your system.
2. Launch the application from your desktop or terminal.
3. The main window will appear, showing a tree view for robot management and a 3D visualization area.

![Main Window](path/to/main_window.png)

---

## Main Features

- **Create, Import, Open, Save, and Delete Robots**
- **3D Visualization** of robot models and joints
- **Edit Robot and Joint Properties** via a tree view
- **Export/Import** robot data in `.re` package format
- **Support for Joint Dynamics and Kinematics Editing**
- **Context Menu Actions** for adding/removing joints and payloads
- **JSON Serialization/Deserialization** for robot data

---

## Step-by-Step Usage

### Launching the Application

- Double-click the RobotEditor icon or run it from the terminal.
- The main window will open with a tree view and 3D visualization area.

![Launch Screenshot](path/to/launch.png)

### Creating a New Robot

1. **Right-click** in the tree view area.
2. Select **"New Robot"** from the context menu.
3. A new robot will be added to the tree view.

![New Robot Screenshot](path/to/new_robot.png)

### Adding Multiple Robots

- Repeat the steps above to add more robots.
- Each robot will appear as a separate item in the tree view.

![Multiple Robots Screenshot](path/to/multiple_robots.png)

### Importing Robots

#### From VCMX File
1. Right-click in the tree view.
2. Select **"Import from Visual Component"**.
3. Choose a `.vcmx` file from your device.
4. The robot will be imported and displayed.

#### From VCMX Folder
1. Right-click in the tree view.
2. Select **"Import from Visual Component Folder"**.
3. Choose a folder containing VCMX data.
4. The robot will be imported and displayed.

![Import Screenshot](path/to/import.png)

### Opening Robot Packages

1. Right-click in the tree view.
2. Select **"Open from Device..."**.
3. Choose a `.re` package file.
4. The robot data and 3D model will be loaded.

![Open Package Screenshot](path/to/open_package.png)

### Editing Robot and Joint Properties

- Click on a robot in the tree view to expand its properties.
- Edit robot name, manufacturer, payload, etc.
- Expand the **Joints** section to view and edit joint properties.
- Right-click on **Joints** to add or delete joints.
- Right-click on **Joint Dynamics** to add or delete payloads.

![Edit Properties Screenshot](path/to/edit_properties.png)

### Visualizing Robots in 3D

- Select a robot to make it active (right-click > "Set as Active Robot").
- The 3D model will appear in the visualization area.
- Use the **Reset Model** and **Rotate Model** actions to adjust the view.
- To visualize a joint, right-click on **Visualization** and select **"Select OBJ File"** to load a 3D mesh.

![3D Visualization Screenshot](path/to/3d_visualization.png)

### Exporting and Saving Robots

- To save a robot, right-click on it and select **"Save Robot"**.
- To save all robots, use the **"Save All Robots"** action.
- Robots are saved as `.re` package files, which include all data and 3D models.

![Save Screenshot](path/to/save.png)

### Deleting Robots, Joints, and Payloads

- To delete a robot, right-click on it and select **"Delete Robot"**.
- To delete all robots, use the **"Delete All Robots"** action.
- To delete a joint or payload, right-click on the respective item and select the delete option.

![Delete Screenshot](path/to/delete.png)

---

## Troubleshooting & FAQ

### Common Issues

- **No 3D Model Appears:** Ensure you have loaded OBJ files for visualization.
- **Cannot Save Robot:** Check that all required properties are filled in.
- **Import Fails:** Verify the VCMX file/folder is valid and accessible.

### FAQ

**Q: Can I edit joint kinematics and dynamics?**
A: Yes, expand the joint in the tree view and edit the properties as needed.

**Q: How do I export my robot data?**
A: Use the "Save Robot" or "Save All Robots" actions to export as `.re` packages.

**Q: Can I visualize custom meshes?**
A: Yes, use the "Select OBJ File" option under Visualization for each joint.

---

## More Help

For advanced usage, refer to the Doxygen documentation or contact support.

---

