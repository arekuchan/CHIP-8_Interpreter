#include "parser.hpp"

namespace Parser {
    int8_t get_int_from_hex_char(const char hexChar) {
        char upperHexChar = std::toupper(hexChar);
        auto mapPair = hexMap.find(upperHexChar);

        if (mapPair != hexMap.end()) {
            return mapPair->second;
        }

        return -1;
    }

    bool is_hex_char(const char toCheck) {
        char upper = std::toupper(toCheck);
        auto mapPair = hexMap.find(upper);

        return mapPair != hexMap.end() ? true : false;
    }

    int8_t get_half_of_byte(std::byte& fullByte, bool leftHalf) {
	if (leftHalf) {
	    return (fullByte >> 4) & 0xF;
	} else {
	    return fullByte & 0xF;
	}	
    }

    void init_decoding_args(std::vector<int8_t&>& args, Chip8Opcode opcode) {
    	bool getLeftHalf = true;

	for (int i : std::views::iota(args.size())) {
	    // divide opcode into continuous 4 bit sections
	    // each 4 bits is an arg

	    if (i < 2) {
		args[i] = get_half_of_byte(opcode[0], getLeftHalf);
	    } else {
		args[i] = get_half_of_byte(opcode[1], getLeftHalf);
	    }

	    getLeftHalf = !getLeftHalf; // left right left right
	}
    }
    
    bool decode_and_execute_type_0(int8_t& arg1, int8_t& arg2, int8_t& arg3) {
	// TODO : finish this
    }

    // args must be init'd
    bool decode_and_execute_helper(std::vector<int8_t&>& args) {
	// TODO : finish this
	switch (args[0]) {
	    case 0x0:
		break;
	    case 0x1:
		break;
	    case 0x2:
		break;
	    case 0x3:
		break;
	    case 0x4:
		break;
	    case 0x5:
		break;
	    case 0x6:
		break;
	    case 0x7:
		break;
	    case 0x8:
		break;
	    case 0x9:
		break;
	    case 0xA:
		break;
	    case 0xB:
		break;
	    case 0xC:
		break;
	    case 0xD:
		break;
	    case 0xE:
		break;
	    case 0xF:
		break;
	    case default:
		// TODO: some exception
		break;
    }

    bool decode_and_execute(Chip8Opcode opcode) {
	int8_t arg0, arg1, arg2, arg3;
	std::vector<int8_t&> args = {arg0, arg1, arg2, arg3};

	// for all helper functions, the following
	// convention will be followed
	// using big-endian
	// arg0: first 4 bits, defines type of opcode
	// arg1, arg2, arg3: the second, third and fourth set of 4 bits, respectively

	init_decoding_args(args, opcode);

	return decode_and_execute_helper(args);
    }
}
