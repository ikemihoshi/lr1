#include<string>
#include<vector>
#include<list>
#include<stack>
#include<map>
#include<algorithm>

using namespace std;

#ifndef _C_STRUCT_H_
#define _C_STRUCT_H_

template <class T>
//重新分配缓冲区大小,返回新的缓冲区大小
unsigned int resize(T**oldBuf, unsigned int oldSize, double rate = 2)
{
	unsigned int newSize = oldSize * rate;
	T *bufTmp = new T[newSize+1];
	memcpy(bufTmp, *oldBuf, oldSize);
	delete[](*oldBuf);
	*oldBuf = bufTmp;
	return newSize;
}

template <class T>
class CArray
{
public:
	CArray(unsigned int initBufSize = 0){
		vct.resize(initBufSize);
	}
	void sort(bool isReserve = false){
		if(!isReserve){
			std::sort(vct.begin(), vct.end());
		}
		else{
			std::sort(vct.rbegin(), vct.rend());
		}
	}
	T& operator[](int index){
		if(0 <= index && index < vct.size()){
			return vct[index];
		}
		throw "Index Out Of Range";
	}
	T& get(int index){
		if(0 <= index && index < vct.size()){
			return vct[index];
		}
		throw "Index Out Of Range";
	}
	bool set(int index , T t){
		if(0 <= index && index < vct.size()){
			vct[index] = t;
			return true;
		}
		return false;
	}
	int size(){
		return vct.size();
	}
	bool add(T t){
		vct.push_back(t);
		return true;
	}
	bool insert(int index , T t){
		if(index < 0 || index > vct.size()){
			return false;
		}
		vct.insert(vct.begin() + index , t);
		return true;
	}
	bool del(int index){
		if(index < 0 || index >= vct.size()){
			return false;
		}
		vct.erase(vct.begin()+index);
		return true;
	}
	int find(T t , int indexBegin = 0){
		if(indexBegin < 0 || indexBegin >= vct.size()){
			return -1;
		}
		for(int i = indexBegin ; i < vct.size() ; i++){
			if(vct[i] == t){
				return i;
			}
		}
		return -1;
	}
	void empty(){
		vct.clear();
	}
	bool isEmpty(){
		return vct.empty();
	}
	CArray<T>& operator =(CArray<T>&t){
		vct = t.vct;
		return *this;
	}
	CArray<T> clone(){
		CArray<T> ct;
		if(vct.size() == 0){
			return ct;
		}
		ct.vct = vct;
		return ct;
	}
private:
	vector<T> vct;
};

template <class T>
class CStack
{
public:
	void pop()
	{
		st.pop();
	}
	void push(T t)
	{
		st.push(t);
	}
	T& top()
	{
		return st.top();
	}
	bool isEmpty()
	{
		return st.empty();
	}
	int size()
	{
		return st.size();
	}
	void empty()
	{
		while(!isEmpty())
		{
			st.pop();
		}
	}
	CStack<T> clone()
	{
		CStack<T> cs;
		cs.st = st;
		return cs;
	}
	CArray<T> toArray()
	{
		CArray<T> ca(size());
		if(size() == 0)
		{
			return ca;
		}
		CStack<T> cs = this->clone();
		for(int i = size() - 1 ; i >= 0 ; i--)
		{
			ca.set(i , cs.top());
			cs.pop();
		}
		return ca;
	}
private:
	stack<T> st;
};

template <class T>
class CSet
{
public:
	T& get(int index)
	{
		if(0 <= index && index < vct.size())
		{
			return vct[index];
		}
		throw "Index Outof Range";
	}
	bool set(int index , T t)
	{
		if(0 <= index && index < vct.size())
		{
			if(find(t) != -1)
			{
				return false;
			}
			vct[index] = t;
			return true;
		}
		return false;
	}
	int size()
	{
		return vct.size();
	}
	bool add(T t)
	{
		if(isExist(t))
		{
			return false;
		}
		vct.push_back(t);
		return true;
	}
	bool insert(int index , T t)
	{
		if(index < 0 || index > vct.size())
		{
			return false;
		}
		if(isExist(t))
		{
			return false;
		}
		vct.insert(vct.begin() + index , t);
		return true;
	}
	bool del(int index)
	{
		if(index < 0 || index >= vct.size())
		{
			return false;
		}
		vct.erase(vct.begin()+index);
		return true;
	}
	bool del(T& t)
	{
		int index = find(t);
		if(index < 0)
		{
			return false;
		}
		del(index);
		return true;
	}
	int find(T& t)
	{
		for(int i = 0 ; i < vct.size() ; i++)
		{
			if(vct[i] == t)
			{
				return i;
			}
		}
		return -1;
	}
	void empty()
	{
		vct.clear();
	}
	bool isEmpty()
	{
		return vct.empty();
	}
	CSet<T> clone()
	{
		CSet<T> ct;
		if(vct.size() == 0)
		{
			return ct;
		}
		ct.vct = vct;
		return ct;
	}
	bool isExist(T t)
	{
		return find(t) != -1;
	}
	bool operator==(CSet<T> &t)
	{
		if(this->size() != t.size())
		{
			return false;
		}
		return this->and(t).size() == this->size();
	}
	CSet<T>& operator=(CSet<T> &t)
	{
		this->vct = t.vct;
		return *this;
	}
	CSet<T> and(CSet<T> &t)
	{
		CSet<T> cs;
		if(size() == 0 || t.size() == 0)
		{
			return cs;
		}
		int i ;
		for(i = 0 ; i < size() ; i ++)
		{
			if(t.isExist(this->get(i)))
			{
				cs.add(this->get(i));
			}
		}
		return cs;
	}
	CSet<T> or(CSet<T> &t)
	{
		CSet<T> cs = this->clone();
		for(int i = 0 ;i < t.size() ; i++)
		{
			cs.add(t.get(i));
		}
		return cs;
	}
	CArray<T> toArray()
	{
		CArray<T> ca;
		for(int i = 0 ; i < size() ; i++)
		{
			ca.add(get(i));
		}
		return ca;
	}
private:
	vector<T> vct;
};

template <class T, class U>
class CMap
{
public :
	void erase(T t)			//return false if t not existed
	{
		mp.erase(t);
	}
	U& get(T t)
	{
		if(isDefined(t))
		{
			return mp[t];
		}
		throw "Key-Value not Exist!";
	}

	U& operator[](T t){
		return get(t);
	}
	void empty()
	{
		mp.clear();
	}
	bool isEmpty()
	{
		return mp.empty();
	}
	void set(T t, U u)
	{
		mp[t] = u;
	}
	bool setWithCheck(T t, U u)		//return false if t already existed
	{
		if(isDefined(t))
		{
			return false;
		}
		set(t, u);
		return true;
	}
	bool isDefined(T t)
	{
		return mp.count(t) > 0;
	}
	CArray<T> getKeys()
	{
		CArray<T> ct;
		map<T,U>::iterator it = mp.begin();
		while(it != mp.end())
		{
			ct.add((*it).first);
			++it;
		}
		return ct;
	}
	int size()
	{
		return mp.size();
	}
	CMap<T,U> clone()
	{
		CMap<T,U> cpTmp;
		cpTmp.mp = mp;
		return cpTmp;
	}
private:
	map<T,U> mp;

};

#endif