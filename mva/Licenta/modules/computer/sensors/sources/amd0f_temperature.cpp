#include "../headers/amd0f_temperature.h"

CAmd0FCpuSensor::CAmd0FCpuSensor()
{

}

CAmd0FCpuSensor::~CAmd0FCpuSensor()
{

}

int CAmd0FCpuSensor::Initialize()
{
    return Unsuccessful;
}

int CAmd0FCpuSensor::Destroy()
{
    return Unsuccessful;
}

int CAmd0FCpuSensor::Update()
{
    return Unsuccessful;
}

double *CAmd0FCpuSensor::GetTemps()
{
    return 0;
}

QString CAmd0FCpuSensor::GetCpuName()
{
    return "";
}

QString CAmd0FCpuSensor::GetMicroArchitecture()
{
    return "";
}

int CAmd0FCpuSensor::GetNumberOfCores()
{
    return 0;
}
