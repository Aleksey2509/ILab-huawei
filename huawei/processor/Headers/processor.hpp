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
    CMD_DMP,
    CMD_JMP,
    CMD_JA,
    CMD_JAE,
    CMD_JB,
    CMD_JBE,
    CMD_JE,
    CMD_JNE,
    CMD_CALL,
    CMD_RET
};

enum ArgTypes
{
    ARG_IMM = 0x20,
    ARG_REG = 0x40,
    ARG_RAM = 0x80,
};

enum Errors
{
    MEM_ERROR = -1000,
    EMPTY_LINE,
    LABEL_OVERFLOW,
    UNKNOWN_CMD,
    INVALID_ARG,
    SYNTAX_ERROR
};

const int MAX_LABELS = 1024;

struct Signature
{
    int sign = 0;
    int version = 0;
};

struct Label
{
    int address = 0;
    char* name = NULL;
};

struct Command
{
    Commands cmdNum;
    const char* cmdName;
};

struct codeData
{
    char** cmdArr = NULL;
    char* codeBuf = NULL;
    int ip = 0;
    int labelsNum = 0;
    Label* labels = NULL;
};

struct Processor
{
    Stack stack {};
    Stack funcCalls {};
    int reg[4] = {0};
    char* codeBuf = 0;
    int ip = 0;
    int RAM[BUFSIZ] = { 0 };

};

const Command commandsArr[] = {{CMD_HLT, "hlt"}, {CMD_PUSH, "push"}, {CMD_POP , "pop" }, {CMD_ADD, "add"}, {CMD_SUB, "sub"},
                               {CMD_MUL, "mul"}, {CMD_DIV , "div" }, {CMD_OUT , "out" }, {CMD_VER, "ver"}, {CMD_DMP, "dmp"},
                               {CMD_JMP, "jmp"}, {CMD_JA,   "ja"  }, {CMD_JAE , "jae" }, {CMD_JB , "jb" }, {CMD_JBE, "jbe"},
                               {CMD_JE , "je" }, {CMD_JNE,  "jne" }, {CMD_CALL, "call"}, {CMD_RET, "ret"} };

int assembler(char* codeFile);

int codeDataCtor (char* codeFile, Text* codeText, codeData* codeCoating);
int codeDataDtor (Text* codeText, codeData* codeCoating);

int assemble(Text* codeText, codeData* codeCoating);
int firstPass(int codeLines, codeData* codeCoating);
int secondPass(int codeLines, codeData* codeCoating);

int parseArgs(char* argStr, codeData* codeCoating, int cmdAddr);
int cmdToCode(char* cmdbuf, char* codeBuf);
int regToCode(char* reg);
char* removeComment(char *str);

int checkIfLabel(char* cmd);
int checkIfJmp(int cmdNum);
int checkForRAM (char* str);
int searchForLabelAddr(const char* labelName, const codeData* codeCoating);

int debugPrintCode(codeData* codeCoating);



int runProcessor (Processor* processor);
int processorCtor(char* codeFile, Processor* processor);
int processorDtor(Processor* processor);

int push(Stack* stack, int a);
int pop(Stack* stack, elem_t* dst = 0);
int add(Stack* stack);
int sub(Stack* stack);
int mul(Stack* stack);
int div(Stack* stack);
void out(Stack* stack);
int ver(Stack* stack);
void dmp(Stack* stack);