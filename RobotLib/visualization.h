#ifndef VISUALIZATION_H
#define VISUALIZATION_H

/*
Implementation Structure.
Visualization can be for the Robot or for the Joints or for the Tools.
So I will implement the concept of Inheritance for the Visualization.
As Visualization is the base class and JointVisualization is the derived class.
And I will also implement the concept of Abrstraction and Polymorphism for the Visualization.
As Visualization is the abstract class and it will contain the common functionalities for the Visualization.
And JointVisualization, ToolVisualization, and RobotVisualization will implement the common functionalities of the Visualization.
And Also can override the functionalities according to their requirements.

*/

// Compare this snippet from kinematics.h:

// This class will responsible for handling the Visualization functionality that is common in Robot, Joint, and Tool.
class Visualization
{
public:
    virtual ~Visualization() = default;
    virtual void addVisualization() = 0;
};

// This class will responsible for adding the Visualization of the Robot.
class RobotVisualization : public Visualization
{
public:
    void addVisualization() override;
};

// This class will responsible for adding the Visualization of the Joint.
class JointVisualization : public Visualization
{
public:
    void addVisualization() override;
    
};

// This class will responsible for adding the Visualization of the Tool.
class ToolVisualization : public Visualization 
{ 
public:
    void addVisualization() override;
};


#endif 
