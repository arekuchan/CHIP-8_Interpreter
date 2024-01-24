#include "config.hpp"

namespace Config {
    // yes I know, file reading bad, cache them
    // but these functions only get called like once
    int get_res_field(const std::string field) {
        YAML::Node config = YAML::LoadFile(configFilePath);

        return config[resToken][field].as<int>();
    }

    int get_res_height() {
        return Config::get_res_field(resHeightToken);
    }

    int get_res_width() {
        return Config::get_res_field(resWidthToken);
    }

    std::string get_beep_sound_path() {
        return YAML::LoadFile(configFilePath)[beepToken].as<std::string>();
    }
}