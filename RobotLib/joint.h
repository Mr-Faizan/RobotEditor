#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <stdexcept> 
#include <vector>
#include <array>
#include "dynamics.h"
#include "kinematics.h"
#include "jsonkeys2.h"

using namespace std;

/**
 * @file joint.h
 * @brief Declaration of Joint class for robot joint properties and dynamics.
 *
 * This file defines the Joint class, which encapsulates properties, kinematics, dynamics,
 * and visualization data for a robot joint. It provides methods for setting and getting
 * joint parameters, managing kinematics and dynamics, and handling visualizations.
 *
 * Main features:
 * - Store joint properties (name, range, speed, coefficients)
 * - Manage kinematics and dynamics objects
 * - Handle multiple visualizations
 * - Provide setters and getters for all properties
 * - Add, create, and retrieve dynamics by payload number
 *
 * @author Faizan Ahmed
 * @date 2025-09-17
 */

/**
 * @class Joint
 * @brief Class representing a robot joint with kinematics, dynamics, and visualization data.
 *
 * The Joint class encapsulates all properties and behaviors of a robot joint, including
 * motion range, speed limits, friction, stiffness, damping, kinematics, dynamics, and
 * visualization files. It supports multiple dynamics payloads and provides methods for
 * managing joint data.
 */
class Joint
{
private:
    std::string jointNumber; /**< Joint number identifier. */
    string name; /**< Joint name. */
    double motionRangeMin; /**< Minimum motion range. */
    double motionRangeMax; /**< Maximum motion range. */
    double jointSpeedLimit; /**< Joint speed limit. */
    double frictionCoefficient; /**< Friction coefficient. */
    double stiffnessCoefficient; /**< Stiffness coefficient. */
    double dampingCoefficient; /**< Damping coefficient. */
    vector <pair<string, string>> visualizations; /**< Visualization files (filename, filepath). */
    JointKinematics kinematics; /**< Kinematics object for the joint. */
    vector<JointDynamics> dynamics; /**< Container for multiple JointDynamics objects. */
    int payloadCounter; /**< Counter for dynamics payloads. */
    std::array<double, 3> translation; /**< Translation vector (Tx, Ty, Tz). */
    std::array<double, 3> rotation; /**< Rotation vector (thetaX, thetaY, thetaZ). */

public:
    /**
     * @brief Default constructor for Joint.
     */
    Joint();

    /**
     * @brief Constructor for Joint with name.
     * @param name Name of the joint.
     */
    Joint(string &name);

    /**
     * @brief Constructor for Joint with all main properties and visualization.
     * @param name Name of the joint.
     * @param motionRangeMin Minimum motion range.
     * @param motionRangeMax Maximum motion range.
     * @param jointSpeedLimit Joint speed limit.
     * @param frictionCoefficient Friction coefficient.
     * @param stiffnessCoefficient Stiffness coefficient.
     * @param dampingCoefficient Damping coefficient.
     * @param visualization Visualization file path.
     */
    Joint(string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization);

    /**
     * @brief Destructor for Joint.
     */
    ~Joint() = default;

    // Setters
    /** @brief Set the joint number. */
    void setJointNumber(const string &jointNumber) { this->jointNumber = jointNumber; }
    /** @brief Set the joint name. */
    void setName(const string &name) { this->name = name; }
    /** @brief Set the minimum motion range. */
    void setMotionRangeMin(double min) { this->motionRangeMin = min; }
    /** @brief Set the maximum motion range. */
    void setMotionRangeMax(double max) { this->motionRangeMax = max; }
    /** @brief Set the joint speed limit. */
    void setJointSpeedLimit(double speed) { this->jointSpeedLimit = speed; }
    /** @brief Set the friction coefficient. */
    void setFrictionCoefficient(double friction) { this->frictionCoefficient = friction; }
    /** @brief Set the stiffness coefficient. */
    void setStiffnessCoefficient(double stiffness) { this->stiffnessCoefficient = stiffness; }
    /** @brief Set the damping coefficient. */
    void setDampingCoefficient(double damping) { this->dampingCoefficient = damping; }
    /** @brief Set the kinematics object. */
    void setKinematics(const JointKinematics &kinematics) { this->kinematics = kinematics; }
    /** @brief Set the translation vector. */
    void setTranslation(const std::array<double, 3> &t) { translation = t; }
    /** @brief Set the rotation vector. */
    void setRotation(const std::array<double, 3> &r) { rotation = r; }
    /** @brief Set the visualizations vector. */
    void setVisualizations(const vector<pair<string, string>>& vis) { visualizations = vis; }

    // Getters
    /** @brief Get the joint number. */
    string getJointNumber() const { return jointNumber; }
    /** @brief Get the joint name. */
    string getName() const { return name; }
    /** @brief Get the minimum motion range. */
    double getMotionRangeMin() const { return motionRangeMin; }
    /** @brief Get the maximum motion range. */
    double getMotionRangeMax() const { return motionRangeMax; }
    /** @brief Get the joint speed limit. */
    double getJointSpeedLimit() const { return jointSpeedLimit; }
    /** @brief Get the friction coefficient. */
    double getFrictionCoefficient() const { return frictionCoefficient; }
    /** @brief Get the stiffness coefficient. */
    double getStiffnessCoefficient() const { return stiffnessCoefficient; }
    /** @brief Get the damping coefficient. */
    double getDampingCoefficient() const { return dampingCoefficient; }
    /** @brief Get the kinematics object. */
    JointKinematics getKinematics() const { return kinematics; }
    /** @brief Get the dynamics vector. */
    vector<JointDynamics> getDynamics() const { return dynamics; }
    /** @brief Get the translation vector. */
    const array<double, 3>& getTranslation() const { return translation; }
    /** @brief Get the rotation vector. */
    const array<double, 3>& getRotation() const { return rotation; }
    /** @brief Get the visualizations vector. */
    const vector<pair<string, string>>& getVisualizations() const { return visualizations; }

    /**
     * @brief Add a visualization file to the joint.
     * @param filename Name of the visualization file.
     * @param filepath Path to the visualization file.
     */
    void addVisualization(const string& filename, const string& filepath) { visualizations.emplace_back(filename, filepath); }

    /**
     * @brief Clear all visualizations from the joint.
     */
    void clearVisualizations() { visualizations.clear(); }

    // General Functions
    /**
     * @brief Add a JointDynamics object to the joint.
     * @param dynamics JointDynamics object to add.
     */
    void addDynamics(JointDynamics &dynamics);

    /**
     * @brief Create and add a new JointDynamics object to the joint.
     * @return Reference to the newly added JointDynamics object.
     */
    JointDynamics& createAndAddDynamics();

    /**
     * @brief Add a JointDynamics payload to the joint.
     * @param payload JointDynamics object to add.
     * @return Reference to the newly added JointDynamics object.
     */
    JointDynamics& setDynamics(JointDynamics &payload);

    /**
     * @brief Get a JointDynamics object by its payload number.
     * @param payloadNumber Payload number identifier.
     * @return Reference to the matching JointDynamics object.
     * @throws std::runtime_error if not found.
     */
    JointDynamics& getDynamicsByPayloadNumber(const string &payloadNumber);
};

#endif // JOINT_H
