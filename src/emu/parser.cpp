#include "parser.hpp"

namespace Parser {
    std::string uint8ToHexDigits(std::uint8_t bitPttrn, bool includePrefix) {
        std::stringstream stream;

        if (includePrefix) {
            stream << "0x";
        }

        stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bitPttrn);

        return stream.str();
    }
    
    std::string uint8ToHexDigitsOnly(std::uint8_t bitPttrn) {
        return uint8ToHexDigits(bitPttrn, false);
    }

    // returns w/ 0x prefix
    std::string uint8ToHexString(std::uint8_t bitPttrn) {
        return uint8ToHexDigits(bitPttrn, true);
    }

    void truncate_prefix_zeros(std::string& str) {
        for (int i : std::views::iota(0, static_cast<int>(str.length())) ) {
            if (str[i] != '0') {
                str.erase(0, i);
            }
        }
    }

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
	        return (static_cast<uint8_t>(fullByte) >> 4) & 0x0F;
	    } else {
	        return static_cast<uint8_t>(fullByte) & 0x0F;
	    }	
    }

    void init_decoding_args(std::vector<uint8_t>& args, Chip8Opcode opcode) {
    	bool getLeftHalf = true;
        int numArgs = opcodeSize * 2; // 2 args in a byte

	    for (int i : std::views::iota(0, numArgs)) {
	        // divide opcode into continuous 4 bit sections
	        // each 4 bits is an arg

	        if (i < 2) {
	    	args.push_back(get_half_of_byte(opcode[0], getLeftHalf));
	        } else {
	    	args.push_back(get_half_of_byte(opcode[1], getLeftHalf));
	        }

	        getLeftHalf = !getLeftHalf; // left right left right
	    }
    }

    std::string constr_opcode_str(std::vector<uint8_t>& args) {
        std::string opcodeStr;
        opcodeStr.reserve(args.size());

        for (auto& arg : args) {
            std::string hexArg = uint8ToHexDigitsOnly(arg);
            truncate_prefix_zeros(hexArg);

            opcodeStr.append(hexArg);
        }

        return opcodeStr;
    }

    bool decode_and_execute_type_0_arg1_is_0(uint8_t& arg2, uint8_t& arg3) {
        switch (arg3) {
            case 0x0:
                return DisplayOpcodes::disp_clear_00E0();
            case 0xE:
                return ControlFlowOps::ret_00EE();
            default:
                std::vector<uint8_t> args = {0, 0, arg2, arg3};
                std::string opcodeStr = constr_opcode_str(args);

                throw InvalidOpcodeException(opcodeStr);
        }
    }
    
    bool decode_and_execute_type_0(std::vector<uint8_t>& args) {
	    switch (args[1]) {
            case 0x0:
                return decode_and_execute_type_0_arg1_is_0(args[2], args[3]);
            default: // 0NNN
                std::string opcodeStr = constr_opcode_str(args);
                throw UnsupportedOpcodeException(opcodeStr);
        }
    }

    uint16_t concat_args_excl_first(std::vector<uint8_t>& args) {
        uint16_t result = 0;
        unsigned int numActualArgs = args.size() - 1; // excl args[0] which is opcode type

        // think of each arg as a list of bits of opcodeArgBitSize
        // goal: concat these lists, prepend zeros and store in result
        // divide result into opcodeArgBitSize sections
        // numActualArgs - i tells you what section is should be in (section 0 is on right most side)
        // multiplying by opcodeArgBitSize shifts it to the right offset

        for (int i : std::views::iota(1, static_cast<int>(numActualArgs) + 1)) {
            int shiftAmt = (numActualArgs - i) * opcodeArgBitSize;

            result |= static_cast<uint16_t>(args[i]) << shiftAmt; 
        }

        return result;
    }

    uint8_t extract_last_byte(std::vector<uint8_t>& args) {
        uint8_t result = 0;
        unsigned int numArgs = args.size();

        result |= args[numArgs - 1];
        result |= (args[numArgs - 2] << opcodeArgBitSize);

        return result;
    }

    bool decode_and_execute_type_1(std::vector<uint8_t>& args) {
        uint16_t addr = concat_args_excl_first(args);

        return ControlFlowOps::jmp_1nnn(addr);
    }

    bool decode_and_execute_type_2(std::vector<uint8_t>& args) {
        uint16_t addr = concat_args_excl_first(args);

        return ControlFlowOps::call_2nnn(addr);
    }

    bool decode_and_execute_type_3(std::vector<uint8_t>& args) {
        // TODO: find out how to unpack these like python tuple
        // for mitigating code duplication
        uint8_t val = extract_last_byte(args);
        uint8_t& vXID = args[1];

        return ControlFlowOps::se_3xkk(varRegID, val);
    }

    bool decode_and_execute_type_4(std::vector<uint8_t>& args) {
        uint8_t val = extract_last_byte(args);
        uint8_t& vXID = args[1];

        return ControlFlowOps::sne_4xkk(varRegID, val);
    }

    bool decode_and_execute_type_5(std::vector<uint8_t>& args) {
        uint8_t& vXID = args[1];
        uint8_t& vYID = args[2];

        return ControlFlowOps::se_5xy0(vXID, vYID);
    }

    bool decode_and_execute_type_6(std::vector<uint8_t>& args) {
        uint8_t val = extract_last_byte(args);
        uint8_t& vXID = args[1];

        return ALU_Ops::ldi_6XNN(vXID, val);

    }

    bool decode_and_execute_type_7(std::vector<uint8_t>& args) {
        uint8_t val = extract_last_byte(args);
        uint8_t& vXID = args[1];

        return ALU_Ops::addi_7XNN(vXID, val);
    }

    bool decode_and_execute_type_8_helper(uint8_t& vXID, uint8_t& vYID,uint8_t& lastArg, std::string opcodeName) {
        switch(lastArg) {
            case 0x0;
                return ALU_Ops::assign_8XY0(vXID, vYID);
            case 0x1:
                return ALU_Ops::or_8XY1(vXID, vYID);
            case 0x2:
                return ALU_Ops::and_8XY2(vXID, vYID);
            case 0x3:
                return ALU_Ops::xor_8XY3(vXID, vYID);
            case 0x4:
                return ALU_Ops::add_8XY4(vXID, vYID);
            case 0x5:
                return ALU_Ops::minus_8XY5(vXID, vYID);
            case 0x6:
                return ALU_Ops::shr_8XY6(vXID, vYID);
            case 0x7:
                return ALU_Ops::subn_8XY7(vXID, vYID);
            case 0xE:
                return ALU_Ops::shl_8XYE(vXID, vYID);
            default:
                throw InvalidOpcodeException(opcodeName);
        }
    }

    bool decode_and_execute_type_8(std::vector<uint8_t>& args) {
        uint8_t& vXID = args[1];
        uint8_t& vYID = args[2];

        std::string opcodeName = constr_opcode_str(args);

        return decode_and_execute_type_8_helper(vXID, vYID, args[args.size() - 1], opcodeName);
    }

    // args must be init'd
    bool decode_and_execute_helper(std::vector<uint8_t>& args) {
	    switch (args[0]) {
	        case 0x0:
	    	    return decode_and_execute_type_0(args);
	        case 0x1:
                return decode_and_execute_type_1(args);
	        case 0x2:
                return decode_and_execute_type_2(args);
	        case 0x3:
                return decode_and_exexute_type_3(args);
	        case 0x4:
	    	    return decode_and_execute_type_4(args);
	        case 0x5:
                return decode_and_execute_type_5(args);
	        case 0x6:
                return decode_and_execute_type_6(args);
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
	        default:
                std::string opcodeStr = constr_opcode_str(args);
                throw InvalidOpcodeException(opcodeStr);
        }

        return false; // for compiler, shouldn't reach here once function is done being written
    }

    bool decode_and_execute(Chip8Opcode opcode) {
	    std::vector<uint8_t> args;

	    // for all helper functions, the following
	    // convention will be followed
	    // using big-endian
	    // arg0: first 4 bits, defines type of opcode
	    // arg1, arg2, arg3: the second, third and fourth set of 4 bits, respectively

	    init_decoding_args(args, opcode);

	    return decode_and_execute_helper(args);
    }
}
