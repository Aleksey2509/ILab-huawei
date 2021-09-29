struct line // line_t
{
    char* line;
    int lineLen;
};

struct TEXT // consts for poisoned init values
{
    char* buffer;
    long long buffSize;
    line* lines;
    int nLines;
};

