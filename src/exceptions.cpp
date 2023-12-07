#include "exceptions.hpp"

std::string int8ToHexString(std::int8_t bitPttrn) {
    std::stringstream stream;
    stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(bitPttrn);

    return stream.str();
}

InvalidStoredSpriteIdentifierException::InvalidStoredSpriteIdentifierException(std::int8_t invalidIdentifier) noexcept {
    this->invalidIdentifier = int8ToHexString(invalidIdentifier);
}

const char* InvalidStoredSpriteIdentifierException::what() const noexcept {
    return ("A stored sprite pattern was attempted to be accessed via invalid identifier: " + this->invalidIdentifier + "\n").c_str();
}

const char* OutOfChip8VmRamException::what() const noexcept {
    return ("The Chip 8 VM has ran out of ram\n");
}