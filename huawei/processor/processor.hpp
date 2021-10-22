struct Signature
{
    int Sign;
    int Version;
};


enum Commands
{
    CMD_HLT = 0,
    CMD_PUSH,
    CMD_POP,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_OUT,
    CMD_VER,
    CMD_DMP
};