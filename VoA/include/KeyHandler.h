#pragma once

#include <vector>

#define KS_DOWN true;
#define KS_UP false;


typedef struct
{
	char *name;
	int Key;
	bool State;
	void *(*Action)(void);
}KeyAction;

class KeyHandler
{
private:
	bool _keys[323];
	std::vector<KeyAction> _actions;
	static KeyHandler *_instance;
protected:
	KeyHandler(void);
	~KeyHandler(void);
public:
	static KeyHandler *GetSingleton();
	void SetKey(int key);
	void ReleaseKey(int key);
	bool IsKeyDown(int key);
	bool AddAction(KeyAction kAct);
	bool AddAction(char *name, int key, bool state, void *(*Action)(void));
	bool RemoveAction(char *name);
};

