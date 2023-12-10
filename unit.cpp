#include "unit.h"

unit::unit()
{
    basicUnit.insert(18,"exa"); // E
    basicUnit.insert(15,"peta"); // P
    basicUnit.insert(12,"tera"); // T
    basicUnit.insert(9,"giga"); // G
    basicUnit.insert(6,"mega"); // M
    basicUnit.insert(3,"kilo"); // k
    basicUnit.insert(2,"hecto"); // h
    basicUnit.insert(1,"deka"); // da
    basicUnit.insert(0,""); //
    basicUnit.insert(-1,"deci"); // d
    basicUnit.insert(-2,"centi"); // c
    basicUnit.insert(-3,"milli"); // m
    basicUnit.insert(-6,"mikro"); // μ
    basicUnit.insert(-9,"nano"); // n
    basicUnit.insert(-12,"piko"); // p
    basicUnit.insert(-15,"femto"); // f
    basicUnit.insert(-18,"atto"); //a
    basicUnit.insert(-21,"zepto"); //z
    basicUnit.insert(-24,"yocto"); //y

    timeUnit.insert(1,"s");
    timeUnit.insert(30,"min");
    timeUnit.insert(60,"h");

    speedUnit.insert(1, "m/s");
    speedUnit.insert(2, "km/h");

    momentOfInertiaUnit.insert(1, "kg*m^2");
    momentOfInertiaUnit.insert(2, "lbs*ft^2");

    longTimeUnit.insert(1, "days");
    longTimeUnit.insert(2, "weeks");
    longTimeUnit.insert(3, "month");
    longTimeUnit.insert(4, "years");

    accelerationUnit.insert(1,"m/s^2");
    accelerationUnit.insert(2,"ft/s^2");

}


float unit::convertUnits(float value, const QString unit) {
    // Convert the value to SI units (meters, seconds, and meters per second)
    if (unit == "km/h") {
        value = (value * 1000) / 3600; // Convert to m/s
    } else if (unit == "min") {
        value = value * 60; // Convert to seconds
    } else if (unit == "h") {
        value = value * 3600; // Convert to seconds
    } else if (unit == "lbs*ft^2") {
        value = value * 0.04214; // Convert to kg*m^2
    } else if (unit == "days") {
        value = value / 30.44; // Convert days to months
    } else if (unit == "weeks") {
        value = value * 4.34; // Convert weeks to months
    } else if (unit == "years") {
        value = value * 12; // Convert years to months
    } else if (unit == "ft/s^2") {
        value = value * 0.3048; // Convert ft/s^2 to m/s^2
    } else {
        value = value; // Default value
    }

    // Convert value to SI units
    return value;
}






