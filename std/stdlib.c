#include"stdlib.h"

// double atof(const char* str){
//     double ret = 0;
//     int sign = 1;
//     int i = 0;
//     while(str[i] == ' ') ++i;
//     if(str[i] == '-'){
//         sign = -1;
//         ++i;
//     }
//     else if(str[i] == '+') ++i;
//     while(str[i] >= '0' && str[i] <= '9'){
//         ret = ret * 10 + (str[i] - '0');
//         ++i;
//     }
//     if(str[i] == '.'){
//         double div = 1;
//         ++i;
//         while(str[i] >= '0' && str[i] <= '9'){
//             div *= 10;
//             ret += (str[i] - '0') / div;
//             ++i;
//         }
//     }
//     return sign * ret;
// }

int atoi(const char* str){
    int ret = 0;
    int sign = 1;
    int i = 0;
    while(str[i] == ' ') ++i;
    if(str[i] == '-'){
        sign = -1;
        ++i;
    }
    else if(str[i] == '+') ++i;
    while(str[i] >= '0' && str[i] <= '9'){
        ret = ret * 10 + (str[i] - '0');
        ++i;
    }
    return sign * ret;
}

long int atol(const char* str){
    long int ret = 0;
    int sign = 1;
    int i = 0;
    while(str[i] == ' ') ++i;
    if(str[i] == '-'){
        sign = -1;
        ++i;
    }
    else if(str[i] == '+') ++i;
    while(str[i] >= '0' && str[i] <= '9'){
        ret = ret * 10 + (str[i] - '0');
        ++i;
    }
    return sign * ret;
}

void* calloc(size_t nmemb, size_t size){
    void* ret = malloc(nmemb * size);
    //if(ret) memset(ret, 0, nmemb * size);
    return ret;
}

void* realloc(void* ptr, size_t size){
    void* ret = malloc(size);
    if(ret){
        //memcpy(ret, ptr, size);
        free(ptr);
    }
    return ret;
}
