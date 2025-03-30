
#include "BusinessHandler.h"
#include <iostream>
/**
 * @file main.cpp
 * @brief main file of the system.
 * 
 * Funcion `main()` initialize `BusinessHandler` with file ini `config.ini`,
 * after that call `BusinessHandler` with handler.run()`.
 *
 * @return return 0;
 */
int main() {
    // Push file .ini into BusinessHandler
    BusinessHandler handler("config.ini");

    handler.run();
    return 0;
}
