#include "../headers/amd10_temperature.h"

CAmd10CpuSensor::CAmd10CpuSensor()
{

}

CAmd10CpuSensor::~CAmd10CpuSensor()
{

}

int CAmd10CpuSensor::Initialize()
{
    return Unsuccessful;
}

int CAmd10CpuSensor::Destroy()
{
    return Unsuccessful;
}

int CAmd10CpuSensor::Update()
{
    return Unsuccessful;
}

double *CAmd10CpuSensor::GetTemps()
{
    return 0;
}

QString CAmd10CpuSensor::GetCpuName()
{
    return "";
}

QString CAmd10CpuSensor::GetMicroArchitecture()
{
    return "";
}

int CAmd10CpuSensor::GetNumberOfCores()
{
    return 0;
}
