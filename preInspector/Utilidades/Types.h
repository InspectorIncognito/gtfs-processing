/**
* Project: "pvm".
* Package: 
* Class: 
* Created by: Mauricio Zuñiga G. ( mzuniga@pragxis.cl )
* Terms: These coded instructions, statements, and computer programs are
* produced as part of a software project. They contain unpublished
* propietary information and are protected by copyright law. They may
* not be disclosed to third parties or copied or duplicated in any form,
* in whole or in part, without the prior written consent of Pragxis SpA.
* Last modified : Mauricio Zuñiga 01-04-2013
*/

#pragma once

///Codificacion de los tipos de transporte posibles de una etapa o transaccion
enum TIPOREPORTE { ASCII=1, LATEX=2, HTML=3, CONSOLA=4 };

///Tipo para coordenadas
#define COORD int

///\brief Constante Verdadero
#define TRUE 1
///\brief Constante Falso
#define FALSE 0

///\brief Constante Plataforma Windows
#define T3_PLATFORM_WIN32 1
///\brief Constante Plataforma Linux
#define T3_PLATFORM_LINUX 2
///\brief Constante Plataforma Apple
#define T3_PLATFORM_APPLE 3

///\brief Constante Arquitectura 32bits
#define T3_ARCHITECTURE_32 1
///\brief Constante Arquitectura 64bits
#define T3_ARCHITECTURE_64 2

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define T3_PLATFORM T3_PLATFORM_WIN32

#elif defined( __APPLE_CC__)
#   define T3_PLATFORM T3_PLATFORM_APPLE

#else

///\brief Constante Plataforma
#   define T3_PLATFORM T3_PLATFORM_LINUX
#endif


#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define T3_ARCH_TYPE T3_ARCHITECTURE_64
#else

///\brief Constante Arquitectura
#   define T3_ARCH_TYPE T3_ARCHITECTURE_32
#endif
