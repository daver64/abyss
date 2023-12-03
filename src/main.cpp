#include "abyss.h"
#include <cstdlib>

void path_test()
{
#ifdef _WIN32
    const char *p = std::getenv("APPDATA");
    const char *f = "abyss";
    printf("data folder='%s\\%s'\n", p, f);
#else
    char *p = std::getenv("HOME");
    const char *f = ".abyss";
    printf("data folder='%s/%s'\n", p, f);
#endif
}
int main(int argc, char *argv[])
{
    path_test();
    return 0;
}