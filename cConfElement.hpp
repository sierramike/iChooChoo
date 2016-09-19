#pragma once

#include <iostream>
#include <string.h>

using namespace std;

class cConfElement
{
	public:
		cConfElement();
		~cConfElement();
		virtual void setID(int iD);
		virtual int getID();
		virtual void setDescription(const char* description);
		virtual char* getDescription();

	private:
		int _iID;
		char* _sDescription;
};
