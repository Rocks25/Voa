/*
 * vector.h
 *
 *  Created on: Feb 5, 2013
 *      Author: Sean Bryant
 */

#pragma once


/************************************************************
*				Used for standard Vectors					*
************************************************************/
template<typename T>
class Vector
{
private:
	/********************************************************
		Name: GetMaxElem
		Description: Gets the maximum number of elements	
					 the vector can currently hold			
	********************************************************/
	int GetMaxElem(){return _Max;}
	
	
	/********************************************************
		Name: SetMem										
		Description: Increases/Decreases the size of the	
					 vector									
	********************************************************/
	void SetMem(int newsize){
		if(newsize<_Size)
			return;
		T *tmp = new T[newsize];
		for(int i=0;i<_Size;i++)
		{
			tmp[i]=Data[i];
		}
		delete Data;
		Data = tmp;
		_Max=newsize;
	};

public:
	T *Data;
	int _Max;
	int _Size;
	int _Mod;
public:
	/********************************************************
		Name: Vector										
		Description: Constructor							
	********************************************************/
	Vector(){
		_Max=10;
		_Size=0;
		_Mod=10;

		Data=new T[_Max];
	};
	
	
	/********************************************************
		Name: ~Vector										
		Description: Desctructor							
	********************************************************/
	~Vector(){ delete Data; };

	
	/********************************************************
		Name: Append										
		Description: Append element to end of list			
	********************************************************/
	inline void Append(T x){
		if(_Size+1 >= _Max)
		{
			SetMem(_Max+_Mod);
		}
		Data[_Size]=x;
		_Size++;
	};

	
	/********************************************************
	*	Name: InsertAt										*
	*	Description: Inserts element at specified location	*
	********************************************************/
	inline void InsertAt(int x, T data)
	{
		// If index is less that 0, set it to 0
		if(x < 0)
		{
			x=0;
		}
		// If index is greater than the size of the vector, set the index to the last element + 1
		if(x>_Size)
		{
			x=_Size+1;
		}
		// If the new size of the vector is larger than the number of
		// elements we can currently support
		if(_Size+1 >= _Max)
		{
			// Increase the maximum size of the vector
			_Max += _Mod;
		}
		// Create and initialize tmp variable
		T *tmp = new T[_Max];
		// Create and initialize counter variable
		int counter = 0;
		// Iterate through the vector
		if(int i=0; i<x; i++)
		{
			// If the current index is the place we want to insert
			if(i==x)
			{
				// Add data to the vector
				tmp[i] = data;
			}
			// Otherwise
			else
			{
				// Add element from Data at counter to the tmp variable at i
				tmp[i] = Data[counter];
			}
		}
		// After we are done, delete the old vector
		delete Data;
		// point Data to the new vector
		Data = tmp;
	}

	/********************************************************
	*	Name: DeleteAt										*
	*	Description: Deletes element at specified location	*
	********************************************************/
	inline void DeleteAt(int x)
	{
		// if specified index is out of range
		if(x < 0 || x > Size)
		{
			// Throw an exeption and return
			throw "Index out of range for DeleteAt function";
			return;
		}
		// If the new size is low enough
		if(_Size-1 < _Max-_Mod)
		{
			//  We should decrease our size 
			SetMem(_Max-_Mod);
		}
		// Create and initialize tmp variable
		T *tmp = new T[_Max];
		// Create and initialize a counter
		int count = 0;
		// Iterate through elements
		for(int i=0; i<Size; i++)
		{
			// If the current index is the one we are looking for
			if(i==x)
			{
				// delete the element out of memory and skip to the next element
				delete Data[x];
			}
			else
			{
				// Otherwise add the element at location i in data to tmp variable at the counter's location 
				tmp[count] = Data[i];
				// Increment count
				count++;
			}
		}
		// delete the old array
		delete Data;
		// point the data array to the new array
		Data = tmp;
		// Otherwise
		else
		{
			// Just decrement the Size variable
			_Size--;
		}
	}

	
	/********************************************************
	*	Name: Pop											*
	*	Description Pops an element off the end of the list	*
	********************************************************/
	inline T Pop()
	{
		T tmp;
		if(_Size>0)
		{
			if(_Size-1 < _Max-_Mod)
			{
				SetMem(_Max-_Mod);
			}
			tmp = Data[_Size];
			_Size--;
		}
		return tmp;
	};
	
	
	/********************************************************
	*	Name: Clear											*
	*	Description: Empties out the array					*
	********************************************************/
	inline void Clear()
	{
		while(_Size>0)
		{
			Pop();
		}
	};

	
	/********************************************************
	*	Name: IsEmpty										*
	*	Description: Checks if the vector is empty			*
	********************************************************/
	inline bool IsEmpty()
	{
		return (_Size<=0);
	};
	
	/********************************************************
	*	Name: Size											*
	*	Description: Returns the number of elements in		*
	*				 the vector								*
	********************************************************/
	inline unsigned int Size()
	{
		return _Size;
	};
	
	/********************************************************
	*	Name: GetAt											*
	*	Description: Gets a specific element from the vector*
	********************************************************/
	inline T GetAt(int at){
		if(Empty())
			throw "Cannot \'GetAt\' from empty list!\n";
		if(at < 0)
		{
			throw "Negative index not allowed!\n";
		}
		if(at >= _Max)
		{
			throw "Index greater than number of elements!\n";
		}
		return Data[at];
	};
	
	/********************************************************
	*	Name: Last											*
	*	Description: Returns the last element in the vector	*
	********************************************************/
	inline T End()
	{
		return GetAt(_Size-1);
	};
	
	/********************************************************
	*	Name: Last											*
	*	Description: Returns the first element in the vector*
	********************************************************/
	inline T Begin()
	{
		return GetAt(_Size-1);
	};
	
	/********************************************************
	*	Name: operator[]									*
	*	Description: Same as GetAt							*
	********************************************************/
	inline T operator[](int x){return GetAt(x);};
	
	/********************************************************
	*	Name: operator+=									*
	*	Description: Appends element to the end of the		*
	*				 vector									*
	********************************************************/
	inline T operator +=(T x){return Append(x);};
};

/************************************************************
*				Used for Vectors of pointers				*
************************************************************/

template<class T>
class Vector<T *>
{
private:
	
	/********************************************************
	*	Name: GetMaxElem									*
	*	Description: Gets the maximum number of elements	*
	*				 the vector can currently hold			*
	********************************************************/
	int GetMaxElem(){return _Max;}
	
	
	/********************************************************
	*	Name: SetMem										*
	*	Description: Increases/Decreases the size of the	*
	*				 vector									*
	********************************************************/
	void SetMem(int newsize){
		if(newsize<_Size)
			return;
		T **tmp = new T[newsize];
		for(int i=0;i<_Size;i++)
		{
			tmp[i]=Data[i];
		}
		delete[] Data;
		Data = tmp;
		_Max=newsize;
	};

public:
	T **Data;
	int _Max;
	int _Size;
	int _Mod;
public:
	
	
	/********************************************************
	*	Name: Vector										*
	*	Description: Constructor							*
	********************************************************/
	Vector(){
		_Max=10;
		_Size=0;
		_Mod=10;

		Data=new T*[_Max];
	};
	
	
	/********************************************************
	*	Name: ~Vector										*
	*	Description: Desctructor							*
	********************************************************/
	~Vector(){ delete[] Data; };

	
	/********************************************************
	*	Name: Append										*
	*	Description: Append element to end of list			*
	********************************************************/
	inline void Append(T *x){
		if(_Size+1 >= _Max)
		{
			SetMem(_Max+_Mod);
		}
		Data[_Size]=x;
		_Size++;
	};

	
	/********************************************************
	*	Name: InsertAt										*
	*	Description: Inserts element at specified location	*
	********************************************************/
	inline void InsertAt(int x, T *data)
	{
		// If index is less that 0, set it to 0
		if(x < 0)
		{
			x=0;
		}
		// If index is greater than the size of the vector, set the index to the last element + 1
		if(x>_Size)
		{
			x=_Size+1;
		}
		// If the new size of the vector is larger than the number of
		// elements we can currently support
		if(_Size+1 >= _Max)
		{
			// Increase the maximum size of the vector
			_Max += _Mod;
		}
		// Create and initialize tmp variable
		T **tmp = new T[_Max];
		// Create and initialize counter variable
		int counter = 0;
		// Iterate through the vector
		if(int i=0; i<x; i++)
		{
			// If the current index is the place we want to insert
			if(i==x)
			{
				// Add data to the vector
				tmp[i] = data;
			}
			// Otherwise
			else
			{
				// Add element from Data at counter to the tmp variable at i
				tmp[i] = Data[counter];
			}
		}
		// After we are done, delete the old vector
		delete[] Data;
		// point Data to the new vector
		Data = tmp;
	}

	/********************************************************
	*	Name: DeleteAt										*
	*	Description: Deletes element at specified location	*
	********************************************************/
	inline void DeleteAt(int x)
	{
		// if specified index is out of range
		if(x < 0 || x > Size)
		{
			// Throw an exeption and return
			throw "Index out of range for DeleteAt function";
			return;
		}
		// If the new size is low enough
		if(_Size-1 < _Max-_Mod)
		{
			//  We should decrease our size 
			SetMem(_Max-_Mod);
		}
		// Create and initialize tmp variable
		T **tmp = new T[_Max];
		// Create and initialize a counter
		int count = 0;
		// Iterate through elements
		for(int i=0; i<Size; i++)
		{
			// If the current index is the one we are looking for
			if(i==x)
			{
				// delete the element out of memory and skip to the next element
				delete Data[x];
			}
			else
			{
				// Otherwise add the element at location i in data to tmp variable at the counter's location 
				tmp[count] = Data[i];
				// Increment count
				count++;
			}
		}
		// delete the old array
		delete[] Data;
		// point the data array to the new array
		Data = tmp;
		// Otherwise
		else
		{
			// Just decrement the Size variable
			_Size--;
		}
	}

	
	/********************************************************
	*	Name: Pop											*
	*	Description Pops an element off the end of the list	*
	********************************************************/
	inline T Pop()
	{
		T *tmp=0;
		if(_Size>0)
		{
			if(_Size-1 < _Max-_Mod)
			{
				SetMem(_Max-_Mod);
			}
			tmp = Data[_Size];
			delete Data[_Size];
			_Size--;
		}
		return tmp;
	};
	
	
	/********************************************************
	*	Name: Clear											*
	*	Description: Empties out the array					*
	********************************************************/
	inline void Clear()
	{
		while(_Size>0)
		{
			Pop();
		}
	};

	
	/********************************************************
	*	Name: IsEmpty										*
	*	Description: Checks if the vector is empty			*
	********************************************************/
	inline bool IsEmpty()
	{
		return (_Size<=0);
	};
	
	/********************************************************
	*	Name: Size											*
	*	Description: Returns the number of elements in		*
	*				 the vector								*
	********************************************************/
	inline unsigned int Size()
	{
		return _Size;
	};
	
	/********************************************************
	*	Name: GetAt											*
	*	Description: Gets a specific element from the vector*
	********************************************************/
	inline T *GetAt(int at){
		if(IsEmpty())
			throw "Cannot \'GetAt\' from empty list!\n";
		if(at < 0)
		{
			throw "Negative index not allowed!\n";
		}
		if(at >= _Max)
		{
			throw "Index greater than number of elements!\n";
		}
		return Data[at];
	};
	
	/********************************************************
	*	Name: Last											*
	*	Description: Returns the last element in the vector	*
	********************************************************/
	inline T *Last()
	{
		return GetAt(_Size-1);
	};
	
	/********************************************************
	*	Name: Last											*
	*	Description: Returns the first element in the vector*
	********************************************************/
	inline T *First()
	{
		return GetAt(_Size-1);
	};
	
	/********************************************************
	*	Name: operator[]									*
	*	Description: Same as GetAt							*
	********************************************************/
	inline T *operator[](int x){return GetAt(x);};
	
	/********************************************************
	*	Name: operator+=									*
	*	Description: Appends element to the end of the		*
	*				 vector									*
	********************************************************/
	inline T *operator +=(T *x){return Append(x);};
};