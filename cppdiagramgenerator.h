#ifndef CPPDIAGRAMGENERATOR_H
#define CPPDIAGRAMGENERATOR_H

#include <QStringList>
#include "idiagramgenerator.h"


enum accessType
{
    PUBLIC,
    PRIVATE,
    PROTECTED
};


class CppDiagramGenerator : public IDiagramGenerator
{
private:
    QString getByAccessType(const QString &classBody, accessType type);
    QStringList getMembers(const QString &classBody);
    QStringList getVariables(const QStringList &members);
    QStringList getMethods(const QStringList &members);
    QString formatParams(const QString &params);
    QString formatInheritance(QString className, QString parents);

public:
    QString toDiagram(const QString &code) override;
};

#endif // CPPDIAGRAMGENERATOR_H
