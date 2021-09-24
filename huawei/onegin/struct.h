struct line
{
    char* line;
    int lineLen;
};

struct TEXT
{
    char* buffer;
    long long buffSize;
    line* lines;
    int nLines;
};

