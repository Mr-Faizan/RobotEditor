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

class Joint
{
private:

    std::string jointNumber;
    string name;
    double motionRangeMin;
    double motionRangeMax;
    double jointSpeedLimit;
    double frictionCoefficient;
    double stiffnessCoefficient;
    double dampingCoefficient;
    vector <pair<string, string>> visualizations;
    JointKinematics kinematics;
    vector<JointDynamics> dynamics; // Container for multiple JointDynamics objects
    int payloadCounter;

    std::array<double, 3> translation; // Translation vector (Tx, Ty, Tz)
    std::array<double, 3> rotation;    // Rotation vector (thetaX, thetaY, thetaZ)

public:

    Joint();
    Joint(string &name);

    Joint(string &name, double motionRangeMin, double motionRangeMax, double jointSpeedLimit, double frictionCoefficient, double stiffnessCoefficient, double dampingCoefficient, const string &visualization);
    ~Joint() = default;

    // Setters
    void setJointNumber(const string &jointNumber) { this->jointNumber = jointNumber; }
    void setName(const string &name) { this->name = name; }
    void setMotionRangeMin(double min) { this->motionRangeMin = min; }
    void setMotionRangeMax(double max) { this->motionRangeMax = max; }
    void setJointSpeedLimit(double speed) { this->jointSpeedLimit = speed; }
    void setFrictionCoefficient(double friction) { this->frictionCoefficient = friction; }
    void setStiffnessCoefficient(double stiffness) { this->stiffnessCoefficient = stiffness; }
    void setDampingCoefficient(double damping) { this->dampingCoefficient = damping; }
    void setKinematics(const JointKinematics &kinematics) { this->kinematics = kinematics; }
    void setTranslation(const std::array<double, 3> &t) { translation = t; }
    void setRotation(const std::array<double, 3> &r) { rotation = r; }
    void setVisualizations(const vector<pair<string, string>>& vis) { visualizations = vis; }

    // Getters
    string getJointNumber() const { return jointNumber; }
    string getName() const { return name; }
    double getMotionRangeMin() const { return motionRangeMin; }
    double getMotionRangeMax() const { return motionRangeMax; }
    double getJointSpeedLimit() const { return jointSpeedLimit; }
    double getFrictionCoefficient() const { return frictionCoefficient; }
    double getStiffnessCoefficient() const { return stiffnessCoefficient; }
    double getDampingCoefficient() const { return dampingCoefficient; }
    JointKinematics getKinematics() const { return kinematics; }
    vector<JointDynamics> getDynamics() const { return dynamics; }
    const array<double, 3>& getTranslation() const { return translation; }
    const array<double, 3>& getRotation() const { return rotation; }
    const vector<pair<string, string>>& getVisualizations() const { return visualizations; }

    void addVisualization(const string& filename, const string& filepath) { visualizations.emplace_back(filename, filepath); }
    void clearVisualizations() { visualizations.clear(); }


    // General Functions
    void addDynamics(JointDynamics &dynamics);

    JointDynamics& createAndAddDynamics();
    JointDynamics& setDynamics(JointDynamics &payload);
    JointDynamics& getDynamicsByPayloadNumber(const string &payloadNumber);
};

#endif // JOINT_H
