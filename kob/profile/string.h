
#ifndef KOB_PROFILE_STRING_H_
#define KOB_PROFILE_STRING_H_

#ifndef KOB_JION
    #define KOB_JION_HELP0(a, b) a##b
    #define KOB_JION_HELP1(a, b) KOB_JION_HELP0(a, b)
    #define KOB_JION(a, b)  KOB_JION_HELP1(a, b)
#endif

#ifndef KOB_STRINGIF
    #define KOB_STRINGIF_HELP(x) #x
    #define KOB_STRINGIF(x) KOB_STRINGIF_HELP(x)
#endif

#ifndef KOB_LPAREN
    #define KOB_LPAREN() (
#endif

#ifndef KOB_RPAREN
    #define KOB_RPAREN() )
#endif

#ifndef KOB_COMMA
    #define KOB_COMMA() ,
#endif

#ifndef KOB_SEMI
    #define KOB_SEMI ;
#endif

#endif //KOB_PROFILE_STRING_H_
