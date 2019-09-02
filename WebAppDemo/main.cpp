#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QString>
#include "httplistener.h"
#include "httprequesthandler.h"
#include "helloworldcontroller.h"
#include "requestmapper.h"
#include "filelogger.h"

using namespace stefanfrings;

/**
 * Search the configuration file.
 * Aborts the application if not found.
 * @return The valid filename
 */
QString searchConfigFile() {
    QString binDir = QCoreApplication::applicationDirPath();
    QString appName = QCoreApplication::applicationName();
    QFile file;
    file.setFileName(binDir+"/webapp1.ini");
    if (!file.exists()) {
        file.setFileName(binDir+"/etc/webapp1.ini");
        if (!file.exists()) {
            file.setFileName(binDir+"/../etc/webapp1.ini");
            if (!file.exists()) {
                file.setFileName(binDir+"/../"+appName+"/etc/webapp1.ini");
                if (!file.exists()) {
                    file.setFileName(binDir+"/../../"+appName+"/etc/webapp1.ini");
                    if (!file.exists()) {
                        file.setFileName(binDir+"/../../../../../"+appName+"/etc/webapp1.ini");
                        if (!file.exists()) {
                            file.setFileName(QDir::rootPath()+"etc/webapp1.ini");
                        }
                    }
                }
            }
        }
    }
    if (file.exists()) {
        QString configFileName=QDir(file.fileName()).canonicalPath();
        qDebug("using config file %s", qPrintable(configFileName));
        return configFileName;
    }
    else {
        qFatal("config file not found");
    }
}



int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Load the configuration file
    QString configFileName = searchConfigFile();

    // Configure logging
    QSettings* logSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    logSettings->beginGroup("logging");
    RequestMapper::logger = new FileLogger(logSettings,1000, &app);
    RequestMapper::logger->installMsgHandler();

    // Log the library version
    qDebug("QtWebApp has version %s",getQtWebAppLibVersion());

    // Session store
    QSettings* sessionSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    sessionSettings->beginGroup("sessions");
    RequestMapper::sessionStore = new HttpSessionStore(sessionSettings,&app);

    // Static file controller
    QSettings* fileSettings = new QSettings(configFileName,QSettings::IniFormat, &app);
    fileSettings->beginGroup("files");
    RequestMapper::staticFileController = new StaticFileController(fileSettings, &app);

    // Configure template cache
    QSettings* templateSettings=new QSettings(configFileName,QSettings::IniFormat, &app);
    templateSettings->beginGroup("templates");
    RequestMapper::templateCache = new TemplateCache(templateSettings,&app);

    // Start the HTTP server
    QSettings* listenerSettings = new QSettings(configFileName, QSettings::IniFormat, &app);
    listenerSettings->beginGroup("listener");

    // http://localhost:8080
    //new HttpListener(listenerSettings, new HttpRequestHandler(&app), &app);

    // http://localhost:8080
    //new HttpListener(listenerSettings,new HelloWorldController(&app),&app);

    // http://localhost:8080
    // http://localhost:8080/hello
    // http://localhost:8080/list
    // http://localhost:8080/login
    // http://localhost:8080/cookie
    // http://localhost:8080/files/hello.html
    // http://localhost:8080/list2
    // http://localhost:8080/login?username=test&password=hello
    new HttpListener(listenerSettings,new RequestMapper(&app),&app);


    return app.exec();
}


