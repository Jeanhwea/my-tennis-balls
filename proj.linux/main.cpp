#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string>

#include "../Classes/AppDelegate.h"

USING_NS_CC;

int main(int argc, char **argv)
{
    // create the application instance
    AppDelegate app;
    return Application::getInstance()->run();
}
