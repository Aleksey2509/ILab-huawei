#include <stdarg.h>
#include "../Headers/textFunc.hpp"
#include "../Headers/stack.hpp"
#include "../Headers/processor.hpp"
#include "../Headers/const.hpp"

int main(int argc, char* argv[])
{
    Processor processor {};
    int Error = processorCtor(argv[1], &processor);
    if (Error)
    {
        return Error;
    }

    Error = runProcessor(&processor);
    if (Error)
        fprintf(stderr, "An Error %d occured\n", Error);

    processorDtor(&processor);

    return 0;
}

int processorCtor(char* codeFile, Processor* processor)
{
    Text code {};
    int Error = TextCreateBuffer(codeFile, &code);
    if (Error)
    {
        fprintf(stderr, "%s\n", TextGetError(Error));
        return Error;
    }

    processor->codeBuf = code.buffer;
    StackCtor(&(processor->stack), 20);
    StackCtor(&(processor->funcCalls), 20);
    for (int i = 0; i < 4; i++)
    {
        processor->reg[i] = 0;
    }

    return 0;
}

int processorDtor(Processor* processor)
{
    free(processor->codeBuf);
    StackDtor(&(processor->funcCalls));
    StackDtor(&(processor->stack));

    return 0;
}



int runProcessor (Processor* processor)
{
    Stack* stack = &processor->stack;
    Stack* retAddrStack = &processor->funcCalls;
    const char* codeBuf = processor->codeBuf;
    int Error = 0;
    int ip = 0;
    int type = 0;
    int arg = 0;
    int cmd = 0;
    int tmp1 = 0;
    int tmp2 = 0;

    while(1)
    {
        //printf("\n\nin cycle: codebuf[%d] = %d\n\n", ip, codeBuf[ip]);
        cmd = codeBuf[ip] & 0x1F;
        //fprintf(stderr, "%d, ip - %d, codebuf[ip] - %x\n", cmd, ip, codeBuf[ip]);
        switch(cmd)
        {
            case CMD_PUSH:
                        type = codeBuf[ip];
                        if (type & ARG_REG)
                        {
                            arg += processor->reg[codeBuf[ip += 1] - 1];
                        }
                        if (type & ARG_IMM)
                        {
                            arg += *(elem_t*)(codeBuf + ip + 1);
                            ip += sizeof(elem_t);
                        }
                        if (type & ARG_RAM)
                        {
                            arg = processor->RAM[arg];
                        }
                        ip += 1;
                        Error = push(stack, arg);
                        if(Error)
                            return Error;
                        arg = 0;
                        break;

            case CMD_POP:
                        type = codeBuf[ip];
                        if (type & ARG_REG)
                        {
                            arg += processor->reg[codeBuf[ip += 1] - 1];
                        }
                        if (type & ARG_IMM)
                        {
                            arg += *(elem_t*)(codeBuf + ip + 1);
                            ip += sizeof(elem_t);
                        }
                        if (type & ARG_RAM)
                            Error = pop(stack, processor->RAM + arg);
                        else
                            Error = pop(stack, processor->reg + codeBuf[ip] - 1);
                        if(Error)
                            return Error;
                        ip += 1;
                        arg = 0;
                        break;

            case CMD_ADD: Error = add(stack);
                          if(Error)
                            return Error;
                        ip += 1;
                        break;

            case CMD_SUB: Error = sub(stack);
                          if(Error)
                            return Error;
                        ip += 1;
                        break;

            case CMD_MUL: Error = mul(stack);
                          if(Error)
                            return Error;
                        ip += 1;
                        break;

            case CMD_DIV: Error = div(stack);
                          if(Error)
                            return Error;
                        ip += 1;
                        break;

            case CMD_OUT: out(stack);
                        ip += 1;
                        break;

            case CMD_VER: Error = ver(stack);
                          if(Error)
                            return Error;
                        ip += 1;
                        break;

            case CMD_DMP: dmp(stack);
                        ip += 1;
                        break;

            case CMD_JMP: Error = push (retAddrStack, ip + 1);
                          if(Error)
                            return Error;
                        ip = codeBuf[ip + 1];
                        break;
            
            case CMD_JA: Error = pop (stack, &tmp1);
                         Error = pop (stack, &tmp2);
                         if(Error)
                            return Error;
                        if (tmp1 > tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_JAE: Error = pop (stack, &tmp1);
                          Error = pop (stack, &tmp2);
                          if(Error)
                            return Error;
                        if (tmp1 >= tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_JB: Error = pop (stack, &tmp1);
                         Error = pop (stack, &tmp2);
                         if(Error)
                            return Error;
                        if (tmp1 < tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_JBE: Error = pop (stack, &tmp1);
                         Error = pop (stack, &tmp2);
                         if(Error)
                            return Error;
                        if (tmp1 <= tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_JE: Error = pop (stack, &tmp1);
                         Error = pop (stack, &tmp2);
                         if(Error)
                            return Error;
                        if (tmp1 == tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_JNE: Error = pop (stack, &tmp1);
                         Error = pop (stack, &tmp2);
                         if(Error)
                            return Error;
                        if (tmp1 == tmp2) 
                        {
                            ip = codeBuf[ip + 1];
                        }
                        else
                            ip += 2;
                        break;

            case CMD_CALL: Error = push(retAddrStack, ip + 2);
                           if(Error)
                            return Error;
                           ip = codeBuf[ip + 1];
                           break;

            case CMD_RET: Error = pop(retAddrStack, &ip);
                          if(Error)
                            return Error;
                          break;

            case CMD_HLT: return 0;

            default: printf("\n\nUnknown code for command; dumping and stopping working\n\n");
                     dmp(stack);
                     return 1;
        }
    }

    return 1;

}

//------------------------------------------------------------------------------------------------------------------------------------

int push(Stack* stack, elem_t a)
{
    int Error = StackPush(stack, a);
    
    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int pop(Stack* stack, elem_t* dst)
{
    int Error = StackPop(stack, dst);
    
    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int add(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(stack, Error);

    Error = StackPop(stack, &b);
    TrackError(stack, Error);

    int result = a + b;

    Error = StackPush(stack, result);
    TrackError(stack, Error);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int sub(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(stack, Error);

    Error = StackPop(stack, &b);
    TrackError(stack, Error);

    int result = b - a;

    Error = StackPush(stack, result);
    TrackError(stack, Error);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int mul(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(stack, Error);

    Error = StackPop(stack, &b);
    TrackError(stack, Error);

    int result = a * b;

    Error = StackPush(stack, result);
    TrackError(stack, Error);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

int div(Stack* stack)
{
    int a = 0;
    int b = 0;

    int Error = StackPop(stack, &a);
    TrackError(stack, Error);

    Error = StackPop(stack, &b);
    TrackError(stack, Error);

    if (b == 0)
        return -1;

    int result = a / b;

    Error = StackPush(stack, result);
    TrackError(stack, Error);

    return Error;
}

//------------------------------------------------------------------------------------------------------------------------------------

void out(Stack* stack)
{
    if (stack->size == 0)
        return;
    int toPrint;
    pop(stack, &toPrint);
    printf("%d\n", toPrint);
}

//------------------------------------------------------------------------------------------------------------------------------------

int ver(Stack* stack)
{
    return (CheckStack(stack));
}

//------------------------------------------------------------------------------------------------------------------------------------

void dmp(Stack* stack)
{
    StackDump(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__);
}