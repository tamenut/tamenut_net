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

	//마지막 데이타 값을 얻어오는 함수
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
	//데이터의 시작을 가리킴
	int _front;

	//데이터의 끝을 가리킴
	int _rear;

	//전체 사이즈
	int _max_size;

	//현재 사이즈
	int _cur_size;

	//내부 버퍼
	T* _queue;

	//full시에 자동 증가여부 결정
	bool _unlimited;

public:

	TCircularQueue(int size = INIT_QUEUE_SIZE, bool unlimited = false)
	{	
		_queue = NULL;
		reserve(size);
		_unlimited = unlimited;
	}
	//복사 생성자
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
		//front 와 rear간의 guard 역할을 위해 1을 크게함
		_max_size = size + 1;

		free_queue();
		init();
	}
	//사이즈 크기를 재설정함
	void resize(int size)
	{
		//새로운 할당 크기 설정
		int new_maxSize = size + 1;
		//새로운 크기를 할당하고 이전값들을 복사함
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
		else //아무것도 없는경우 메모리 복사 없이 상태값만 변경
		{

		}
		//for(int i=0; i<cur_size; i++)
		//	new_queue[i] = get_at(i);

		free_queue();			//이전 할당한 큐를 삭제함

		_queue = new_queue;		//새로 생성한 큐를 할당함
		_front = 0;				//맨처음부터 채워지므로 front는 무조건 0 
		_rear = _cur_size;			//rear는 기존 사이즈만큼 위치 이동
		_max_size = new_maxSize;	//새로 할당된 크기 설정
	}

	void unlimited()
	{
		_unlimited = true;
	}
	//모든 값을 삭제하는 함수
	void clear()
	{
		_front = 0;
		_rear = 0;
		_cur_size = 0;
	}
/*
	//값을 추가하는 함수
	void add_first(T value) 
	{
		push(value);
	}

	//값을 제거하는 함수
	T& remove_last()
	{
		return pop();
	}
*/
	//비어 있는 지 체크
	bool is_empty()
	{	//rear와 front가 같은 경우 비어 있음
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
		//b_unlimited 여부에 따라 full상태인경우 이전 값을 삭제하거나 리사이징함
		if(is_full())//2011.09.05 VxWorks 컴파일 오류에 따른 코드 변경  
		{	
			if(_unlimited == true)//UNLIMITED 모드인 경우 사이즈를 2배 크기로 변경함
				resize((_max_size - 1) * 2);
			else//가장 처음 데이타를 팝업함
				pop_front(); 
		}

		_rear = (_rear+1)%_max_size;
		_queue[_rear] = value;

		//현재 사이즈 증가
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
		//b_unlimited 여부에 따라 full상태인경우 이전 값을 삭제하거나 리사이징함
		if(is_full())// VxWorks 컴파일 오류에 따른 코드 변경  
		{	
			if(_unlimited == true)//UNLIMITED 모드인 경우 사이즈를 2배 크기로 변경함
				resize((_max_size - 1) * 2);
			else//가장 처음 데이타를 팝업함
				pop(); 
		}

		_queue[_front] = value;
		_front = _front-1;
		if(_front < 0)
			_front += _max_size;

		//현재 사이즈 증가
		_cur_size++;
	}
#endif

	//사이즈를 구하는 함수
	size_t size() const
	{
		return _cur_size;
	}

	size_t capacity() const
	{
		return _max_size - 1;
	}

	//해당 인덱스 값을 얻는 함수
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


	//처음 데이타 값을 얻어오는 함수
	T& get_first() //const
	{
		return get_at(0);
	}

	T& front() //const
	{
		return get_at(0);
	}

	//마지막 데이타 값을 얻어오는 함수
	T& get_last() //const
	{
		return get_at(size() - 1);
	}

	T& back() //const
	{
		return get_at(size() - 1);
	}

	//복사 연산자
	TCircularQueue<T>& operator=(const TCircularQueue<T> & q)
	{
		copy(q);
	}

	//첨자 연산자
	T& operator[](int i) //const
	{
		return get_at(i);//static_cast<T> (get_at(i));
	}

	const T& operator[](int i) const
	{
		return get_at(i);
	}

	//객체를 복사하는 함수
	void copy(const TCircularQueue<T>& q)
	{
#if 0
		_max_size = q._max_size;

		init();
		for(unsigned int i=0; i<q.size(); i++)
			push(q.get_at(i));
#else
		//새로운 크기를 할당하고 이전값들을 복사함
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

		_front = 0;				//맨처음부터 채워지므로 front는 무조건 0 
		_rear = q._cur_size;			//rear는 기존 사이즈만큼 위치 이동
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

