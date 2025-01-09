#ifndef CI_COMMAND_TYPE_H
#define CI_COMMAND_TYPE_H

/**
 * @brief An enum representing the types of commands that can be executed by the
 * interpreter.
 */
typedef enum {
    // add x0 x1 x2
    // add x0 x1 5
    // Can either be variable variable variable or variable variable number
    CMD_ADD,

    // and x0 x1 x2
    // Is always variable variable variable
    CMD_AND,

    // asr x0 x1 1
    // Is always variable variable number
    CMD_ASR,

    // b.ge l1
    // b.lt l2
    // Always in the form of b.cond label_name
    // See token_type.h for valid condition tokens
    CMD_BRANCH,

    // call foo
    // Always followed by a label
    CMD_CALL,

    // cmp x1 x2
    // cmp x1 0b10101101
    // Can either be variable variable or variable number
    CMD_CMP,

    // unsigned variant of CMD_CMP
    CMD_CMP_U,

    // Shouldn't ever be seen
    CMD_ERR,

    // eor x0 x1 x2
    // Is always variable variable variable
    CMD_EOR,

    // load x0 4 100
    // load x8 1 x0
    // Can either be variable number number or variable number variable
    CMD_LOAD,

    // lsl x0 x1 1
    // Is always variable variable number
    CMD_LSL,

    // lsr x0 x1 1
    // Is always variable variable number
    CMD_LSR,

    // mov x0 5
    // mov x1 0b1010101
    // mov x2 0xdeadbeef
    // Must always be variable number
    CMD_MOV,

    // orr x0 x1 x2
    // Is always variable variable variable
    CMD_ORR,

    // print x0 x
    // print x0 s
    // Always is variable base, where base is either d (decimal), x (hex), b (binary), or s (string)
    CMD_PRINT,

    // put "hello, world!" 100
    // Is either put string immediate or put string variable
    CMD_PUT,

    // ret
    // No further arguments
    CMD_RET,

    // store x0 100 4
    // store x8 x1 1
    // Always follows variable variable number or variable number number
    CMD_STORE,

    // sub x0 x1 x2
    // sub x0 x1 5
    // Can either be variable variable variable or variable variable number
    CMD_SUB,
} CommandType;

#endif
