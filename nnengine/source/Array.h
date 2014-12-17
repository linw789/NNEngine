/*
	file: Array.h

*/

#ifndef NNE_ARRAY_H
#define NNE_ARRAY_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include <vector>

namespace NNE{

/*
	#define and other prepocessor directives are not
	restrict to namespace. #define Array std::vector
	is globally visible which causes a naming conflict 
	with something in winnt.h file
*/
//#define Array std::vector

/*
	type alias is not supported even in VC++ 11
*/
//template<typename T>
//using Array = std::vector<T>;

// You shouldn't inherit from classes that do not have a virtual destructor. 
// It's related to destructors in derived classes not being called when they 
// should be and you could end up with unallocated memory.
//template<typename T>
//struct Array : public std::vector<T>
//{};


/*
	Dynamic Array, compatible with aligned data storage.
	Warning: 1) be cautious when you push_back an element that already exists
				in the array, especially when the element contains pointer variale members
				or the element itself is a pointer.
			 2) if element has pointer member, the copy constuctor and 
				assignment overload must do shallow copy.
*/
template<typename T, size_t align = 1>
class Array
{
	// the structure was created to work with Iterator
	struct InnerData
	{
		InnerData()
			:dataBegin(nullptr), rawPtr(nullptr), elemSize(0), size(0), capacity(0)
		{}

		T *elemPtr(NNEvoid *startPtr, NNEuint index) const
		{
			debugAssertMsg(index < capacity, "index out of boundary");
			return (T *)((NNEuint)startPtr + index * elemSize);
		}

		T *elemPtr(NNEuint index) const
		{
			return (T *)elemPtr(dataBegin, index);
		}

		// pointer of the first actuall object stored in the array
		NNEvoid *dataBegin;

		NNEvoid *rawPtr;

		// element size including padding for alignment
		size_t elemSize;

		// number of elements
		NNEuint size;

		// number of elements that can be stored in the array
		NNEuint capacity;
	};

public:

	Array()
		:m_increment(5)
	{
		debugAssertMsg(1 == align || 0 == align % 4, "alignment must be times of 4");

		// compute the size of element including alignment if necessary
		size_t misalign = sizeof(T) % align;
		m_data.elemSize = sizeof(T) + (0 == misalign ? 0 : sizeof(T) - misalign);
	}

	Array(NNEuint capacity)
		:m_increment(5)
	{
		debugAssertMsg(1 == align || 0 == align % 4, "alignment must be times of 4");

		// compute the size of element including alignment if necessary
		size_t misalign = sizeof(T) % align;
		m_data.elemSize = sizeof(T) + (0 == misalign ? 0 : sizeof(T) - misalign);

		resize(capacity);
	}

	// initialize with num number of elements of initValue
	Array(T initValue, size_t num)
		:m_increment(5)
	{
		debugAssertMsg(1 == align || 0 == align % 4, "alignment must be times of 4");

		// compute the size of element including alignment if necessary
		size_t misalign = sizeof(T) % align;
		m_data.elemSize = sizeof(T) + (0 == misalign ? 0 : sizeof(T) - misalign);

		resize(num);
		for (NNEint i = 0; i < num; ++i)
		{
			push_back(initValue);
		}
	}

	~Array()
	{
		for (NNEuint i = 0; i < m_data.size; ++i)
		{
			static_cast<T *>(m_data.elemPtr(i))->~T();
		}

		::free(m_data.rawPtr);
	};

	// custom iterator 
	// reference: http://www.oreillynet.com/pub/a/network/2005/11/21/what-is-iterator-in-c-plus-plus-part2.html?page=1
	class Iterator
	{
	public:

		Iterator(NNEuint index, InnerData *arrayData)
			:m_elemIndex(index), m_arrayData(arrayData)
		{}

		~Iterator(){};

		Iterator& operator = (const Iterator& other)
		{
			m_elemIndex = other.m_elemIndex;
			m_arrayData = other.m_arrayData;
			return *this;
		}

		NNEbool operator == (const Iterator& other) const
		{
			return (m_elemIndex == other.m_elemIndex
				&& m_arrayData == other.m_arrayData);
		}

		NNEbool operator != (const Iterator& other) const
		{
			return (m_elemIndex != other.m_elemIndex
				|| m_arrayData != m_arrayData);
		}

		// pre-increment
		Iterator &operator ++ ()
		{
			debugAssertMsg(m_arrayData->size > m_elemIndex, "Access out of boundary!");

			++m_elemIndex;

			return *this;
		}

		// post-increment
		Iterator operator ++ (NNEint)
		{
			debugAssertMsg(m_arrayData->size > m_elemIndex, "Access out of boundary!");

			Iterator tmp(m_elemIndex, m_arrayData);
			++m_elemIndex;
			return tmp;
		}

		// pre-decrement 
		Iterator &operator -- ()
		{
			debugAssertMsg(m_elemIndex > 0, "Access out of boundary!");

			--m_elemIndex;
			return *this;
		}

		// post-decrement
		Iterator operator -- (NNEint)
		{
			debugAssertMsg(m_elemIndex > 0, "Access out of boundary!");

			Iterator tmp(m_elemIndex, m_arrayData);
			--m_elemIndex;
			return tmp;
		}

		// In a const-function, every data member becomes const 
		// in such way that it cannot be modified
		T &operator * () /*const*/
		{
			return *(static_cast<T *>(m_arrayData->elemPtr(m_elemIndex)));
		}	

		T *operator -> () /*const*/
		{
			return static_cast<T *>(m_arrayData->elemPtr(m_elemIndex));
		}

	private:

		NNEuint m_elemIndex;

		InnerData *m_arrayData;
	};
	typedef Iterator iterator;

	Iterator begin()/* const*/
	{
		return Iterator(0, &m_data);
	}

	Iterator end()/* const */
	{
		return Iterator(m_data.size, &m_data);
	}

	NNEvoid reserve(NNEuint size)
	{
		resize(size);
	}

	// 
	T *next()
	{
		if (m_data.size < m_data.capacity)
		{
			new (m_data.elemPtr(m_data.size)) T();
			++m_data.size;
			return m_data.elemPtr(m_data.size - 1);
		}
		else
		{
			if (0 == m_increment)
			{
				resize(m_data.size * 2);
				new (m_data.elemPtr(m_data.size)) T();
				++m_data.size;
				return m_data.elemPtr(m_data.size - 1);
			}
			else
			{
				resize(m_data.size + m_increment);
				new (m_data.elemPtr(m_data.size)) T();
				++m_data.size;
				return m_data.elemPtr(m_data.size - 1);
			}
		}
	}

	// Can't push_back the element that already exists in the array,
	// because when m_size >= m_capacity, resize() will be called, which
	// will delete the old data causing the passed in element's reference dangling
	NNEvoid push_back(const T& obj)
	{
		if (m_data.size < m_data.capacity)
		{
			new (m_data.elemPtr(m_data.size)) T(obj);
			++m_data.size;
		}
		else
		{
			if (0 == m_increment)
			{
				resize(m_data.size * 2);
				new (m_data.elemPtr(m_data.size)) T(obj);
				++m_data.size;
			}
			else
			{
				resize(m_data.size + m_increment);
				new (m_data.elemPtr(m_data.size)) T(obj);
				++m_data.size;
			}
		}
	}

	// only used when the inner data type is char
	NNEvoid push_back(NNEchar *bytePtr, NNEuint num)
	{
		if (m_data.size + num < m_data.capacity)
		{
			memcpy(m_data.elemPtr(m_data.size), bytePtr, num);
			m_data.size += num;
		}
		else
		{
			if (0 == m_increment)
			{
				resize(m_data.size * 2);
				memcpy(m_data.elemPtr(m_data.size), bytePtr, num);
				m_data.size += num;
			}
			else
			{
				resize(m_data.size + m_increment + num);
				memcpy(m_data.elemPtr(m_data.size), bytePtr, num);
				m_data.size += num;
			}
		}
	}

	NNEvoid push_back(const T &obj, NNEuint num)
	{
		if (m_data.size + num < m_data.capacity)
		{
			for (NNEuint i = 0; i < num; ++i)
			{
				new (m_data.elemPtr(m_data.size++)) T(obj);
			}
		}
		else
		{
			if (0 == m_increment)
			{
				resize(m_data.size * 2 + num);
				for (NNEuint i = 0; i < num; ++i)
				{
					new (m_data.elemPtr(m_data.size++)) T(obj);
				}
			}
			else
			{
				resize(m_data.size + m_increment + num);
				for (NNEuint i = 0; i < num; ++i)
				{
					new (m_data.elemPtr(m_data.size++)) T(obj);
				}
			}
		}
	}

	// delete the last element
	NNEvoid pop_back()
	{
		debugAssertMsg(0 != m_data.size, "array empty before pop_back");
		static_cast<T *>(m_data.elemPtr(--m_data.size))->~T();
	}

	// destroy every object but don't wipe out memory
	NNEvoid destroy()
	{
		
	}

	// used when the inner data type is POD
	NNEvoid clear()
	{
		m_data.size = 0;
	}

	// fill the first num number of elemnets with val
	NNEvoid assign(NNEuint num, const T& val)
	{
		resize(num);
		//debugAssertMsg(num <= m_data.capacity, "access out of bound");
		for (NNEint i = 0; i < num; ++i)
		{
			push_back(val);
		}
		m_data.size = num;
	}

	// fill the first num number of elemnets using its default construtor
	NNEvoid assign(NNEuint num)
	{
		resize(num);
		for (NNEuint i = 0; i < num; ++i)
		{
			push_back(T());
		}
		m_data.size = num;
	}

	// fill the first num elements with zero
	NNEvoid assignZero(NNEuint num)
	{
		resize(num);
		//debugAssertMsg(num <= m_data.capacity, "access out of bound");
		memset(m_data.rawPtr, 0, num * m_data.elemSize);
		m_data.size = num;
	}

	// right value
	// member function "reserve" deosn't increase the size of element,
	// so you can't call this operator immediately after reserve function.
	const T &operator [] (NNEuint i) const
	{
		alwaysAssertMsg(i < m_data.size, "access out of bound");
		return *((T *)m_data.elemPtr(i));
	}

	// left value
	// member function "reserve" deosn't increase the size of element,
	// so you can't call this operator immediately after reserve function.
	T &operator [] (NNEuint i)
	{
		alwaysAssertMsg(i < m_data.size, "access out of bound");
		return *((T *)m_data.elemPtr(i));
	}

	NNE_FORCE_INLINE NNEuint size() const
	{
		return m_data.size;
	}

	size_t getBytes() const
	{
		return m_data.elemSize * m_data.size;
	}

	size_t getCapacityBytes() const
	{
		return m_data.elemSize * m_data.capacity;
	}

	T *data()
	{
		return static_cast<T *>(m_data.dataBegin);
	}

	NNEvoid setIncrement(NNEuint inc)
	{
		m_increment = inc;
	}

private:

	NNEvoid resize(NNEuint newsize)
	{
		if (newsize <= m_data.capacity)
		{
			return ;
		}

		NNEvoid *oldPtr = m_data.rawPtr;

		// (align - 1) is for starting pointer alignment
		m_data.rawPtr = ::malloc((align) + newsize * m_data.elemSize);

		m_data.capacity = newsize;
	
		NNEvoid *oldData = m_data.dataBegin;

		// align starting pointer
		m_data.dataBegin = (NNEvoid *)(((NNEuint)m_data.rawPtr + align) &~ (align - 1));

		if (nullptr != oldData)
		{
			// copy data from original memory to newly allocated memory
		
			// efficiency is not primary
			::memcpy(m_data.dataBegin, oldData, m_data.size * m_data.elemSize);

			//for (NNEuint i = 0; i < m_data.size; ++i)
			//{
			//	new (m_data.elemPtr(i)) T(*((T *)m_data.elemPtr(oldData, i)));
			//}

			// destroy old data
			// DON'T delete objects which might just be shallow copied
			// to the objects on new memory location
			// 
			/*for (NNEuint i = 0; i < m_size; ++i)
			{
				elemPtr(oldData, i)->~T();
			}*/
		}
	
		if (NULL != oldPtr)
		{
			// free original memory
			::free(oldPtr);
		}
	}

private:

	InnerData m_data;

	// when size reaches the capacity
	// increase the size by m_increment number
	// 0 means increase the size of array to twice as the orginal size
	// defaut is 5
	NNEuint m_increment;
};

} // namespace NNE

#endif // NNE_ARRAY_H