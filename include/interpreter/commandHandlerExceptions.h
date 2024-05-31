#ifndef INTERPRETER_COMMANDHANDLEREXCEPTIONS_H
#define INTERPRETER_COMMANDHANDLEREXCEPTIONS_H

namespace command {
    // Exceptions ///////////////////////////////////////////////////////////////

    struct InvalidToken {
        virtual const char* what();
    };

    struct InvalidName : public InvalidToken {
        const char* what();
    };

    struct InvalidNumeral : public InvalidToken {
        const char* what();
    };

    /////////////////////////////////////////////////////////////////////////////
};

#endif