/****************************************************************************
 *                                                                          *
 * Copyright (C) 2015 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>, Skype: wolfram_lin            *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_AUTOMATION_H
#define QT_AUTOMATION_H

#include <QtCore>
#include <QtFoundation>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_QTAUTOMATION_LIB)
#      define Q_AUTOMATION_EXPORT Q_DECL_EXPORT
#    else
#      define Q_AUTOMATION_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_AUTOMATION_EXPORT
#endif


namespace N
{

class Q_AUTOMATION_EXPORT CppCoding ;

/*****************************************************************************
 *                                                                           *
 *                            Cognitive Coders                               *
 *                                                                           *
 *****************************************************************************/

class Q_AUTOMATION_EXPORT CppCoding
{ // C/C++ coding tool
  public:

    enum CppMode       {
      SOURCE = 0       ,
      HEADER = 1       ,
      TEXT   = 2     } ;

    enum BracketMode   {
      None       = 0   ,
      Round      = 1   ,
      Squirrelly = 2   ,
      Inequality = 3 } ;

    QByteArray Cpp;

    explicit CppCoding       (void) ;
             CppCoding       (const CppCoding & coding) ;
    virtual ~CppCoding       (void) ;

    ///////////////
    // operators //
    ///////////////
    CppCoding  & operator =  (const CppCoding & coding) ;
    QByteArray & operator () (void) ;
    QByteArray & operator += (QString    statements) ;
    QByteArray & operator += (QByteArray statements) ;

    ///////////////
    void setMode             (CppMode mode) ;
    bool reset               (void) ;

    /////////////////////
    // File Operations //
    /////////////////////
    bool load                (QString FileName) ;
    bool save                (QString FileName) ;
    int  size                (void) ;

    //////////////////
    // line control //
    //////////////////
    void adjust              (void) ;
    void setCRLF             (QString CRLF) ;
    void addTab              (int spaces = 2) ;
    void delTab              (int spaces = 2) ;
    void newl                (void) ;
    void endl                (void) ;
    void ends                (void) ;
    void pushAlign           (void) ;
    void popAlign            (void) ;
    void doAlign             (void) ;
    void line                (QString string) ;

    ///////////////////
    // Preprocessors //
    ///////////////////
    void pushDefine          (QString DEFINE) ;
    void popDefine           (void) ;
    void doDefine            (QString DEFINE,QString VARIABLE = "") ;
    void unDefine            (QString DEFINE) ;
    void Include             (QString INCLUDE,bool equal = false) ;
    void Ifdef               (QString DEFINE) ;
    void Endif               (void) ;

    /////////////////
    // C++ Control //
    /////////////////
    void addNamespace        (QString name,QString       statements) ;
    void addNamespaces       (QString name,QStringList & statements) ;
    void addComment          (QString       comment ) ;
    void addComments         (QStringList & comments) ;
    void commentBlock        (QString       comment ) ;
    void Quote               (QString name) ;
    void String              (QString string,int max = 40) ;
    void add                 (QString cpp) ;
    // Brackets
    void brackets            (QStringList & items) ;
    void startb              (void) ;
    void endb                (void) ;
    // Variables
    void Var                 (QString type,QString name,QString comment = "") ;
    void If                  (QString Statements,QString after = "") ;
    void Switch              (QString State) ;
    void Case                (QString State) ;
    void Break               (void) ;
    // Functions
    void declare             (QString Return,QString name,QString Paraments,QString after = ";") ;
    void quickFor            (QString Index,QString End,QString extra = "",bool autoInt = true) ;

    ///////////////////////
    // C++ Class Control //
    ///////////////////////
    void doClass             (QString ClassName) ;
    void function            (QString Return,QString name,QString Paraments,QString after = "") ;

  protected:

    virtual void Initialize (void) ;

  private:

    CppMode     Mode     ;
    BracketMode Brackets ;
    int         x        ;
    int         y        ;
    int         Tabs     ;
    QString     tab      ;
    QString     space    ;
    QString     crlf     ;
    QString     Class    ;
    QStringList Defines  ;
    QStringList Lines    ;
    CUIDs       Align    ;

};

}

Q_DECLARE_METATYPE(N::CppCoding)

QT_END_NAMESPACE

#endif
