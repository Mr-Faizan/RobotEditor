/**
 * @file robot.cpp
 * @brief Implementation of Robot class for robot entity and joint aggregation.
 *
 * This file contains the implementation of the Robot class, which manages robot-level
 * properties and a collection of joints. It provides methods for adding, creating,
 * and retrieving joints, and manages unique IDs for each robot instance.
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

#include "robot.h"

using namespace std;

// Initialize the static member variable
int Robot::lastAssignedId = 1000;

/**
 * @brief Default constructor for Robot.
 */
Robot::Robot()
    : id(++lastAssignedId),
      name(""),
      manufacturer(""),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0)
{
    // Add a default joint
    //addJoint();
}

/**
 * @brief Constructor for Robot with name.
 * @param name Name of the robot.
 */
Robot::Robot(const string &name)
    : id(++lastAssignedId),
      name(name),
      manufacturer(""),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0),
      jointCounter(0)
{
    // Add a default joint
    //addJoint();
}

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
Robot::Robot(const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
    : id(++lastAssignedId),
      name(name),
      manufacturer(manufacturer),
      payload(payload),
      footPrint(footPrint),
      maxReach(maxReach),
      repeatability(repeatability),
      weight(weight),
      dof(dof),
      jointCounter(0)
{
    // Add a default joint
    //addJoint();
}

/**
 * @brief Add a joint to the robot.
 * @param joint Joint object to add.
 */
void Robot::addJoint(const Joint &joint)
{
    joints.push_back(joint);
}

/**
 * @brief Create and add a new joint to the robot.
 * @return Reference to the newly added Joint object.
 */
Joint& Robot::createAndAddJoint()
{
    Joint newJoint;
    // newJoint.setJointNumber(JointKeys2::Joint + std::to_string(++jointCounter));
    joints.push_back(newJoint);
    return joints.back();
}

/**
 * @brief Get a joint by its joint number.
 * @param jointNumber Joint number identifier.
 * @return Reference to the matching Joint object.
 * @throws std::runtime_error if not found.
 */
Joint &Robot::getJointByJointNumber(const std::string &jointNumber)
{
    for (auto &joint : joints)
    {
        if (joint.getJointNumber() == jointNumber)
        {
            return joint;
        }
    }
    throw std::runtime_error("Joint not found");
}
