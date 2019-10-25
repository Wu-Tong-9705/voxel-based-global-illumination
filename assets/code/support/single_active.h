#pragma once
#include <memory>

//ȫ�ַ���ģ���ࣨͬһ����ֻ��һ�������ܼ��
template<class T> class SingleActive
{
	public:
		static void ResetActive();//ȡ������
		virtual void SetAsActive();//����
		bool IsActive();//�Ƿ񼤻�
		
		static std::unique_ptr<T> &Active();//���ص�����������ȫ�ַ���

	protected:
		static std::unique_ptr<T> current;//ָ��
		~SingleActive();
};

template<class T>
std::unique_ptr<T> SingleActive<T>::current = nullptr;

template<class T>
SingleActive<T>::~SingleActive()
{
	//����ö���Ϊ��������ͷ�ָ��Ŀ���Ȩ�������ⲿɾ����ĪҪ�������
	if (static_cast<T *>(this) == current.get())
	{
		current.release();
	}
}

template <class T>
void SingleActive<T>::ResetActive()
{
	current.release();
	current.reset(static_cast<T *>(nullptr));
}

template<class T>
void SingleActive<T>::SetAsActive()
{
	current.release();
	current.reset(static_cast<T *>(this));
}

template <class T>
bool SingleActive<T>::IsActive()
{
	return current.get() == static_cast<T *>(this);
}

template<class T>
std::unique_ptr<T> &SingleActive<T>::Active()
{
	return current;
}
