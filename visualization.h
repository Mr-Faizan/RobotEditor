#ifndef VISUALIZATION_H
#define VISUALIZATION_H

class Visualization
{
public:
    virtual ~Visualization() = default;
    virtual void addVisualization() = 0;
};

class JointVisualization : public Visualization
{
public:
    void addVisualization() override;
    
};

#endif 
