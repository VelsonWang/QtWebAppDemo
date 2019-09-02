#ifndef STARTUP_H
#define STARTUP_H

#include <QCoreApplication>
#include "qtservice.h"
#include "httplistener.h"

using namespace stefanfrings;

class Startup : public QtService<QCoreApplication> {

public:
    Startup(int argc, char *argv[]);

protected:
    void start();
    void stop();
    
private:
    HttpListener* listener;
};

#endif // STARTUP_H
