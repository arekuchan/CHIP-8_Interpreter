#ifndef _CHIP_8_PARSER
#define _CHIP_8_PARSER

#include <cstdint>
#include <cctype>
#include <unordered_map>
#include <vector>

#include <cstddef>
#include <string>
#include <ranges>

#include "display.hpp"
#include "control_flow.hpp"
#include "OpcodeType.hpp"
#include "ALU-Operations.hpp"

namespace Parser {
    const std::unordered_map<char, int> hexMap = {
            {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
            {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
            {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
            {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };

    std::string uint8ToHexDigits(std::uint8_t bitPttrn, bool includePrefix);

    std::string uint8ToHexString(std::uint8_t bitPttrn);

    std::string uint8ToHexDigitsOnly(std::uint8_t bitPttrn);

    std::string constr_opcode_str(std::vector<int8_t>& args);

    void truncate_prefix_zeros(std::string&);

    int8_t get_int_from_hex_char(const char);

    bool is_hex_char(const char);

    int8_t get_half_of_byte(std::byte&, bool);

    uint8_t extract_last_byte(std::vector<uint8_t>& args);

    void init_decoding_args(std::vector<int8_t&>&, Chip8Opcode);

    bool decode_and_execute(Chip8Opcode);
}

#endif
