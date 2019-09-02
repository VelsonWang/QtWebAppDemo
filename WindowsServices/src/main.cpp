#include "startup.h"

using namespace stefanfrings;

// run the program with argument "-e"

#if 0
    Windows
    When you now go to the target Windows server, you enter the command "MyFirstWebApp -i" to install the windows service. Then enter "MyFirstWebapp" without arguments to start the service. The command "MyFirstWebApp -t" would terminate the service.
    You can also use the service manager screen in the system control panel to check the status, start and stop the service.

    And you can also use the "net" command, e.g. "net start MyFirstWebApp".

    Linux
    In Linux you do not need qtservice features.
    You would normally write an init script in /etc/init.d which wraps around your web server executable. The init script would start your program by the command "MyFirstWebApp &", and it would stop the program using the kill or pkill command.

    But when you use qtservice, then the commands are different: You have to enter the command "./MyFirstWebApp -i" once to create a configuration setting that qtservice needs. Then start the program by the command "MyFirstWebApp" without arguments. Terminate the program by calling "MyFirstWebApp -t".

    On both operating systems, the argument -h shows a help about the available options.

#endif

int main(int argc, char *argv[])
{
    Startup startup(argc, argv);
    return startup.exec();
}
