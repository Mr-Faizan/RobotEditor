#include "robot.h"

using namespace std;

// Initialize the static member variable
int Robot::lastAssignedId = 1000;

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
      joints.push_back(Joint());
}

Robot::Robot(const string &name)
    : id(++lastAssignedId),
      name(name),
      manufacturer(""),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0)
{
      // Add a default joint
      joints.push_back(Joint());
}


Robot::Robot(const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
    : id(++lastAssignedId),
      name(name),
      manufacturer(manufacturer),
      payload(payload),
      footPrint(footPrint),
      maxReach(maxReach),
      repeatability(repeatability),
      weight(weight),
      dof(dof)
{
      // Add a default joint
      joints.push_back(Joint());
}

