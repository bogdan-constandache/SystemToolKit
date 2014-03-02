#ifndef _ISENSOR_
#define _ISENSOR_

class ISensor
{
public:
    virtual ~ISensor() {}
    virtual int Initialize() = 0;
    virtual int Destroy() = 0;
    virtual int Update() = 0;
    virtual double* GetTemps() = 0;
    virtual double* GetFanSpeeds() = 0;
    virtual double* GetVoltages() = 0;
};

#endif
