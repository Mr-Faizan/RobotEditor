#ifndef JOINT_H
#define JOINT_H

#include <QString>
#include <QVector>
#include "dynamics.h"
#include "kinematics.h"
#include "visualization.h"

class Joint
{
private:
    QString name;
    double motionRangeMin;
    double motionRangeMax;
    double jointSpeedLimit;
    double frictionCoefficient;
    double stiffnessCoefficient;
    double dampingCoefficient;

    QVector<JointDynamics> dynamics; // Container for multiple JointDynamics objects
    Kinematics *kinematics;
    Visualization *visualization;

public:
    Joint() = default;
    Joint(const QString &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient);
    ~Joint();

    // Setters
    void setName(const QString &name) { this->name = name; }
    void setMotionRangeMin(double min) { this->motionRangeMin = min; }
    void setMotionRangeMax(double max) { this->motionRangeMax = max; }
    void setJointSpeedLimit(double speed) { this->jointSpeedLimit = speed; }
    void setFrictionCoefficient(double friction) { this->frictionCoefficient = friction; }
    void setStiffnessCoefficient(double stiffness) { this->stiffnessCoefficient = stiffness; }
    void setDampingCoefficient(double damping) { this->dampingCoefficient = damping; }

    // Getters
    QString getName() const { return name; }
    double getMotionRangeMin() const { return motionRangeMin; }
    double getMotionRangeMax() const { return motionRangeMax; }
    double getJointSpeedLimit() const { return jointSpeedLimit; }
    double getFrictionCoefficient() const { return frictionCoefficient; }
    double getStiffnessCoefficient() const { return stiffnessCoefficient; }
    double getDampingCoefficient() const { return dampingCoefficient; }
};

#endif // JOINT_H
