#ifndef TERRAIN_H
#define TERRAIN_H

#include <unordered_map>

enum class TerrainType {
    UNKN,
    FLAT,
    WATE,
    GRAV,
    SAND,
    FOOD,
    MIS1,
    MIS2,
    MIS3,
    CMPL
};

struct TerrainParams {
    enum class DetectionType {
        IR,
        LIDAR
    };
    int min_val_;
    int max_val_;
    int planning_cost_;
    DetectionType detection_type_;

public:
    TerrainParams(DetectionType type, int min_val, int max_val);
    bool GetParams(DetectionType type, int& min_val, int& max_val);
    bool IsWithinRange(DetectionType type, int val);
};

class Terrain {
private:
    const std::unordered_map<TerrainType, TerrainParams> terrain_params_;
public:
    Terrain(std::unordered_map<TerrainType, TerrainParams>& terrain_params);
    bool GetLidarParams(TerrainType terrainType, int& min_lidar, int& max_lidar);
    bool GetIRParams(TerrainType terrainType, int& min_IR, int& max_IR);
    TerrainType GetTerrainType(TerrainParams::DetectionType detection_type, int val);
    const std::unordered_map<TerrainType, TerrainParams>& GetMapping();
};

#endif // TERRAIN_H