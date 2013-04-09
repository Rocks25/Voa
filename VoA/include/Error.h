#pragma once

#include "vector.h"

class Error
{
private:

public:
	Error(void);
	~Error(void);
	static void Print(const char *text);
};

