struct line_t // init values + line_t
{
    char* line = 0;
    int lineLen = 0;
};

struct TEXT // init values + consts for poisoned init values
{
    char* buffer = 0;
    long long buffSize = 0;
    line_t* lines = 0;
    int nLines = 0;
};
