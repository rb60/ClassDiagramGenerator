#include "mainwindow.h"

#include <QApplication>
#include <QtLogging>
#include <stdio.h>
#include <stdlib.h>


void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message = qFormatLogMessage(type, context, msg);
    static FILE *f = fopen("log.txt", "a");
    fprintf(f, "%s\n", qPrintable(message));
    fflush(f);
}


int main(int argc, char *argv[])
{
    remove("log.txt");
    qInstallMessageHandler(logToFile);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
