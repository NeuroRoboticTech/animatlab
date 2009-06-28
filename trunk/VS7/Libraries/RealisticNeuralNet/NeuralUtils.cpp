#include "stdafx.h"
#include "ClassFactory.h"

string Rn_NeuralModuleName()
{
	return "RealisticNeuralNet";
}


extern "C" __declspec(dllexport) IStdClassFactory* __cdecl GetStdClassFactory() 
{
	IStdClassFactory *lpFactory = new ClassFactory;
	return lpFactory;
}



