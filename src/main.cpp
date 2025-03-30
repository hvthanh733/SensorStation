
#include "BusinessHandler.h"
#include <iostream>

int main() {
    // Push file .ini into BusinessHandler
    BusinessHandler handler("config.ini");

    handler.run();
    return 0;
}
