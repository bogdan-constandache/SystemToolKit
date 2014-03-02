#ifndef SENSOR_DEFINES_H
#define SENSOR_DEFINES_H

typedef enum _CHIP_
{
    Unknown_Chip = 0,

    ATK0110 = 1,

    F71858 = 2,
    F71862 = 3,
    F71869 = 4,
    F71869A = 5,
    F71882 = 6,
    F71889AD = 7,
    F71889ED = 8,
    F71889F = 9,
    F71808E = 10,

    IT8705F = 11,
    IT8712F = 12,
    IT8716F = 13,
    IT8718F = 14,
    IT8720F = 15,
    IT8721F = 16,
    IT8726F = 17,
    IT8728F = 18,
    IT8771E = 19,
    IT8772E = 20,

    NCT6771F = 21,
    NCT6776F = 22,
    NCT6779D = 23,
    NCT6791D = 24,

    W83627DHG = 25,
    W83627DHGP = 26,
    W83627EHF = 27,
    W83627HF = 28,
    W83627THF = 29,
    W83667HG = 30,
    W83667HGB = 31,
    W83687THF = 32

}Chip;

#endif // SENSOR_DEFINES_H
