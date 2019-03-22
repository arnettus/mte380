#ifndef TERRAIN_H
#define TERRAIN_H

#include <Arduino.h>



struct Terrain {
    enum TerrainType {
        UNKN = 0,
        FLAT = 1,
        PIT = 2,
        GRAV = 3,
        SAND = 4,
        FOOD = 5,
        MIS1 = 6,
        MIS2 = 7,
        MIS3 = 8,
        CMPL = 9
    };

    static String GetTerrainString(TerrainType type) {
        switch(type) {
            case UNKN:
                return "UNKN";
            case FLAT:
                return "FLAT";
            case PIT:
                return "PIT";
            case GRAV:
                return "GRAV";
            case SAND:
                return "SAND";
            case FOOD:
                return "FOOD";
            case MIS1:
                return "MIS1";
            case MIS2:
                return "MIS2";
            case CMPL:
                return "CMPL";
            default:
                return "";
        }
    }

    TerrainType map [4][6][6] = {
        {
            {FLAT, FLAT, FLAT, GRAV, FLAT, FLAT},
            {FLAT, SAND, FLAT, FLAT, PIT , FLAT},
            {PIT , FLAT, FLAT, SAND, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, GRAV},
            {FLAT, GRAV, FLAT, FLAT, SAND, FLAT},
            {FLAT, FLAT, PIT , FLAT, FLAT, FLAT}
        },
        {
            {FLAT, FLAT, FLAT, PIT , FLAT, FLAT},
            {FLAT, GRAV, FLAT, FLAT, SAND, FLAT},
            {PIT , FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, SAND, FLAT, GRAV},
            {FLAT, SAND, FLAT, FLAT, PIT , FLAT},
            {FLAT, FLAT, GRAV, FLAT, FLAT, FLAT}
        },
        {
            {FLAT, FLAT, FLAT, PIT , FLAT, FLAT},
            {FLAT, SAND, FLAT, FLAT, GRAV, FLAT},
            {GRAV, FLAT, FLAT, FLAT, FLAT, FLAT},
            {FLAT, FLAT, SAND, FLAT, FLAT, PIT },
            {FLAT, PIT , FLAT, FLAT, SAND, FLAT},
            {FLAT, FLAT, GRAV, FLAT, FLAT, FLAT}
        },
        {
            {FLAT, FLAT, FLAT, GRAV, FLAT, FLAT},
            {FLAT, PIT , FLAT, FLAT, SAND, FLAT},
            {GRAV, FLAT, SAND, FLAT, FLAT, FLAT},
            {FLAT, FLAT, FLAT, FLAT, FLAT, PIT },
            {FLAT, SAND, FLAT, FLAT, GRAV, FLAT},
            {FLAT, FLAT, PIT , FLAT, FLAT, FLAT}
        }
    };
};

#endif // TERRAIN_H
