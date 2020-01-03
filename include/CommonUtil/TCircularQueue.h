#pragma once

#define INIT_QUEUE_SIZE 1024
#define ADVANCED_FUNCTION
//#define USE_VECTOR
#include <stdio.h>
#include <string.h>

#ifdef USE_VECTOR
#include <vector>
using namespace std;

template<class T>
class TCircularQueue : public vector<T>
{
public:
	TCircularQueue (int size = INIT_QUEUE_SIZE, bool _unlimited = false)
	{
		max_size = size;
	}
	bool is_empty()
	{	
		return size()==0 ? true : false;
	}

	bool is_full()
	{
		return size()>=max_size ? true : false;
	}

	T pop()
	{
		return pop_front();
	}

	void push(T &value)
	{
		push_back(value);
	}

	T pop_front()
	{	
		T value = front();
		erase(begin());
		return value;
	}

	T& get_first() //const
	{
		return front();
	}

	T& get_at(int i) //const
	{
		return at(i);
	}

	//������ ����Ÿ ���� ������ �Լ�
	T& get_last() //const
	{
		return back();
	}

	void resize(unsigned int _size)
	{
		while(size() > _size)
		{
			pop_front();
		}
		
		max_size = _size;
	}

	
private:
	unsigned int max_size;
};

#else

template<class T>
class TCircularQueue
{
private:
	//�������� ������ ����Ŵ
	int _front;

	//�������� ���� ����Ŵ
	int _rear;

	//��ü ������
	int _max_size;

	//���� ������
	int _cur_size;

	//���� ����
	T* _queue;

	//full�ÿ� �ڵ� �������� ����
	bool _unlimited;

public:

	TCircularQueue(int size = INIT_QUEUE_SIZE, bool unlimited = false)
	{	
		_queue = NULL;
		reserve(size);
		_unlimited = unlimited;
	}
	//���� ������
	TCircularQueue(const TCircularQueue<T>& v)
	{
		_queue = NULL;
		copy(v);
	}

	~TCircularQueue()
	{
		clear();
		free_queue();
	}

	void reserve(int size)
	{
		//front �� rear���� guard ������ ���� 1�� ũ����
		_max_size = size + 1;

		free_queue();
		init();
	}
	//������ ũ�⸦ �缳����
	void resize(int size)
	{
		//���ο� �Ҵ� ũ�� ����
		int new_maxSize = size + 1;
		//���ο� ũ�⸦ �Ҵ��ϰ� ���������� ������
		T* new_queue = new T[new_maxSize];

		if(_front < _rear){
			memcpy(new_queue + 1, _queue + (_front + 1), sizeof(T) * _cur_size);
		}
		else if(_front > _rear)
		{
			int offset = _max_size - (_front + 1);
			memcpy(new_queue + 1, _queue + (_front + 1), sizeof(T) * offset);
			memcpy(new_queue + 1 + offset, _queue, sizeof(T) * (_rear + 1));
		}
		else //�ƹ��͵� ���°�� �޸� ���� ���� ���°��� ����
		{

		}
		//for(int i=0; i<cur_size; i++)
		//	new_queue[i] = get_at(i);

		free_queue();			//���� �Ҵ��� ť�� ������

		_queue = new_queue;		//���� ������ ť�� �Ҵ���
		_front = 0;				//��ó������ ä�����Ƿ� front�� ������ 0 
		_rear = _cur_size;			//rear�� ���� �����ŭ ��ġ �̵�
		_max_size = new_maxSize;	//���� �Ҵ�� ũ�� ����
	}

	void unlimited()
	{
		_unlimited = true;
	}
	//��� ���� �����ϴ� �Լ�
	void clear()
	{
		_front = 0;
		_rear = 0;
		_cur_size = 0;
	}
/*
	//���� �߰��ϴ� �Լ�
	void add_first(T value) 
	{
		push(value);
	}

	//���� �����ϴ� �Լ�
	T& remove_last()
	{
		return pop();
	}
*/
	//��� �ִ� �� üũ
	bool is_empty()
	{	//rear�� front�� ���� ��� ��� ����
		return (_rear == _front);
	}

	bool is_full()
	{
		return ( (_rear + 1) % _max_size == _front );
	}

	T pop()
	{
		return pop_front();
	}

	void push(T &value)
	{
		push_back( value);
	}

	T pop_front()
	{	
		_cur_size--;

		_front = (_front+1)%_max_size;
		return _queue[_front];
	}

	void push_back(T &value)
	{
		//b_unlimited ���ο� ���� full�����ΰ�� ���� ���� �����ϰų� ������¡��
		if(is_full())//2011.09.05 VxWorks ������ ������ ���� �ڵ� ����  
		{	
			if(_unlimited == true)//UNLIMITED ����� ��� ����� 2�� ũ��� ������
				resize((_max_size - 1) * 2);
			else//���� ó�� ����Ÿ�� �˾���
				pop_front(); 
		}

		_rear = (_rear+1)%_max_size;
		_queue[_rear] = value;

		//���� ������ ����
		_cur_size++;
	}

#ifdef ADVANCED_FUNCTION //Advanced Function
	T pop_back()
	{	
		_cur_size--;

		int tmp = _rear;
		_rear = _rear-1;
		if(_rear < 0)
			_rear+= _max_size;
		return _queue[tmp];
	}

	void push_front(const T &value)
	{
		push_front((T) value);
	}

	void push_front(T &value)
	{
		//b_unlimited ���ο� ���� full�����ΰ�� ���� ���� �����ϰų� ������¡��
		if(is_full())// VxWorks ������ ������ ���� �ڵ� ����  
		{	
			if(_unlimited == true)//UNLIMITED ����� ��� ����� 2�� ũ��� ������
				resize((_max_size - 1) * 2);
			else//���� ó�� ����Ÿ�� �˾���
				pop(); 
		}

		_queue[_front] = value;
		_front = _front-1;
		if(_front < 0)
			_front += _max_size;

		//���� ������ ����
		_cur_size++;
	}
#endif

	//����� ���ϴ� �Լ�
	size_t size() const
	{
		return _cur_size;
	}

	size_t capacity() const
	{
		return _max_size - 1;
	}

	//�ش� �ε��� ���� ��� �Լ�
	T& get_at(int i) //const
	{
		int tmp = (_front+i+1)%_max_size;
		return _queue[tmp];
	}

	const T& get_at(int i) const
	{
#if 1
		int tmp = (_front+i+1)%_max_size;
		return _queue[tmp];
#else
		if( (_max_size - 1) - _front < i)
			return _ueue[i - (_max_size - _front)];
		//return queue[i - (front % max_size)];
		else
			return _queue[_front + i];
#endif
	}


	//ó�� ����Ÿ ���� ������ �Լ�
	T& get_first() //const
	{
		return get_at(0);
	}

	T& front() //const
	{
		return get_at(0);
	}

	//������ ����Ÿ ���� ������ �Լ�
	T& get_last() //const
	{
		return get_at(size() - 1);
	}

	T& back() //const
	{
		return get_at(size() - 1);
	}

	//���� ������
	TCircularQueue<T>& operator=(const TCircularQueue<T> & q)
	{
		copy(q);
	}

	//÷�� ������
	T& operator[](int i) //const
	{
		return get_at(i);//static_cast<T> (get_at(i));
	}

	const T& operator[](int i) const
	{
		return get_at(i);
	}

	//��ü�� �����ϴ� �Լ�
	void copy(const TCircularQueue<T>& q)
	{
#if 0
		_max_size = q._max_size;

		init();
		for(unsigned int i=0; i<q.size(); i++)
			push(q.get_at(i));
#else
		//���ο� ũ�⸦ �Ҵ��ϰ� ���������� ������
		_queue = new T[q.max_size];


		if(q._front < q._rear){
			memcpy(_queue, q._queue + (q._front + 1), sizeof(T) * q._cur_size);
		}
		else 
		{
			int offset = q._max_size - (q._front + 1);
			memcpy(_queue, q._queue + (_front + 1), sizeof(T) * offset);
			memcpy(_queue + offset, q._queue, sizeof(T) * (q._rear + 1));
		}

		_front = 0;				//��ó������ ä�����Ƿ� front�� ������ 0 
		_rear = q._cur_size;			//rear�� ���� �����ŭ ��ġ �̵�
		_cur_size = q._cur_size;
		_max_size = q._max_size;
		_unlimited = q._unlimited;
#endif
	}
private:
	void init()
	{
		clear();

		_queue = new T[_max_size];
	}

	void free_queue()
	{
		if(_queue != NULL)
			delete _queue;
		_queue = NULL;
	}
};
#endif

