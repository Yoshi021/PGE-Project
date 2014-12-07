#include "autocodecompiler.h"

AutocodeCompiler::AutocodeCompiler(QList<EventCommand *> commands, QObject *parent) :
    BasicCompiler(commands, parent)
{}

AutocodeCompiler::~AutocodeCompiler()
{}

QString AutocodeCompiler::compileCode()
{
    QString compiledCode;
    compiledCode += "//This code was generated by PGE!\n";
    compiledCode += "//You're free to modify this code but recompiling will overwrite\n";
    compiledCode += "//user-modified code!\n\n";

    for(int i = 0; i < m_events.size(); ++i){
        compiledCode += m_events[i]->compileSegment(Script::COMPILER_AUTOCODE, 0);
        compiledCode += "\n\n\n";
    }

    return compiledCode;
}
