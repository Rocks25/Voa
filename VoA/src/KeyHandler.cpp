#include "../include/KeyHandler.h"

KeyHandler *KeyHandler::_instance = NULL;

KeyHandler::KeyHandler(void)
{
}


KeyHandler::~KeyHandler(void)
{
	delete _instance;
}

KeyHandler *KeyHandler::GetSingleton()
{
	static KeyHandler instance;
	return &instance;
}

void KeyHandler::SetKey(int key)
{
	if(key < 323 && key >=0)
		_keys[key]=true;
}

void KeyHandler::ReleaseKey(int key)
{
	if(key < 323 && key >= 0)
		_keys[key]=false;
}

bool KeyHandler::IsKeyDown(int key)
{
	if(key < 323 && key >=0)
		return _keys[key];
	return false;
}


bool KeyHandler::AddAction(KeyAction kAct)
{
	for(unsigned int i=0;i<_actions.size();i++)
	{
		if(_actions[i].name==kAct.name)
			return false;
	}
	_actions.push_back(kAct);
	return true;
}

bool KeyHandler::AddAction(char *name, int key, bool state, void *(*Action)(void))
{
	for(unsigned int i=0;i<_actions.size();i++)
	{
		if(_actions[i].name==name)
			return false;
	}
	KeyAction kAct;
	kAct.name = name;
	kAct.Key = key;
	kAct.State = state;
	kAct.Action = Action;
	return true;
}

bool KeyHandler::RemoveAction(char *name)
{
	for(unsigned int i=0;i<_actions.size();i++)
	{
		if(_actions[i].name==name)
		{
			_actions.erase(_actions.begin()+i);
			return true;
		}
	}
	return false;
}