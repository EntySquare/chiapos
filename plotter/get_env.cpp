//
// Created by Nate Yang on 2021/5/21.
//
#include <cstdlib>
#include <cstdio>


int main(void)
{
    char *pathvar;
    pathvar = getenv("HOME");
    printf("pathvar=%s",pathvar);
    return 0;
}
