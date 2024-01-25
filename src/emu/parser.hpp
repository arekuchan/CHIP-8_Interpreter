#ifndef _CHIP_8_PARSER
#define _CHIP_8_PARSER

#include <cstdint>
#include <cctype>
#include <unordered_map>
#include <vector>

#include <cstddef>

namespace Parser {
    const std::unordered_map<char, int> hexMap = {
            {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3},
            {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
            {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},
            {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}
    };

    int8_t get_int_from_hex_char(const char);

    bool is_hex_char(const char);

    int8_t get_half_of_byte(std::byte&, bool);

    void init_decoding_args(std::vector<int8_t&>&, Chip8Opcode);

    bool decode_and_execute_type_0(int8_t&, int8_t&, int8_t&);

    bool decode_and_execute_helper(std::vector<int8_t&>&);

    bool decode_and_execute(Chip8Opcode);
}

#endif
