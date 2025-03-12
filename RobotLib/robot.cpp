#include "robot.h"

using namespace std;

// Initialize the static member variable
int Robot::lastAssignedId = 1000;

Robot::Robot(const std::string &name)
    : id(++lastAssignedId),
      name(name),
      payload(0),
      footPrint(0),
      maxReach(0),
      repeatability(0),
      weight(0),
      dof(0)
{
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
}

Robot::~Robot()
{
}
