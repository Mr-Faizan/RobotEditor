#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <string>

using namespace std;

/*
Implementation Structure.
Kinematics can be for the Robot or for the Joints or for the Tools.
So I will implement the concept of Inheritance for the Kinematics.
As Kinematics is the base class and JointKinematics is the derived class.
And I will also implement the concept of Abrstraction and Polymorphism for the Kinematics.
As Kinematics is the abstract class and it will contain the common functionalities for the Kinematics.
And JointKinematics, ToolKinematics, and RobotKinematics will implement the common functionalities of the Kinematics.
And Also can override the functionalities according to their requirements.

*/

// This class will responsible for handling the functionality that is common in Robot, Joint, and Tool.
class Kinematics
{
public:
    virtual void calculateDhParams() = 0;
};

// This class will responsible for calculating the Kinematics of the Joint.
class JointKinematics : public Kinematics
{
public:
    // DH Parameters Structure
    struct DHParameters
    {
        double alpha;
        double d;
        double theta;
        double a;
        string dhType;
        bool modifiedDh;

        DHParameters()
        {
            alpha = 0;
            d = 0;
            theta = 0;
            a = 0;
            dhType = "";
            modifiedDh = false;
        }

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

    // Rotational Values Structure
    struct RotationalValues
    {
        double ixx;
        double ixy;
        double ixz;
        double iyy;
        double iyz;
        double izz;

        RotationalValues()
        {
            ixx = 0;
            ixy = 0;
            ixz = 0;
            iyy = 0;
            iyz = 0;
            izz = 0;
        }

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
    DHParameters dhParameters;
    RotationalValues rotationalValues;
    string dhType;
    bool modifiedDh;

public:
    JointKinematics()
    {
        dhParameters = DHParameters();
        rotationalValues = RotationalValues();
        dhType = "";
        modifiedDh = false;
    }

    JointKinematics(const DHParameters &dhParameters, const RotationalValues &rotationalValues, const string &dhType, bool modifiedDh)
        : dhParameters(dhParameters),
          rotationalValues(rotationalValues),
          dhType(dhType),
          modifiedDh(modifiedDh) {}

    void calculateDhParams() override
    {
        // Implementation
    }

    // Setters
    void setDhParameters(const DHParameters &dhParameters) { this->dhParameters = dhParameters; }
    void setRotationalValues(const RotationalValues &rotationalValues) { this->rotationalValues = rotationalValues; }
    void setDhType(const string &dhType) { this->dhType = dhType; }
    void setModifiedDh(bool modifiedDh) { this->modifiedDh = modifiedDh; }

    // Getters
    DHParameters getDhParameters() const { return dhParameters; }
    RotationalValues getRotationalValues() const { return rotationalValues; }
    string getDhType() const { return dhType; }
    bool isModifiedDh() const { return modifiedDh; }
};

// This class will responsible for calculating the Kinematics of the Robot.
class RobotKinematics : public Kinematics
{
public:
    void calculateDhParams() override
    {
        // Implementation
    }
};

// This class will responsible for calculating the Kinematics of the Tool.
class ToolKinematics : public Kinematics
{

public:
    void calculateDhParams() override
    {
        // Implementation
    }
};

#endif
