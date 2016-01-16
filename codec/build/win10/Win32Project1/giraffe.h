// giraffe.h
#pragma once

#ifdef _DLL
#define GIRAFFE_API __declspec(dllexport)
#else
#define GIRAFFE_API 
#endif

GIRAFFE_API int giraffeFunction();



class Giraffe
{
	int id;
	Giraffe(int id_in);
	friend class GiraffeFactory;

public:
	GIRAFFE_API int GetID();
};

class GiraffeFactory
{
	static int nextID;

public:
	GIRAFFE_API GiraffeFactory();
	GIRAFFE_API static int GetNextID();
	GIRAFFE_API static Giraffe* Create();
};