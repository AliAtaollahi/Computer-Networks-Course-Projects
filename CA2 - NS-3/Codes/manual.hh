#pragma once

#include <string>

//colors
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define MAPPERS_NUM 3
#define MAPPERS_PORT_INIT 1000
#define GLOBAL_PORT 1102

const std::string  MAPPER_0_COLOR = KGRN;
const std::string  MAPPER_1_COLOR = KBLU;
const std::string  MAPPER_2_COLOR = KMAG;

struct Codes
{
    static std::map<int,char> create_map()
    {
        std::map<int, char> messages = 
        {
            {0 , 'm'},
            {1 , 'g'},
            {2 , 'b'},
            {3 , 'c'},
            {4 , 'd'},
            {5 , 'e'},
            {6 , 'f'},
            {7 , 'p'},
            {8 , 'j'},
            {9 , 'o'},
            {10, 'k'},
            {11, 'z'},
            {12, 'i'},
            {13, 'h'},
            {14, 'q'},
            {15, 'y'},
            {16, 'n'},
            {17, 's'},
            {18, 't'},
            {19, 'r'},
            {20, 'a'},
            {21, 'v'},
            {22, 'u'},
            {23, 'l'},
            {24, 'w'},
            {25, 'x'}
        };
        return messages;
    }
    static std::map<int, char> decrypt;
};

namespace InterfacesNum
{
    enum {
        CLIENT = 1,
        MASTER = 2,
        MAPPER = 3
    };
} 
