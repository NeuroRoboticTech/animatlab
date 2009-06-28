#ifndef __BOOTSTRAP_LOADER_DLL_H__
#define __BOOTSTRAP_LOADER_DLL_H__

#ifndef _BOOTSTRAP_LOADER_DLL_NOFORCELIBS
	#if _MSC_VER > 1300  // VC 7
		#ifdef _DEBUG
			#pragma comment(lib, "BootStrapLoader_vc7D.lib")
		#else
			#pragma comment(lib, "BootStrapLoader_vc7.lib")
		#endif          
	#else   // VC 6
		#ifdef _DEBUG
			#pragma comment(lib, "BootStrapLoader_vc6D.lib")
		#else
			#pragma comment(lib, "BootStrapLoader_vc6.lib")
		#endif         
	#endif          
#endif          // _BOOTSTRAP_LOADER_DLL_NOFORCELIBS

#define BOOTSTRAP_LOADER_PORT __declspec( dllimport )

#pragma warning(disable: 4018 4244 4290 4786 4251 4275 4267 4311)

#include <config/stlcomp.h>
#include <direct.h>
#include <conio.h>
#include <exception>
#include <string>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <stdio.h>
#include <io.h>
#include <iomanip.h>
#include <cctype>
#include <math.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/timeb.h>
using namespace std;

int BOOTSTRAP_LOADER_PORT BootStrap_RunLibrary(int argc, const char **argv);


#endif // __BOOTSTRAP_LOADER_DLL_H__
