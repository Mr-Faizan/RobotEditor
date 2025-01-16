# RobotEditor

## Overview

**RobotEditor** is a desktop application designed to manage and simulate the kinematics and dynamics of robots. The application allows users to define and manipulate various components of a robot, including joints, tools, and the overall robot structure. This documentation provides an overview of the project's how to get started, key components, structure, UML Diagram, and User Interface Layout.

<details>
<summary>Getting Started</summary>

### Clone the Repository

```bash
git clone https://gitlab.cc-asp.fraunhofer.de/fahmed1/roboteditor
cd RobotEditor
```

### Build the Project

- Ensure you have the necessary build tools and dependencies installed.
- Use your preferred build system (e.g., CMake, qmake) to compile the project.

### Run the Application

- Execute the compiled binary to start the RobotEditor application.
- Use the user interface to add, modify, and simulate robot components.

</details>

<details>
<summary>Key Components</summary>

### 1. Joint
- Represents a single joint in the robot.
- Contains properties such as motion range, speed limits, and friction coefficients.
- Manages multiple dynamic properties through the JointDynamics class.

### 2. Kinematics
- Handles the kinematic calculations for joints, robots, and tools.
- Includes classes like JointKinematics, RobotKinematics, and ToolKinematics.
- Uses DH parameters and rotational values to perform calculations.

### 3. Dynamics
- Manages the dynamic properties and calculations for joints, robots, and tools.
- Includes classes like JointDynamics, RobotDynamics, and ToolDynamics.
- Calculates forces, torques, and other dynamic properties.

### 4. Visualization
- Handles the visualization aspects of the robot and its components.
- Provides a graphical representation of the robot's structure and movements.

</details>

<details>
<summary>Project Structure</summary>

### Headers (.h files)
Contain class declarations and inline method definitions.

### Source (.cpp files)
Contain the implementation of class methods.

### Main Application
Integrates all components and provides the user interface for interacting with the robot model.

### Key Classes and Their Responsibilities

- **Joint:** Manages joint-specific properties and dynamics.
- **JointKinematics:** Performs kinematic calculations for joints.
- **JointDynamics:** Handles dynamic properties and calculations for joints.
- **RobotKinematics:** Manages kinematic calculations for the entire robot.
- **ToolKinematics:** Handles kinematic calculations for tools attached to the robot.
- **Visualization:** Provides graphical representation and visualization of the robot.

</details>

<details>
<summary>UML Diagram</summary>

Here I will add the UML Class Diagram of this application.

</details>

<details>
<summary>User Interface Overview</summary>

Here I will add the screenshots of the application, once it is completed.

</details>