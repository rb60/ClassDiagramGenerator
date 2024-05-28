#ifndef IDIAGRAMGENERATOR_H
#define IDIAGRAMGENERATOR_H
#include <QString>

class IDiagramGenerator
{
public:
    virtual QString toDiagram(const QString& code) = 0;
};

#endif // IDIAGRAMGENERATOR_H
