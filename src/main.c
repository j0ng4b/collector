#include <locale.h>
#include "collector.h"

int main(void)
{
    struct collector collector;

    setlocale(LC_ALL, "C.UTF-8");

    collector_novo(&collector);
    collector_rodar(&collector);

    return 0;
}

