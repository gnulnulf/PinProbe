//!
//! @file
//! @brief Casing for pinprobe prototype
//!
//! @version 2.0
//! @author Arco van Geest <arco@appeltaart.mine.nu>
//! @copyright 2021 Arco van Geest <arco@appeltaart.mine.nu> All right reserved.
//! @license  Creative Commons - Attribution - Share alike https://creativecommons.org/licenses/by-sa/4.0/

//! @date 20210607 Initial version


wall=1;
rotate([180,0,0]) difference() {
    cube([150+2*wall,20+2*wall,20+wall]);
    union() {
        translate([wall,wall,-1])  cube([150,20,20+1]);
        translate([-1,-1,10])  cube([10,30,20]);
        translate([-1,-1,-1])  cube([10,30,6+1+10]);
        translate([60+wall,8,8]) rotate([90,0,0]) cylinder(d=9,h=20);

        translate([98+wall,8,4]) rotate([90,0,0]) cylinder(d=4,h=20);
        translate([98+wall-3,-1,4-3]) cube([6,6,6]);
        translate([98+10+wall,8,4]) rotate([90,0,0]) cylinder(d=4,h=20);
        translate([98+10+wall-3,-1,4-3]) cube([6,6,6]);
        translate([98+20+wall,8,4]) rotate([90,0,0]) cylinder(d=4,h=20);
        translate([98+20+wall-3,-1,4-3]) cube([6,6,6]);
        translate([98+31+wall,8,4]) rotate([90,0,0]) cylinder(d=4,h=20);
        translate([98+31+wall-3,-1,4-3]) cube([6,6,6]);

        translate([72+wall,4+wall,0]) cube([26,10,100]);

        translate([98+15+wall,6,4])  cylinder(d=4,h=20);
        translate([98+22+wall,5,4])  cylinder(d=4,h=20);
        translate([98+22+wall,9,4])  cylinder(d=4,h=20);
        translate([98+22+wall,13,4])  cylinder(d=4,h=20);

        translate([150,10+wall-5,-1]) cube([10,10,6]);

        
    } //union
} // difference

//translate([0,30,0]) cube([150+2*wall,20+2*wall,wall]);

