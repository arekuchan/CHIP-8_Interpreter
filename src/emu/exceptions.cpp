#include "exceptions.hpp"

InvalidStoredSpriteIdentifierException::InvalidStoredSpriteIdentifierException(std::uint8_t invalidIdentifier) noexcept {
    this->invalidIdentifier = Parser::uint8ToHexString(invalidIdentifier);
}

const char* InvalidStoredSpriteIdentifierException::what() const noexcept {
    return ("A stored sprite pattern was attempted to be accessed via invalid identifier: " + this->invalidIdentifier + "\n").c_str();
}

const char* OutOfChip8VmRamException::what() const noexcept {
    return ("The Chip 8 VM has ran out of ram\n");
}

RegisterOverflowException::RegisterOverflowException(std::string registerName) noexcept {
    this->registerName = registerName;
}

const char* RegisterOverflowException::what() const noexcept {
    return ("Register: " + this->registerName + "overflowed\n").c_str();
}

const char* RenderEngineWindowCreationException::what() const noexcept {
    return ("The render engine failed to create the emulator window\n");
}

const char* RenderEngineRendererCreationException::what() const noexcept {
    return ("The render engine failed to create the renderer\n");
}

SDLException::SDLException(const char* sdlErrMsg) noexcept {
    this->sdlErrMsg = sdlErrMsg;
}

std::string SDLException::name() const noexcept {
    return std::string("SDLException");
}

const char* SDLException::what() const noexcept {
    return ("A " + this->name() + "occurred with the following error: " + this->sdlErrMsg + "\n").c_str();
}

std::string SDLWaitException::name() const noexcept {
    return std::string("SDLWaitException");
}

std::string SDLLoadWavException::name() const noexcept {
    return std::string("SDLLoadWavException");
}

std::string SDLOpenAudioDevException::name() const noexcept {
    return std::string("SDLOpenAudioDevException");
}

std::string SDLQueueAudioException::name() const noexcept {
    return std::string("SDLQueueAudioException");
}

OpcodeException::OpcodeException(std::string opcodeName) noexcept {
    this->opcodeName = opcodeName;
}

const char* UnsupportedOpcodeException::what() const noexcept {
    return ("An unsupported opcode: " + this->opcodeName + "  was attempted to be exectued\n").c_str();;
}

const char* InvalidOpcodeException::what() const noexcept {
    return ("An invalid opcode: " + this->opcodeName + "  was attempted to be executed\n").c_str();
}
