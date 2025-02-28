#include "robot.h"

using namespace std;

Robot::Robot(const std::string &name) 
: name(name), 
payload(0), 
footPrint(0), 
maxReach(0), 
repeatability(0), 
weight(0), 
dof(0) 
{

}

Robot::Robot(const string &name, const string &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
    : name(name),
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
