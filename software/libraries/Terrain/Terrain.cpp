#include "Terrain.h"

TerrainParams::TerrainParams(DetectionType detection_type, int min_val, int max_val) :
    detection_type_(detection_type),
    min_val_(min_val),
    max_val_(max_val) {}

bool TerrainParams::GetParams(DetectionType detection_type, int& min_val, int& max_val) {
    if (detection_type != detection_type_) {
        return false;
    }

    min_val = min_val_;
    max_val = max_val_;
    return true;
}

bool TerrainParams::IsWithinRange(DetectionType detection_type, int val) {
    if (detection_type != detection_type_) {
        return false;
    }

    return (val >= min_val_ && val <= max_val_);
}

Terrain::Terrain(std::unordered_map<TerrainType, TerrainParams>& terrain_params) : 
    terrain_params_(terrain_params) {}

bool Terrain::GetLidarParams(TerrainType terrain_type, int& min_lidar, int& max_lidar) {
    if (terrain_params_.find(terrain_type) == terrain_params_.end()) {
        return false;
    }

    TerrainParams::DetectionType detection_type = TerrainParams::DetectionType::LIDAR;
    return terrain_params_[terrain_type].GetParams(detection_type, min_lidar, max_lidar);
}

bool Terrain::GetIRParams(TerrainType terrain_type, int& min_IR, int& max_IR) {
    if (terrain_params_.find(terrain_type) == terrain_params_.end()) {
        return false;
    }
    
    TerrainParams::DetectionType detection_type = TerrainParams::DetectionType::LIDAR;
    return terrain_params_[terrain_type].GetParams(detection_type, min_IR, max_IR);
}

TerrainType Terrain::GetTerrainType(TerrainParams::DetectionType detection_type, int val) {
    for (auto& param : terrain_params_) {
        if (param.second.IsWithinRange(detection_type, val)) {
            return param.first;
        }
    }

    return TerrainType::UNKN;
}

const std::unordered_map<TerrainType, TerrainParams>& Terrain::GetMapping() {
    return terrain_params_;
}
