/*
 * vector.h
 *
 *  Created on: Feb 5, 2013
 *      Author: Sean Bryant
 */

#ifndef VECTOR_H_
#define VECTOR_H_

template<class T>
class Vector
{
	T *data;
	int maxElements;
	int noElements;
	int incBy;
public:
	Vector(){
		maxElements=10;
		noElements=0;
		incBy=10;

		data=new T[maxElements];
	};
	~Vector(){ delete[] data; };
	void setMem(int size){
		T *tmp = new T[size];
		for(int i=0;i<noElements;i++)
		{
			tmp[i]=data[i];
		}
		delete[] data;
		data = tmp;
		maxElements=size;
	};
	inline T push(T x){
		if(noElements+1 >= maxElements)
		{
			setMem(maxElements+incBy);
		}
		data[noElements]=x;
		noElements++;
		return x;
	};
	inline T pop()
	{
		T tmp;
		if(noElements>0)
		{
			tmp = data[noElements];
			delete data[noElements];
			noElements--;
		}
	};
	
	inline void clear()
	{
		while(noElements>0)
		{
			pop();
		}
	};
	inline bool empty()
	{
		return (noElements<=0);
	};
	inline unsigned int size()
	{
		return noElements;
	};
	inline T &getAt(int at){
		if(at < 0)
		{
			throw "Negative index not allowed!\n";
		}
		if(at >= maxElements)
		{
			throw "Index greater than number of elements!\n";
		}
		return data[at];
	};
	int getMaxElem(){return maxElements;};
	int getNoElements(){return noElements;};
	inline T & operator[](int x){return getAt(x);};
	inline T operator +=(T x){return append(x);};
};

#endif /* VECTOR_H_ */
