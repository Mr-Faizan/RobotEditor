#ifndef ROBOT_H
#define ROBOT_H

#include <string>

using namespace std;

/*
Implementation Structure.
Robot is the Main Entity of the application.
It is composed of Joints, which are the main components of the robot.
Joints are composed of Dynamics, Kinematics and Visualization.
So I will implement the concept of Aggregation for the Robot and Joints.
As Joints are the part of the Robot, but they can exist independently.

However, I will implement the concept of Composition for the Robot and Tools.
As Tools are independent of the Robot and it can attach or detach from the Robot.

*/
class Robot
{

private:
    int id; // Unique ID for each Robot.
    string name;
    string manufacturer;
    double payload;
    double footPrint;
    double maxReach;
    double repeatability;
    double weight;
    int dof;

public:
    Robot() = default;

    // Implement Constructor Overloading for easy initialization of the Robot.
    Robot(int id, const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof);

    // Implement Destructor for the Robot.
    ~Robot();

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const string &name) { this->name = name; }
    void setManufacturer(const string &manufacturer) { this->manufacturer = manufacturer; }
    void setPayload(double payload) { this->payload = payload; }
    void setFootPrint(double footPrint) { this->footPrint = footPrint; }
    void setMaxReach(double maxReach) { this->maxReach = maxReach; }
    void setRepeatability(double repeatability) { this->repeatability = repeatability; }
    void setWeight(double weight) { this->weight = weight; }
    void setDof(int dof) { this->dof = dof; }

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getManufacturer() const { return manufacturer; }
    double getPayload() const { return payload; }
    double getFootPrint() const { return footPrint; }
    double getMaxReach() const { return maxReach; }
    double getRepeatability() const { return repeatability; }
    double getWeight() const { return weight; }
    int getDof() const { return dof; }
};

#endif // ROBOT_H
