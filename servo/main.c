/*
 * File:   main.c
 * Author: Andrew McOlash
 *
 * Created on March 10, 2018
 */

#include "defines.h"

// FUNCTION PROTOTYPES ---------------------------------------------------------
void gpio_init();
void delay_us(int);


// Generated with:
// var delays = [];
// for (var i = 0; i < 90; i++) {
//   delays[i] = Math.floor(800 + (i / 89) * 1200);
// }
const int high_delays[] = {
    800, 813, 826, 840, 853, 867, 880, 894, 907, 921, 934, 948, 961, 975, 988, 1002, 1015, 1029, 1042, 1056, 1069, 1083, 1096, 1110,
    1123, 1137, 1150, 1164, 1177, 1191, 1204, 1217, 1231, 1244, 1258, 1271, 1285, 1298, 1312, 1325, 1339, 1352, 1366, 1379, 1393, 1406,
    1420, 1433, 1447, 1460, 1474, 1487, 1501, 1514, 1528, 1541, 1555, 1568, 1582, 1595, 1608, 1622, 1635, 1649, 1662, 1676, 1689, 1703,
    1716, 1730, 1743, 1757, 1770, 1784, 1797, 1811, 1824, 1838, 1851, 1865, 1878, 1892, 1905, 1919, 1932, 1946, 1959, 1973, 1986, 2000
};

const int low_delays[] = {
    19200, 19187, 19174, 19160, 19147, 19133, 19120, 19106, 19093, 19079, 19066, 19052, 19039, 19025, 19012, 18998, 18985, 18971, 18958,
    18944, 18931, 18917, 18904, 18890, 18877, 18863, 18850, 18836, 18823, 18809, 18796, 18783, 18769, 18756, 18742, 18729, 18715, 18702,
    18688, 18675, 18661, 18648, 18634, 18621, 18607, 18594, 18580, 18567, 18553, 18540, 18526, 18513, 18499, 18486, 18472, 18459, 18445,
    18432, 18418, 18405, 18392, 18378, 18365, 18351, 18338, 18324, 18311, 18297, 18284, 18270, 18257, 18243, 18230, 18216, 18203, 18189,
    18176, 18162, 18149, 18135, 18122, 18108, 18095, 18081, 18068, 18054, 18041, 18027, 18014, 18000
};

// MAIN FUNCTION ---------------------------------------------------------------
void main() {
    int angle = 0; // between 0-90
    int inc = 1;
    
    gpio_init();
    
    while (1) {
        angle = 0;
        
        while (angle < 90) {
            angle += inc;
            
            RA0 = true;
            delay_us(high_delays[angle]);
            RA0 = false;
            delay_us(low_delays[angle]);
        }
        
        while (angle > 0) {
            angle -= inc;
            
            RA0 = true;
            delay_us(high_delays[angle]);
            RA0 = false;
            delay_us(low_delays[angle]);
        }
        
        
        for (angle = 100; angle > 0; angle--) {
            RA0 = true;
            __delay_us(800);
            RA0 = false;
            __delay_us(19200);
        }
        
//        delay = 800 + (1200 * angle) / (1200 * 90);
        
//        delay = 800;
//        
//        RA0 = true;
//        __delay_us(800);
//        RA0 = false;
//        __delay_us(19200);
        
//        for (i = 0; i < 100; i++) {
//            RA0 = true;
//            __delay_us(800);
//            RA0 = false;
//            __delay_us(19200);
//        }
//        
//        for (i = 0; i < 100; i++) {
//            RA0 = true;
//            __delay_us(2000);
//            RA0 = false;
//            __delay_us(18000);
//        }
    }
}

// FUNCTION IMPLEMENTATIONS ----------------------------------------------------
void gpio_init() {
    // Disable analog inputs, we want digital!
    ANSELAbits = 0;

    // Output
    RA0_TRIS = OUTPUT;
}

 void delay_us(int us) {
   while(us > 0) {
       __delay_us(5);
      us -= 5;
    }
 }