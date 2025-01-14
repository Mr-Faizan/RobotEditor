#ifndef DYNAMICS_H
#define DYNAMICS_H

class Dynamics
{
public:
    virtual ~Dynamics() = default;
    virtual void calculateDynamics() = 0;
};

class JointDynamics : public Dynamics
{
public:
    void calculateDynamics() override;

};

#endif 
