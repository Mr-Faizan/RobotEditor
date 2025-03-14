#ifndef JOINT_H
#define JOINT_H

#include <string>
#include <vector>
#include "dynamics.h"
#include "kinematics.h"

using namespace std;

class Joint
{
private:
    string name;
    double motionRangeMin;
    double motionRangeMax;
    double jointSpeedLimit;
    double frictionCoefficient;
    double stiffnessCoefficient;
    double dampingCoefficient;
    string visualization;
    JointKinematics kinematics;
    vector<JointDynamics> dynamics; // Container for multiple JointDynamics objects

public:
    Joint()
        : name(""), motionRangeMin(0), motionRangeMax(0), jointSpeedLimit(0), frictionCoefficient(0), stiffnessCoefficient(0), dampingCoefficient(0), visualization("")
    {
        // Add a default JointDynamics object
        dynamics.push_back(JointDynamics());
    }
    Joint(const string &name) 
    : name(name), motionRangeMin(0), motionRangeMax(0), jointSpeedLimit(0), frictionCoefficient(0), stiffnessCoefficient(0), dampingCoefficient(0), visualization("") 
    {
        // Add a default JointDynamics object
        dynamics.push_back(JointDynamics());
    }
    
    Joint(const string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization);
    ~Joint() = default;

    // Setters
    void setName(const string &name) { this->name = name; }
    void setMotionRangeMin(double min) { this->motionRangeMin = min; }
    void setMotionRangeMax(double max) { this->motionRangeMax = max; }
    void setJointSpeedLimit(double speed) { this->jointSpeedLimit = speed; }
    void setFrictionCoefficient(double friction) { this->frictionCoefficient = friction; }
    void setStiffnessCoefficient(double stiffness) { this->stiffnessCoefficient = stiffness; }
    void setDampingCoefficient(double damping) { this->dampingCoefficient = damping; }
    void setVisualization(const string &visualization) { this->visualization = visualization; }
    void setKinematics(const JointKinematics &kinematics) { this->kinematics = kinematics; }
    void addDynamics(const JointDynamics &dynamics) { this->dynamics.push_back(dynamics); }

    // Getters
    string getName() const { return name; }
    double getMotionRangeMin() const { return motionRangeMin; }
    double getMotionRangeMax() const { return motionRangeMax; }
    double getJointSpeedLimit() const { return jointSpeedLimit; }
    double getFrictionCoefficient() const { return frictionCoefficient; }
    double getStiffnessCoefficient() const { return stiffnessCoefficient; }
    double getDampingCoefficient() const { return dampingCoefficient; }
    string getVisualization() const { return visualization; }
    JointKinematics getKinematics() const { return kinematics; }
    vector<JointDynamics> getDynamics() const { return dynamics; }
};

#endif // JOINT_H
