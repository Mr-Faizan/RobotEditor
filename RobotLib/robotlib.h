/**
 * @file robotlib.h
 * @brief Declaration of RobotLib class for robot management, file I/O, and VCMX import.
 *
 * This file defines the RobotLib class, which provides methods for managing a collection of robots,
 * loading and saving robot data to JSON, importing robots from VCMX files, and handling robot packages.
 *
 * Main features:
 * - Manage a collection of Robot objects
 * - Add, remove, and retrieve robots by name or ID
 * - Load and save robot data from/to JSON files
 * - Import robots from VCMX files and parse robot data
 * - Zip and extract robot package folders
 * - Print robot and joint data for debugging
 *
 * 
 * 
 * 
 * @section extlibs External Libraries Used
 *
 * - @b nlohmann/json:
 *   Used for JSON serialization and deserialization throughout the project.
 *   The header file (json.hpp) is included directly from the nlohmann folder.
 *   Location: RobotLib/libraries/nlohmann/json.hpp
 *
 * - @b miniz:
 *   Used for zipping and unzipping VCMX files and creating RE package files.
 *   Source code is in RobotLib/libraries/miniz, and the compiled DLL is in RobotLib/libraries/miniz-install.
 *   The compiled version is used for faster builds.
 *   Location: RobotLib/libraries/miniz (source), RobotLib/libraries/miniz-install (compiled)
 *
 * - @b Assimp:
 *   Used to convert 3DS images into OBJ format for 3D visualization in Qt3D.
 *   Source code is in robotlib/libraries/assimp, and the compiled version is in robotlib/libraries/assimp-install.
 *   The compiled version is used for faster builds.
 *   Location: robotlib/libraries/assimp (source), robotlib/libraries/assimp-install (compiled)
 *
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * @author Faizan Ahmed
 * @date 2025-09-19
 */

#ifndef ROBOTLIB_H
#define ROBOTLIB_H

#include <iostream>
#include <string>
#include <stdexcept> 
#include <vector>
#include <fstream>

#include <miniz/miniz.h>
#include <filesystem>
#include <cstring> // for memset

#include "RobotLib_global.h"
#include "robot.h"
#include "nlohmann/json.hpp"

#include "jsonkeys2.h"

#include "importvcmx.h"

using namespace std;
using json = nlohmann::json;

/**
 * @class RobotLib
 * @brief Main library class for robot management, file I/O, and VCMX import.
 *
 * The RobotLib class provides a high-level interface for managing robots, including creation,
 * loading, saving, and importing from VCMX files. It supports file operations, robot collection
 * management, and utility functions for robot data.
 */
class ROBOTLIB_EXPORT RobotLib
{
private:
    vector<Robot> robotCollection; /**< Collection of Robot objects. */

public:
    /**
     * @brief Default constructor for RobotLib.
     */
    RobotLib();
    /**
     * @brief Destructor for RobotLib.
     */
    ~RobotLib();
    // Robot management
    /**
     * @brief Initialize a new Robot with a default joint and dynamics.
     * @return New Robot object.
     */
    Robot initializeNewRobot();
    /**
     * @brief Create a new Robot object.
     * @return New Robot object.
     */
    Robot createRobot();
    /**
     * @brief Create a new Joint object.
     * @return New Joint object.
     */
    Joint createJoint();
    /**
     * @brief Create a new JointDynamics object.
     * @return New JointDynamics object.
     */
    JointDynamics createDynamics();

    /**
     * @brief Add a Robot to the collection.
     * @param robot Robot object to add.
     */
    void addRobot(const Robot &robot);
    /**
     * @brief Remove a Robot from the collection by name.
     * @param name Name of the robot to remove.
     */
    void removeRobot(const string &name);
    /**
     * @brief Get a Robot by name.
     * @param name Name of the robot.
     * @return Reference to the Robot object.
     * @throws std::runtime_error if not found.
     */
    Robot& getRobot(const string &name);
    /**
     * @brief Get the collection of robots.
     * @return Const reference to the vector of Robot objects.
     */
    const vector<Robot> &getRobots() const;
    /**
     * @brief Get a Robot by ID.
     * @param robotId Robot ID.
     * @return Reference to the Robot object.
     * @throws std::runtime_error if not found.
     */
    Robot& getRobotById(int robotId);
    /**
     * @brief Get a Robot by ID (const pointer version).
     * @param robotId Robot ID.
     * @return Pointer to the Robot object, or nullptr if not found.
     */
    const Robot *getRobotById2(int robotId) const;

    /**
     * @brief Update and save robot data to a file.
     * @param filePath Path to the file.
     * @param json JSON data to save.
     * @return True if successful, false otherwise.
     */
    bool updateAndSaveRobotData(const std::string &filePath, const json &json);
    /**
     * @brief Load a Robot from a JSON file.
     * @param filePath Path to the file.
     * @return Loaded Robot object.
     */
    Robot loadFromFile(const string &filePath);
    /**
     * @brief Get a number value from a JSON object (handles string/number types).
     * @param value JSON value.
     * @return Number as double.
     */
    double getNumberFromJson(const json &value);
    // Load and save data
    /**
     * @brief Load a Robot from a JSON object.
     * @param jsonData JSON data.
     * @return Loaded Robot object.
     */
    Robot loadFromJson(const json jsonData);
    /**
     * @brief Save a Robot to a JSON file.
     * @param filePath Path to the file.
     * @param robot Robot object to save.
     * @return True if successful, false otherwise.
     */
    bool saveToJson(const std::string &filePath, Robot &robot) const;
    /**
     * @brief Zip a robot package folder into a .re file.
     * @param folderPath Path to the folder.
     * @return Path to the .re file.
     */
    string zipRobotPackage(const string& folderPath);
    /**
     * @brief Extract a robot package .re file to a destination directory.
     * @param reFilePath Path to the .re file.
     * @param destDir Destination directory.
     * @return True if successful, false otherwise.
     */
    bool extractRobotPackage(const string& reFilePath, const string& destDir);



    // Print data
    /**
     * @brief Print all robot and joint data to the console.
     */
    void printData() const;
    // New function to initiate vcmxDataExtractor
    /**
     * @brief Import a robot from a VCMX file.
     * @param filePath Path to the VCMX file.
     * @return Imported Robot object.
     */
    Robot importRobotFromVCMX(const string &filePath);
    /**
     * @brief Parse robot data from a VCMX data folder.
     * @param robotDataFolderPath Path to the robot data folder.
     * @return Parsed Robot object.
     */
    Robot parseRobotFromVCMX(const string &robotDataFolderPath);
};

#endif // ROBOTLIB_H
