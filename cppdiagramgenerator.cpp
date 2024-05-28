#include "cppdiagramgenerator.h"
#include <QRegularExpression>
#include <QtLogging>

QString CppDiagramGenerator::getByAccessType(const QString &classBody, accessType type)
{

    QRegularExpression re(R"(private:(?<body>[\s\S]*?)((public:)|(protected:)|$))");

    switch (type) {
    case PUBLIC:
        re.setPattern(R"(public:(?<body>[\s\S]*?)((private:)|(protected:)|$))");
        break;
    case PRIVATE:
        re.setPattern(R"(private:(?<body>[\s\S]*?)((public:)|(protected:)|$))");
        break;
    case PROTECTED:
        re.setPattern(R"(protected:(?<body>[\s\S]*?)((private:)|(public:)|$))");
        break;
    default:
        break;
    }
    QString result = "";
    for (const QRegularExpressionMatch &match : re.globalMatch(classBody))
    {
        result.append(" " + match.captured("body"));
    }
    return result;

}

QStringList CppDiagramGenerator::getMembers(const QString &classBody)
{
    QRegularExpression re(R"([^;}]*(;|}))");
    QStringList result;
    for (const QRegularExpressionMatch &match : re.globalMatch(classBody))
    {
        result.append(match.captured());
    }
    return result;
}

QStringList CppDiagramGenerator::getVariables(const QStringList &members)
{
    QRegularExpression re(R"(\s*((virtual)|(static)|(const)|())\s*(?<dataType>[\w\<\>\&\*]*)(\s|^)+(?<name>[\w~]+)[^\(]*(?<braccet>\(*))");
    QStringList result;
    QRegularExpressionMatch match;
    for(const QString &str : members)
    {
        match = re.match(str);
        if(match.hasMatch() && match.captured("braccet") == "")
        {
            result.append(match.captured("name") + ": " + match.captured("dataType") + ";");
        }
    }
    return result;
}

QStringList CppDiagramGenerator::getMethods(const QStringList &members)
{
    QRegularExpression re(R"(\s*((virtual)|(static)|(const)|())\s*(?<dataType>[\w\<\>\&\*]*)(\s|^)+(?<name>[\w~]+)[^\(]*\((?<params>[\s\S]*)\))");
    QStringList result;
    QRegularExpressionMatch match;
    for(const QString &str : members)
    {

        match = re.match(str);
        if(match.hasMatch())
        {
            qInfo() << "Method: " << match.captured() << " dataType: " << match.captured("dataType") << " name: " << match.captured("name");
            QString dataType = match.captured("dataType");

            if(dataType == "")
            {
                dataType = match.captured("name");
            }

            if(match.captured("name").startsWith("~"))
            {
                dataType = "void";
            }



            result.append(match.captured("name") + "(" + formatParams(match.captured("params")) + "): " + dataType + ";");
        }
    }
    return result;
}

QString CppDiagramGenerator::formatParams(const QString &params)
{
    QRegularExpression re(R"((?<dataType>[\w\*\&]+)\s+(?<name>\w+),*)");
    QString result = "";
    for (const QRegularExpressionMatch &match : re.globalMatch(params))
    {
        result += match.captured("dataType") + ", ";
    }
    result.chop(2);
    return result;
}

QString CppDiagramGenerator::formatInheritance(QString className, QString parents)
{
    QRegularExpression re(R"((?<type>\w+)\s+(?<dataType>\w+),*)");
    QString result = "";
    for (const QRegularExpressionMatch &match : re.globalMatch(parents))
    {
        result += " [" + match.captured("dataType") + "]Extends^[" + className + "] \n";
    }
    return result;

}



QString CppDiagramGenerator::toDiagram(const QString &code)
{
    QString result = "";
    QRegularExpression re(R"(class\s+(?<name>\w+)\s(?<parent>[^{]*){(?<body>[\S\s]*?)};)");
    for (const QRegularExpressionMatch &match : re.globalMatch(code))
    {
        result += " [" + match.captured("name") + "| ";

        QStringList privateMembers = getMembers(getByAccessType(match.captured("body"), PRIVATE));
        QStringList publicBody = getMembers(getByAccessType(match.captured("body"), PUBLIC));
        QStringList protectedBody = getMembers(getByAccessType(match.captured("body"), PROTECTED));

        QStringList privateMethods = getMethods(privateMembers);
        QStringList publicMethods = getMethods(publicBody);
        QStringList protectedMethods = getMethods(protectedBody);

        QStringList privateVariables = getVariables(privateMembers);
        QStringList publicVariables = getVariables(publicBody);
        QStringList protectedVariables = getVariables(protectedBody);


        result += (privateVariables.size() != 0 ? ("-" + privateVariables.join("-")) : "");
        result += (publicVariables.size() != 0 ? ("+" + publicVariables.join("+")) : "");
        result += (protectedVariables.size() != 0 ? ("+-" + protectedVariables.join("+-")) : "");

        result += " | ";

        result += (privateMethods.size() != 0 ? ("-" + privateMethods.join("-")) : "");
        result += (publicMethods.size() != 0 ? ("+" + publicMethods.join("+")) : "");
        result += (protectedMethods.size() != 0 ? ("+-" + protectedMethods.join("+-")) : "");

        result += "] \n";

        result += formatInheritance(match.captured("name"), match.captured("parent"));
    }
    result = result.replace("<","＜");
    result = result.replace(">","＞");
    return result;
}
