#include "robot.h"

Robot::Robot(int id, const QString &name, const QString &manufacturer, double payload, double footPrint, double maxReach, double repeatability, double weight, int dof)
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
