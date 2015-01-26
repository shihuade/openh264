// SimpleCaseDll.cpp : Defines the exported functions for the DLL application.
//

//#include "SimpleCaseDll.h"

#include "SimpleTestCase.h"


int MySimpleTest()
{

	//return 10; // 
	TestAll();
	return 10;

}

int TestAllCases(int argc, char** argv)
{

	return CodecUtMain(argc, argv);

}
