#include <qtautomation.h>

N::CppCoding:: CppCoding(void)
{
  Initialize ( ) ;
}

N::CppCoding::CppCoding (const CppCoding & coding)
{
  ME = coding ;
}

N::CppCoding::~CppCoding(void)
{
}

N::CppCoding & N::CppCoding::operator = (const CppCoding & coding)
{
  nMemberCopy ( coding , Cpp      ) ;
  nMemberCopy ( coding , Mode     ) ;
  nMemberCopy ( coding , Brackets ) ;
  nMemberCopy ( coding , x        ) ;
  nMemberCopy ( coding , y        ) ;
  nMemberCopy ( coding , Tabs     ) ;
  nMemberCopy ( coding , tab      ) ;
  nMemberCopy ( coding , space    ) ;
  nMemberCopy ( coding , crlf     ) ;
  nMemberCopy ( coding , Class    ) ;
  nMemberCopy ( coding , Defines  ) ;
  nMemberCopy ( coding , Lines    ) ;
  nMemberCopy ( coding , Align    ) ;
  return ME                         ;
}

QByteArray & N::CppCoding::operator () (void)
{
  return Cpp ;
}

void N::CppCoding::Initialize(void)
{
  reset();
}

bool N::CppCoding::reset(void)
{
  x        = 0       ;
  y        = 0       ;
  Tabs     = 0       ;
  space    = " "     ;
  Mode     = SOURCE  ;
  Brackets = None    ;
  Class    = ""      ;
  Defines  . clear() ;
  Align    . clear() ;
  Lines    . clear() ;
  Cpp      . clear() ;
  setCRLF  ( "\r\n" );
  return (0==Cpp.size());
}

void N::CppCoding::setMode(CppMode mode)
{
  Mode = mode;
}

void N::CppCoding::setCRLF(QString CRLF)
{
  crlf = CRLF;
}

void N::CppCoding::newl(void)
{
  if (Tabs<=0) return;
  Cpp += tab.toUtf8();
  x   += tab.length();
}

void N::CppCoding::endl(void)
{
  Cpp += crlf.toUtf8();
  y++  ;
  x = 0;
}

void N::CppCoding::ends(void)
{
  Cpp += ";";
  x  ++     ;
}

void N::CppCoding::addTab(int spaces)
{
  Tabs += spaces;
  tab   = space.repeated(Tabs);
}

void N::CppCoding::delTab(int spaces)
{
  Tabs -= spaces;
  if (Tabs< 0) Tabs = 0;
  if (Tabs<=0) tab = "";
          else tab = space.repeated(Tabs);
}

void N::CppCoding::pushAlign(void)
{
  Align << x;
}

void N::CppCoding::popAlign(void)
{
  int S = Align.count() - 1;
  if (S<0) return;
  Align.takeAt(S);
}

void N::CppCoding::doAlign(void)
{
  int S = Align.count() - 1;
  if (S<0) return;
  int X = Align[S];
  if (X<=x) return;
  Cpp += space.repeated(X-x).toUtf8();
  x    = X;
}

void N::CppCoding::adjust(void)
{
  int size  = Cpp.size();
  int from  = 0;
  int index = 0;
  QByteArrayMatcher BAM(Cpp);
  y = 0;
  do {
    index = BAM.indexIn("\n",from);
    if (index>=0) { from = index + 1; y++; };
  } while (index>=0);
  x = 0;
  x = size - from;
}

/////////////////////
// File Operations //
/////////////////////
bool N::CppCoding::load(QString FileName)
{
  QFile F(FileName);
  if (!F.open(QIODevice::WriteOnly)) return false;
  reset();
  Cpp = F.readAll();
  F.close();
  return true;
}

bool N::CppCoding::save(QString FileName)
{
  QFile F(FileName);
  if (!F.open(QIODevice::WriteOnly)) return false;
  F.write(Cpp);
  F.close();
  return true;
}

int N::CppCoding::size(void)
{
  return Cpp.size();
}

///////////////
// operators //
///////////////
QByteArray & N::CppCoding::operator += (QString statements)
{
  Cpp += statements.toUtf8();
  adjust();
  return Cpp;
}

QByteArray & N::CppCoding::operator += (QByteArray statements)
{
  Cpp += statements;
  adjust();
  return Cpp;
}

/////////////////
// C++ Control //
/////////////////
void N::CppCoding::addNamespace(QString name,QString statements)
{
  doAlign()                  ;
  Cpp += "namespace "        ;
  Cpp += name.toUtf8()       ;
  Cpp += " {"                ;
  Cpp += crlf.toUtf8()       ;
  doAlign()                  ;
  Cpp += "  "                ;
  Cpp += statements.toUtf8() ;
  Cpp += crlf.toUtf8()       ;
  Cpp += "};"                ;
  Cpp += crlf.toUtf8()       ;
  x    = 0                   ;
  y++                        ;
  y++                        ;
  y++                        ;
}

void N::CppCoding::addNamespaces(QString name,QStringList & statements)
{
  if (statements.count()<=0) return;
  doAlign()              ;
  Cpp += "namespace "    ;
  Cpp += name.toUtf8()            ;
  Cpp += " {"            ;
  Cpp += crlf.toUtf8()            ;
  for (int i=0;i<statements.count();i++)
  {
    doAlign()            ;
    Cpp += "  "          ;
    Cpp += statements[i].toUtf8() ;
    Cpp += crlf.toUtf8()          ;
  }                      ;
  Cpp += "};"            ;
  Cpp += crlf.toUtf8()            ;
  x    = 0               ;
  y++                    ;
  y++                    ;
}

void N::CppCoding::addComment(QString comment)
{
  Cpp += "// ";
  x   += 3;
  Cpp += comment.toUtf8();
  x   += comment.toUtf8().size();
}

void N::CppCoding::addComments(QStringList & comments)
{
  if (comments.count()<=0) return;
  for (int i=0;i<comments.count();i++) {
    Cpp += "// ";
    Cpp += comments[i].toUtf8();
    Cpp += crlf.toUtf8();
    y++;
  };
  x = 0;
}

void N::CppCoding::commentBlock(QString comment)
{
  int max = 0;
  QString p = "/";
  QStringList c = comment.split("\r\n");
  for (int i=0;i<c.count();i++) c[i].replace("\r\n","");
  for (int i=0;i<c.count();i++) if (c[i].toUtf8().size()>max) max = c[i].toUtf8().size();
  max += 6;
  Cpp += p.repeated(max).toUtf8();
  endl();
  if (c.count()>0) addComments(c);
  Cpp += p.repeated(max).toUtf8();
  endl();
}

void N::CppCoding::Quote(QString name)
{
  Cpp += "\"";
  Cpp += name.toUtf8();
  Cpp += "\"";
  x   += 2;
  x   += name.length();
}

void N::CppCoding::String(QString string,int max)
{
  if (string.length()<=max) { Quote(string); return; };
  int     lastX = x   ;
  int     start = max ;
  QString s           ;
  Quote(string.left(max));
  while (start<string.length()) {
    endl();
    Cpp += space.repeated(lastX).toUtf8();
    x    = lastX;
    s    = string.mid(start,max);
    if (s.length()>0) Quote(s);
  };
}

void N::CppCoding::add(QString cpp)
{
  Cpp += cpp.toUtf8();
  x   += cpp.length();
}

void N::CppCoding::line(QString string)
{
  Cpp += string.toUtf8();
  endl();
}

void N::CppCoding::doClass(QString ClassName)
{
  Class = ClassName;
}

void N::CppCoding::quickFor(QString Index,QString End,QString extra,bool autoInt)
{
  Cpp   += "for ( ";
  if (autoInt) { Cpp += "int "; x += 4; };
  Cpp   += Index.toUtf8();
  Cpp   += " ; ";
  Cpp   += Index.toUtf8();
  Cpp   += " < ";
  Cpp   += End.toUtf8();
  Cpp   += " ; ";
  Cpp   += Index.toUtf8();
  Cpp   += "++ )";
  if (extra.length()>0) {
    Cpp += extra.toUtf8();
    x   += extra.length();
  };
  x     += End.length();
  x     += Index.length() * 3;
  x     += 19;
}

///////////////////
// Preprocessors //
///////////////////
void N::CppCoding::pushDefine(QString DEFINE)
{
  Defines << DEFINE;
  Cpp += "#ifndef ";
  Cpp += DEFINE.toUtf8();
  Cpp += crlf.toUtf8();
  Cpp += "#define ";
  Cpp += DEFINE.toUtf8();
  Cpp += crlf.toUtf8();
  x    = 0;
  y++;
  y++;
}

void N::CppCoding::popDefine(void)
{
  if (Defines.count()<=0) return;
  int S = Defines.count() - 1;
  QString DEFINE = Defines[S];
  Defines.takeAt(S);
  Cpp += "#endif // ";
  Cpp += DEFINE.toUtf8();
  Cpp += crlf.toUtf8();
  x    = 0;
  y++;
}

void N::CppCoding::doDefine(QString DEFINE,QString VARIABLE)
{
  newl();
  Cpp += "#define ";
  Cpp += DEFINE.toUtf8();
  if (VARIABLE.length()>0) {
    Cpp += " ";
    Cpp += VARIABLE.toUtf8();
  };
  Cpp += crlf.toUtf8();
  x    = 0;
  y++;
}

void N::CppCoding::unDefine(QString DEFINE)
{
  Cpp += "#undef  ";
  Cpp += DEFINE.toUtf8();
  Cpp += crlf.toUtf8();
  x    = 0;
  y++;
}

void N::CppCoding::Include(QString INCLUDE,bool equal)
{
  Cpp += "#include ";
  Cpp += (equal) ? "<" : "\"";
  Cpp += INCLUDE.toUtf8();
  Cpp += (equal) ? ">" : "\"";
  endl();
}

void N::CppCoding::Ifdef(QString DEFINE)
{
  newl();
  Cpp += "#ifdef ";
  Cpp += DEFINE.toUtf8();
  endl();
}

void N::CppCoding::Endif(void)
{
  newl();
  Cpp += "#endif";
  endl();
}

// Brackets
void N::CppCoding::brackets(QStringList & items)
{
  newl();
  Cpp += "{";
  endl();
  addTab(2);
  if (items.count()>0)
  for (int i=0;i<items.count();i++) {
    newl();
    Cpp += items[i].toUtf8();
    endl();
  };
  delTab(2);
  newl();
  Cpp += "};";
  endl();
}

void N::CppCoding::startb(void)
{
  Cpp += "{";
  endl();
  addTab(2);
}

void N::CppCoding::endb(void)
{
  delTab(2);
  newl();
  Cpp += "};";
  endl();
}

// Variables
void N::CppCoding::Var(QString type,QString name,QString comment)
{
  newl();
  Cpp += type.toUtf8();
  Cpp += " ";
  Cpp += name.toUtf8();
  Cpp += ";";
  if (comment.length()>0) {
    Cpp += " // ";
    Cpp += comment.toUtf8();
  };
  endl();
}

void N::CppCoding::If(QString Statements,QString after)
{
  QStringList s = Statements.split("\r\n");
  if (s.count()<2) {
    Cpp += "if (";
    Cpp += Statements.toUtf8();
    Cpp += ")";
    x   += Statements.length();
    x   += 5;
    if (after.length()>0) {
      Cpp += after.toUtf8();
      x   += after.length();
    };
    return;
  };
  int lastX = x;
  for (int i=0;i<s.count();i++) s[i].replace("\r\n","");
  Cpp += "if (";
  Cpp += s[0].toUtf8();
  endl();
  for (int i=1;i<s.count();i++) {
    Cpp += space.repeated(lastX+4).toUtf8();
    x    = lastX+4;
    Cpp += s[i].toUtf8();
    x   += s[i].length();
    if ((i+1)<s.count()) endl();
  };
  Cpp += ")";
  x++;
  if (after.length()>0) {
    Cpp += after.toUtf8();
    x   += after.length();
  };
}

void N::CppCoding::Switch(QString State)
{
  newl();
  Cpp += "switch (";
  Cpp += State.toUtf8();
  Cpp += ")";
  endl();
}

void N::CppCoding::Case(QString State)
{
  newl();
  Cpp += "case ";
  Cpp += State.toUtf8();
  Cpp += " :";
  endl();
}

void N::CppCoding::Break(void)
{
  newl();
  Cpp += "break;";
  endl();
}

// Functions
void N::CppCoding::declare(QString Return,QString name,QString Paraments,QString after)
{
  if (Return.length()==0) Return = "void";
  newl();
  Cpp += Return.toUtf8() ;
  Cpp += " "    ;
  Cpp += name.toUtf8()   ;
  Cpp += " ("   ;
  if (Paraments.length()==0) Cpp += "void";
                       else  Cpp += Paraments.toUtf8() ;
  Cpp += ")"    ;
  if (after.length()>0) Cpp += after.toUtf8();
  endl();
}

void N::CppCoding::function(QString Return,QString name,QString Paraments,QString after)
{
  newl();
  if (Return.length()>0) {
    Cpp += Return.toUtf8() ;
    Cpp += " "    ;
  };
  Cpp += Class.toUtf8()  ;
  Cpp += "::"   ;
  Cpp += name.toUtf8()   ;
  Cpp += " ("   ;
  if (Paraments.length()==0) Cpp += "void";
                       else  Cpp += Paraments.toUtf8() ;
  Cpp += ")"    ;
  if (after.length()>0) Cpp += after.toUtf8();
  endl();
}
