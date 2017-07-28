/*
#include "StringUtils.h"

using namespace std;

vector<std::string> splitString(const char *str, char c)
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}

std::string trimString(const std::string & sStr)
{
    int nSize = sStr.size();
    int nSPos = 0, nEPos = 1, i;
    for(i = 0; i< nSize; ++i) {
        if( !isspace( sStr[i] ) ) {
            nSPos = i ;
            break;
        }
    }
    for(i = nSize -1 ; i >= 0 ; --i) {
        if( !isspace( sStr[i] ) ) {
            nEPos = i;
            break;
        }
    }
    return string(sStr, nSPos, nEPos - nSPos + 1);
}
*/
