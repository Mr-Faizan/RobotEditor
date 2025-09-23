#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <string>

using namespace std;

/**
 * @file kinematics.h
 * @brief Declaration of Kinematics classes for robot, joint, and tool kinematics calculations.
 *
 * This file defines the base class Kinematics and its derived classes JointKinematics, RobotKinematics,
 * and ToolKinematics. These classes provide an abstraction for calculating kinematic properties for
 * robots, joints, and tools, using inheritance and polymorphism. JointKinematics includes structures
 * for DH parameters and rotational values, with methods for setting and getting these properties.
 *
 * Main features:
 * - Abstract base class for kinematics calculations
 * - Derived classes for joint, robot, and tool kinematics
 * - Structures for DH parameters and rotational values
 * - Setters and getters for kinematic properties
 * - Polymorphic interface for calculating DH parameters
 *
 * @author Faizan Ahmed
 * @date 2025-09-18
 */

/**
 * @class Kinematics
 * @brief Abstract base class for kinematics calculations.
 *
 * Provides a pure virtual interface for calculating DH parameters, to be implemented by derived classes.
 */
class Kinematics
{
public:
    /**
     * @brief Pure virtual function to calculate DH parameters.
     */
    virtual void calculateDhParams() = 0;
};

/**
 * @class JointKinematics
 * @brief Class for calculating kinematics of robot joints.
 *
 * Inherits from Kinematics and provides structures and methods for joint-specific kinematics calculations,
 * including DH parameters and rotational values.
 */
class JointKinematics : public Kinematics
{
public:
    /**
     * @struct DHParameters
     * @brief Structure for Denavit-Hartenberg parameters of a joint.
     */
    struct DHParameters
    {
        double alpha; /**< Link twist angle. */
        double d; /**< Link offset. */
        double theta; /**< Joint angle. */
        double a; /**< Link length. */
        string dhType; /**< DH type (standard/modified). */
        bool modifiedDh; /**< Flag for modified DH convention. */

        /** @brief Default constructor for DHParameters. */
        DHParameters()
        {
            alpha = 0;
            d = 0;
            theta = 0;
            a = 0;
            dhType = "";
            modifiedDh = false;
        }

        /** @brief Parameterized constructor for DHParameters. */
        DHParameters(double alpha, double d, double theta, double a, const string &dhType, bool modifiedDh)
            : alpha(alpha), d(d), theta(theta), a(a), dhType(dhType), modifiedDh(modifiedDh) {}

        // Getters
        double getAlpha() const { return alpha; }
        double getD() const { return d; }
        double getTheta() const { return theta; }
        double getA() const { return a; }
        string getDhType() const { return dhType; }
        bool isModifiedDh() const { return modifiedDh; }

        // Setters
        void setAlpha(double alpha) { this->alpha = alpha; }
        void setD(double d) { this->d = d; }
        void setTheta(double theta) { this->theta = theta; }
        void setA(double a) { this->a = a; }
        void setDhType(const string &dhType) { this->dhType = dhType; }
        void setModifiedDh(bool modifiedDh) { this->modifiedDh = modifiedDh; }
    };

    /**
     * @struct RotationalValues
     * @brief Structure for rotational inertia values of a joint.
     */
    struct RotationalValues
    {
        double ixx; /**< Inertia XX. */
        double ixy; /**< Inertia XY. */
        double ixz; /**< Inertia XZ. */
        double iyy; /**< Inertia YY. */
        double iyz; /**< Inertia YZ. */
        double izz; /**< Inertia ZZ. */

        /** @brief Default constructor for RotationalValues. */
        RotationalValues()
        {
            ixx = 0;
            ixy = 0;
            ixz = 0;
            iyy = 0;
            iyz = 0;
            izz = 0;
        }

        /** @brief Parameterized constructor for RotationalValues. */
        RotationalValues(double ixx, double ixy, double ixz, double iyy, double iyz, double izz)
            : ixx(ixx), ixy(ixy), ixz(ixz), iyy(iyy), iyz(iyz), izz(izz) {}

        // Getters
        double getIxx() const { return ixx; }
        double getIxy() const { return ixy; }
        double getIxz() const { return ixz; }
        double getIyy() const { return iyy; }
        double getIyz() const { return iyz; }
        double getIzz() const { return izz; }

        // Setters
        void setIxx(double ixx) { this->ixx = ixx; }
        void setIxy(double ixy) { this->ixy = ixy; }
        void setIxz(double ixz) { this->ixz = ixz; }
        void setIyy(double iyy) { this->iyy = iyy; }
        void setIyz(double iyz) { this->iyz = iyz; }
        void setIzz(double izz) { this->izz = izz; }
    };

private:
    DHParameters dhParameters; /**< DH parameters for the joint. */
    RotationalValues rotationalValues; /**< Rotational inertia values for the joint. */
    string dhType; /**< DH type (standard/modified). */
    bool modifiedDh; /**< Flag for modified DH convention. */

public:
    /**
     * @brief Default constructor for JointKinematics.
     */
    JointKinematics()
    {
        dhParameters = DHParameters();
        rotationalValues = RotationalValues();
        dhType = "";
        modifiedDh = false;
    }

    /**
     * @brief Parameterized constructor for JointKinematics.
     * @param dhParameters DH parameters structure.
     * @param rotationalValues Rotational values structure.
     * @param dhType DH type string.
     * @param modifiedDh Flag for modified DH convention.
     */
    JointKinematics(const DHParameters &dhParameters, const RotationalValues &rotationalValues, const string &dhType, bool modifiedDh)
        : dhParameters(dhParameters),
          rotationalValues(rotationalValues),
          dhType(dhType),
          modifiedDh(modifiedDh) {}

    /**
     * @brief Calculate DH parameters for the joint (override).
     */
    void calculateDhParams() override
    {
        // Implementation
    }

    // Setters
    /** @brief Set the DH parameters structure. */
    void setDhParameters(const DHParameters &dhParameters) { this->dhParameters = dhParameters; }
    /** @brief Set the rotational values structure. */
    void setRotationalValues(const RotationalValues &rotationalValues) { this->rotationalValues = rotationalValues; }
    /** @brief Set the DH type string. */
    void setDhType(const string &dhType) { this->dhType = dhType; }
    /** @brief Set the modified DH flag. */
    void setModifiedDh(bool modifiedDh) { this->modifiedDh = modifiedDh; }

    // Getters
    /** @brief Get the DH parameters structure. */
    DHParameters getDhParameters() const { return dhParameters; }
    /** @brief Get the rotational values structure. */
    RotationalValues getRotationalValues() const { return rotationalValues; }
    /** @brief Get the DH type string. */
    string getDhType() const { return dhType; }
    /** @brief Get the modified DH flag. */
    bool isModifiedDh() const { return modifiedDh; }
};

/**
 * @class RobotKinematics
 * @brief Class for calculating kinematics of the robot as a whole.
 *
 * Inherits from Kinematics and provides an interface for robot-level kinematics calculations.
 */
class RobotKinematics : public Kinematics
{
public:
    /**
     * @brief Calculate DH parameters for the robot (override).
     */
    void calculateDhParams() override
    {
        // Implementation
    }
};

/**
 * @class ToolKinematics
 * @brief Class for calculating kinematics of the tool.
 *
 * Inherits from Kinematics and provides an interface for tool-level kinematics calculations.
 */
class ToolKinematics : public Kinematics
{
public:
    /**
     * @brief Calculate DH parameters for the tool (override).
     */
    void calculateDhParams() override
    {
        // Implementation
    }
};

#endif
