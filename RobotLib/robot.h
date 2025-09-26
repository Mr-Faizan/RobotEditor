#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include <stdexcept> 
#include <vector>
#include "joint.h"

using namespace std;

/**
 * @file robot.h
 * @brief Declaration of Robot class for robot entity and joint aggregation.
 *
 * This file defines the Robot class, which represents a robot entity composed of joints.
 * The Robot class manages robot-level properties (name, manufacturer, payload, etc.),
 * and provides methods for adding, creating, and retrieving joints.
 * Joints are aggregated as part of the robot, and each joint can have its own kinematics, dynamics, and visualization data.
 *
 * Main features:
 * - Store robot properties (name, manufacturer, payload, reach, etc.)
 * - Aggregate and manage multiple joints
 * - Provide setters and getters for all properties
 * - Add, create, and retrieve joints by joint number
 *
 * @author 
 * @date 2025-09-17
 */

/**
 * @class Robot
 * @brief Class representing a robot entity composed of joints.
 *
 * The Robot class encapsulates all properties and behaviors of a robot, including
 * its name, manufacturer, payload, reach, and a collection of joints. It supports
 * adding, creating, and retrieving joints, and manages a unique ID for each robot.
 */
class Robot
{

private:
    static int lastAssignedId;           /**< Static counter for unique robot IDs. */
    int id;                              /**< Unique robot ID. */
    string name;                         /**< Robot name. */
    string manufacturer;                 /**< Robot manufacturer. */
    double payload;                      /**< Maximum payload. */
    double footPrint;                    /**< Robot footprint. */
    double maxReach;                     /**< Maximum reach. */
    double repeatability;                /**< Repeatability. */
    double weight;                       /**< Robot weight. */
    int dof;                             /**< Degrees of freedom. */
    std::vector<Joint> joints;           /**< Collection of joints. */
    int jointCounter;                    /**< Counter for joints. */
    string robotDataDir;                 /**< Path to RobotData for this robot. */
public:
    /**
     * @brief Default constructor for Robot.
     */
    Robot();

    /**
     * @brief Constructor for Robot with name.
     * @param name Name of the robot.
     */
    Robot(const std::string &name);

    /**
     * @brief Constructor for Robot with all main properties.
     * @param name Name of the robot.
     * @param manufacturer Manufacturer name.
     * @param payload Maximum payload.
     * @param footPrint Robot footprint.
     * @param maxReach Maximum reach.
     * @param repeatability Repeatability.
     * @param weight Robot weight.
     * @param dof Degrees of freedom.
     */
    Robot(const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof);

    /**
     * @brief Destructor for Robot.
     */
    ~Robot(){};

    // Setters
    /** @brief Set the robot ID (for testing only). */
    void setId(int id) { this->id = id; }
    /** @brief Set the robot name. */
    void setName(const string &name) { this->name = name; }
    /** @brief Set the manufacturer name. */
    void setManufacturer(const string &manufacturer) { this->manufacturer = manufacturer; }
    /** @brief Set the maximum payload. */
    void setPayload(double payload) { this->payload = payload; }
    /** @brief Set the robot footprint. */
    void setFootprint(double footPrint) { this->footPrint = footPrint; }
    /** @brief Set the maximum reach. */
    void setMaxReach(double maxReach) { this->maxReach = maxReach; }
    /** @brief Set the repeatability. */
    void setRepeatability(double repeatability) { this->repeatability = repeatability; }
    /** @brief Set the robot weight. */
    void setWeight(double weight) { this->weight = weight; }
    /** @brief Set the degrees of freedom. */
    void setDof(int dof) { this->dof = dof; }
    /** @brief Set the robot data directory. */
    void setRobotDataDir(const string& dir) { robotDataDir = dir; }
    

    // Getters
    /** @brief Get the robot ID. */
    int getId() const { return id; }
    /** @brief Get the robot name. */
    string getName() const { return name; }
    /** @brief Get the manufacturer name. */
    string getManufacturer() const { return manufacturer; }
    /** @brief Get the maximum payload. */
    double getPayload() const { return payload; }
    /** @brief Get the robot footprint. */
    double getFootprint() const { return footPrint; }
    /** @brief Get the maximum reach. */
    double getMaxReach() const { return maxReach; }
    /** @brief Get the repeatability. */
    double getRepeatability() const { return repeatability; }
    /** @brief Get the robot weight. */
    double getWeight() const { return weight; }
    /** @brief Get the degrees of freedom. */
    int getDof() const { return dof; }
    /** @brief Get the collection of joints. */
    const std::vector<Joint> &getJoints() const { return joints; }

    // General Functions
    /**
     * @brief Add a joint to the robot.
     * @param joint Joint object to add.
     */
    void addJoint(const Joint &joint);

    /**
     * @brief Create and add a new joint to the robot.
     * @return Reference to the newly added Joint object.
     */
    Joint& createAndAddJoint();

    /**
     * @brief Get a joint by its joint number.
     * @param jointNumber Joint number identifier.
     * @return Reference to the matching Joint object.
     * @throws std::runtime_error if not found.
     */
    Joint& getJointByJointNumber(const std::string &jointNumber);
    /** @brief Get the robot data directory. */
    const string& getRobotDataDir() const { return robotDataDir; }
};

#endif // ROBOT_H
