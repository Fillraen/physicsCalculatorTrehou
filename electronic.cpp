#include "electronic.h"

electronic::electronic()
{
    electronics.insert(2000,"Air Conditioner (2,000W)");
    electronics.insert(9600,"Electric Vehicle Charger (9,600W)");
    electronics.insert(1500,"Baseboard Heater (1,500W)");
    electronics.insert(5000,"Garage Heater (5,000W)");
    electronics.insert(150,"Refrigerator (150W)");
    electronics.insert(100,"Deep Freezer (100W)");
    electronics.insert(1000,"Stove (1,000W)");
    electronics.insert(1200,"Dishwasher (1,200W)");
    electronics.insert(4000,"Clothes Dryer (4,000W)");
    electronics.insert(200,"Desktop Computer (200W)");
    electronics.insert(50,"Laptop Computer (50W)");
    electronics.insert(20,"LED TV (20W)");
    electronics.insert(90,"LCD TV (90W)");
    electronics.insert(260,"Plasma TV	(260W)");
    electronics.insert(7,"LED Light bulb (7W)");
    electronics.insert(16,"Fluorescent Light Bulb (16W)");
    electronics.insert(60,"Incandescent Light Bulb (60W)");
    electronics.insert(25,"Ceiling Fan (25W)");
    electronics.insert(10,"Table Fan (10W)");
    electronics.insert(0,"Other");
}
