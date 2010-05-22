#ifndef Library_august2002Fwd_h
#define Library_august2002Fwd_h

#ifdef AUGUST2002_LIBRARY
#define AUGUST2002_EXPORT   __declspec( dllexport )
#else
#define AUGUST2002_EXPORT   __declspec( dllimport )
#endif

#endif
//#define _EXPORT_CLASS_ __declspec(dllexport)
//#define _IMPORT_CLASS_ __declspec(dllimport)