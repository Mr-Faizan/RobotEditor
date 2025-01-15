#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <QString>

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
    virtual ~Kinematics() = default;
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

        DHParameters()
            : alpha(0.0), d(0.0), theta(0.0), a(0.0) {}
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
            : ixx(0.0), ixy(0.0), ixz(0.0), iyy(0.0), iyz(0.0), izz(0.0) {}
    };

private:
    DHParameters dhParameters;
    RotationalValues rotationalValues;
    QString dhType;
    bool modifiedDh;

public:
    JointKinematics();
    JointKinematics(const DHParameters &dhParameters, const RotationalValues &rotationalValues, const QString &dhType, bool modifiedDh);

    void calculateDhParams() override;

    // Setters
    void setDhParameters(const DHParameters &dhParameters) { this->dhParameters = dhParameters; }
    void setRotationalValues(const RotationalValues &rotationalValues) { this->rotationalValues = rotationalValues; }
    void setDhType(const QString &dhType) { this->dhType = dhType; }
    void setModifiedDh(bool modifiedDh) { this->modifiedDh = modifiedDh; }

    // Getters
    DHParameters getDhParameters() const { return dhParameters; }
    RotationalValues getRotationalValues() const { return rotationalValues; }
    QString getDhType() const { return dhType; }
    bool isModifiedDh() const { return modifiedDh; }
};

// This class will responsible for calculating the Kinematics of the Robot.
class RobotKinematics : public Kinematics
{
public:
    void calculateDhParams() override;
};

// This class will responsible for calculating the Kinematics of the Tool.
class ToolKinematics : public Kinematics
{

public:
    void calculateDhParams() override;
};

#endif
