#ifndef _CHIP_8_CONFIG
#define _CHIP_8_CONFIG

#include <string>
#include <yaml-cpp/yaml.h>

namespace Config {
    const std::string configFilePath = "../config/config.yaml";

    const std::string resToken = "Chip-8-Display-Resolution";
    const std::string resWidthToken = "Width";
    const std::string resHeightToken = "Height";

    const std::string beepToken = "Chip-8-Beep-Sound";

    int get_res_field(const std::string_view);

    int get_res_height(void);

    int get_res_width(void);

    std::string get_beep_sound_path(void);
}

#endif 