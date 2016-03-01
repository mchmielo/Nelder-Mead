#-------------------------------------------------
#
# Project created by QtCreator 2014-04-12T15:18:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Nelder-Mead
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    muParser.cpp \
    muParserBase.cpp \
    muParserBytecode.cpp \
    muParserCallback.cpp \
    muParserDLL.cpp \
    muParserError.cpp \
    muParserInt.cpp \
    muParserTest.cpp \
    muParserTokenReader.cpp \
    neldermead.cpp \
    parserLib.cpp \
    variable.cpp \
    graph.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    muParser.h \
    muParserBase.h \
    muParserBytecode.h \
    muParserCallback.h \
    muParserDef.h \
    muParserDLL.h \
    muParserError.h \
    muParserFixes.h \
    muParserInt.h \
    muParserStack.h \
    muParserTemplateMagic.h \
    muParserTest.h \
    muParserToken.h \
    muParserTokenReader.h \
    neldermead.h \
    parserLib.h \
    variable.h \
    graph.h

FORMS    += mainwindow.ui \
    graph.ui
