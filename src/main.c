#include <locale.h>
#include "collector.h"

int main(void)
{
    struct collector collector;

    setlocale(LC_ALL, "C.UTF-8");

    collector = collector_novo();
    collector_rodar(collector);

    return 0;
}

