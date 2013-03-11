#pragma once

class BasicObject
{
public:
	BasicObject(void);
	virtual ~BasicObject(void);
	virtual void Render();
	virtual void Init();
};

