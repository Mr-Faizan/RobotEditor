#include "robot.h"
#include <string>

using namespace std;


Robot::Robot(int id, const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
    : id(id),
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
