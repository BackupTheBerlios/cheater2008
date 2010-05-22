#ifndef STANDART_StandartFwd_h
#define STANDART_StandartFwd_h

#ifdef STANDART_LIBRARY
#define STANDART_EXPORT_FUN(type)   __declspec( dllexport ) type
#else
#define STANDART_EXPORT_FUN(type)   __declspec( dllimport ) type
#endif

#endif