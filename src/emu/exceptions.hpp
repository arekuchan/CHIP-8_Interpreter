#ifndef CHIP_8_EXCEPTIONS
#define CHIP_8_EXCEPTIONS

#include <iostream>
#include <exception>
#include <sstream>
#include <iomanip>

#include <cstdint>
#include <typeinfo>


namespace Parser {
    std::string int8ToHexString(std::int8_t);
}

class InvalidStoredSpriteIdentifierException : public std::exception {
    private:
        std::string invalidIdentifier;

    public:
        InvalidStoredSpriteIdentifierException(std::int8_t) noexcept;

        virtual const char* what() const noexcept; 
};

class OutOfChip8VmRamException : public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class RegisterOverflowException : public std::exception {
    private:
        std::string registerName;

    public:
        RegisterOverflowException(std::string) noexcept;

        virtual const char* what() const noexcept;
};

class RenderEngineWindowCreationException : public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class RenderEngineRendererCreationException : public std::exception {
    public:
        virtual const char* what() const noexcept;
};

class SDLException : public std::exception {
    protected:
        const char* sdlErrMsg;

    public:
        SDLException(const char*) noexcept;

        virtual const char* what() const noexcept;

        virtual std::string name() const noexcept;
};

class SDLWaitException : public SDLException {
    using SDLException::SDLException;

    public:
        std::string name() const noexcept;
};

class SDLLoadWavException : public SDLException {
    using SDLException::SDLException;
    
    public:
        std::string name() const noexcept;
};

class SDLOpenAudioDevException : public SDLException {
    using SDLException::SDLException;

    public:
        std::string name() const noexcept;
};

class SDLQueueAudioException : public SDLException {
    using SDLException::SDLException;

    public:
        std::string name() const noexcept;
};

class OpcodeException : public std::exception {
    protected:
        std::string opcodeName;

    public:
        OpcodeException(std::string opcodeName) noexcept;

        virtual const char* what() const noexcept = 0;
};

class UnsupportedOpcodeException : public OpcodeException {
    using OpcodeException::OpcodeException;

    public:
        virtual const char* what() const noexcept;
};

class InvalidOpcodeException : public OpcodeException {
    using OpcodeException::OpcodeException;

    public:
        virtual const char* what() const noexcept;
};

#endif
