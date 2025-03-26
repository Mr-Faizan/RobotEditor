#ifndef ROBOTLIB_TEST_H
#define ROBOTLIB_TEST_H

#include "robotlib.h"
#include "robot.h"
#include <iostream>

class RobotLibTest {
public:
    void runTests();

private:
    void testAddRobot();
    void testRemoveRobot();
    void testGetRobot();
    void testGetRobotById();
    void testLoadFromJson();
    void testSaveToJson();
    void testUpdateAndSaveRobotData();
};

#endif // ROBOTLIB_TEST_H
