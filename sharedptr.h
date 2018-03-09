#include<iostream>
#include<memory>
#include<functional>
#include<mutex>
#include<atomic>
//对象类。
class Reference {
public:
	std::atomic<int> referenceCounter; // 处理referenceCounter的时候就变成了原子操作
	Reference() {
		referenceCounter = 0;
	}
	virtual ~Reference() {};
};

template<class T>
class ReferenceObject :public Reference {
public: 
	T* data;
	ReferenceObject() {
		data = NULL;
		this->referenceCounter = 0;
	}
	ReferenceObject(T* temp) {
		data = temp;
		referenceCounter++;
	}
	~ReferenceObject() {
		delete this->data;
		data = NULL;
		referenceCounter = 0;
	}
};
template <typename T>
class SharedPtr {
public:
	Reference *refObject;
	T* data;

	SharedPtr() {
		data = NULL;
		refObject = NULL;
	}
	SharedPtr(T* temp) {
		refObject = new ReferenceObject(temp);
		data = temp;
	}
	~SharedPtr() {
		if (refObject != NULL) {
			if (refObject->referenceCounter == 1)
			{
				refObject->referenceCounter = 0;
				delete refObject;
				refObject = NULL;
				data = NULL;


			}
			else refObject->referenceCounter--;
		
		}
	}
	Sharetr(const SharedPtr &p) {
		data = p.data;
		refObject = p.refObject;
		if (refObject != NULL)
			refObject->referenceCounter++;
	}
	template<typename U>
	SharedPtr(const SharedPtr<U>&p) {
		data = static_cast<T*>(p.data);
		refObject = (ReferenceObject<T>*)p.refObject;
		if (refObject != NULL)
			refObject->referenceCounter++;
			
	}
	SharedPtr &operator= (const SharedPtr &p)
	{
		if (this != &p)
		{
			if (refObject != NULL)
			{	 // 
				if (refObject->referenceCounter == 1)
				{
					refObject->referenceCounter = 0;
					delete this->refObject;
					this->refObject = NULL;
					this->data = NULL;
				}
			}
		}
	
	
	}


};
