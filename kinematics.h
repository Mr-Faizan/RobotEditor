#ifndef KINEMATICS_H
#define KINEMATICS_H

class Kinematics
{
public:
    virtual ~Kinematics() = default;
    virtual void calculateDhParams() = 0;
};

class JointKinematics : public Kinematics
{
public:
    void calculateDhParams() override;
    
};

#endif 
