// giraffe.cpp
#include "stdafx.h"
#include "giraffe.h"



Giraffe::Giraffe(int id_in) : id(id_in)
{
}

int Giraffe::GetID()
{
	return id;
}

int GiraffeFactory::nextID = 0;

GiraffeFactory::GiraffeFactory()
{
	nextID = 0;
}

int GiraffeFactory::GetNextID()
{
	return nextID;
}