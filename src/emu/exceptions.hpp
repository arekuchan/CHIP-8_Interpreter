#ifndef CHIP_8_EXCEPTIONS
#define CHIP_8_EXCEPTIONS

#include <iostream>
#include <exception>
#include <sstream>
#include <iomanip>
#include <cstdint>

std::string int8ToHexString(std::int8_t);

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

#endif