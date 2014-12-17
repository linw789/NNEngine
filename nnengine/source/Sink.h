#ifndef NNE_SINK_H
#define NNE_SINK_H

#include "PlatformSpecs.h"
#include "DebugUtilities.h"
#include "Array.h"
#include "Preprocessor.h"

namespace NNE{
namespace EVENTS{
/*
	Delegate serves as a type-safe wrapper for function pointer
	both free function pointer and member function pointer.
	
	reference:
	working example of delegate implementation
	http://molecularmusings.wordpress.com/2011/09/19/generic-type-safe-delegates-and-events-in-c/
	delegate implementated using template with non-type argument
	http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
	explain member function pointer in great detail
	http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible-
	automatic disconnection, simplified syntax
	http://hoyvinglavin.com/2012/08/06/jl_signal/

	template<typename T>
	class delegate{};

	template<typename T0, typename T1>
	class delegate{};

	The above template classes are illegal. In order to make template class name consistent,
	we use partial template technique. 
	reference:http://stackoverflow.com/questions/4642079/function-signature-like-expressions-as-c-template-arguments
*/

/*
	The class Sink serves as a collection of event handlers to a certain event.
	Each Event class holds a static event handler sink, therefore events of the 
	same Event class will have the same handler sink respond to.
	The implementation of delegate is embedded in the HandlerSink class.
*/
// primary template
template<typename T>
class Sink{};

#define NNE_EVENTS_DEFINE_SINK(NUM) \
/* R stands for return type */ \
template<typename RETURN_TYPE NNE_PP_COMMA_IF(NUM) NNE_PP_SEQUENCE(NUM, typename P)> \
class Sink<RETURN_TYPE (NNE_PP_SEQUENCE(NUM, P))> \
{ \
	typedef NNEvoid* ObjPtrType;  \
	typedef RETURN_TYPE (*FuncPtrType)(ObjPtrType NNE_PP_COMMA_IF(NUM) NNE_PP_SEQUENCE(NUM, P)); \
	\
	/* this structure is just a wapper storing function pointer and*/ \
	/* instance pointer if function pointer points to member function*/ \
	struct Delegate \
	{ \
		Delegate() \
			:objectPtr(nullptr), funcPtr(nullptr) \
		{} \
		\
		Delegate(const Delegate &d) \
			:objectPtr(d.objectPtr), funcPtr(d.funcPtr) \
		{} \
		\
		Delegate &operator = (const Delegate &d) \
		{ \
			objectPtr = d.objectPtr; \
			funcPtr = d.funcPtr; \
		} \
		\
		ObjPtrType objectPtr; \
		FuncPtrType funcPtr; \
	}; \
	\
private: \
	\
	template<RETURN_TYPE (*freeFunc)(NNE_PP_SEQUENCE(NUM, P))> \
	static NNE_FORCE_INLINE RETURN_TYPE freeFuncStub(ObjPtrType objptr NNE_PP_COMMA_IF(NUM) NNE_PP_SEQUENCE_2SYMBOL(NUM, P, ARG)) \
	{ \
		return (freeFunc)(NNE_PP_SEQUENCE(NUM, ARG)); \
	} \
	\
	template<class C, RETURN_TYPE (C::*MemberFunc)(NNE_PP_SEQUENCE(NUM, P))> \
	static NNE_FORCE_INLINE RETURN_TYPE memberFuncStub(ObjPtrType objptr NNE_PP_COMMA_IF(NUM) NNE_PP_SEQUENCE_2SYMBOL(NUM, P, ARG)) \
	{ \
		return (static_cast<C *>(objptr)->*MemberFunc)(NNE_PP_SEQUENCE(NUM, ARG)); \
	} \
	\
public: \
	\
	Sink() \
	{} \
	\
	Sink(NNEuint n) \
	{ \
		m_handlerArray.reserve(n); \
	} \
	\
	/* bind a free function to a delegate as an event handler*/ \
	template<RETURN_TYPE (*freeFunc)(NNE_PP_SEQUENCE(NUM, P))> \
	void addEventHandler() \
	{ \
		Delegate tmp; \
		tmp.objectPtr = nullptr; \
		tmp.funcPtr = &freeFuncStub<freeFunc>; \
		\
		m_handlerArray.push_back(tmp); \
	} \
	\
	/* bind a member function to a delegate as an event handler*/ \
	template<class C, RETURN_TYPE (C::*memberFunc)(NNE_PP_SEQUENCE(NUM, P))> \
	void addEventHandler(C *ptr) \
	{ \
		Delegate tmp; \
		tmp.objectPtr = ptr; \
		tmp.funcPtr = &memberFuncStub<C, memberFunc>; \
		\
		m_handlerArray.push_back(tmp); \
	} \
    \
	NNEvoid invoke(NNE_PP_SEQUENCE_2SYMBOL(NUM, P, ARG)) \
	{ \
		for (Array<Delegate>::iterator it = m_handlerArray.begin(); it != m_handlerArray.end(); ++it) \
		{ \
			debugAssertMsg((*it).funcPtr != nullptr, "function equals nullptr"); \
			((*it).funcPtr)((*it).objectPtr NNE_PP_COMMA_IF(NUM) NNE_PP_SEQUENCE(NUM, ARG)); \
		} \
	} \
	\
private: \
	\
	/* array of delegates which serve as event handlers*/ \
	Array<Delegate> m_handlerArray; \
}; 

// I could set VC++ preprocessor build option to output preprocessed code
// to a file, but the code generated doesn't preserve the format(no line break).
// define 7 possible Sink templates
NNE_EVENTS_DEFINE_SINK(0);
NNE_EVENTS_DEFINE_SINK(1);
NNE_EVENTS_DEFINE_SINK(2);
NNE_EVENTS_DEFINE_SINK(3);
NNE_EVENTS_DEFINE_SINK(4);
NNE_EVENTS_DEFINE_SINK(5);
NNE_EVENTS_DEFINE_SINK(6);

} // namespace EVENTS
} // manespace NNE

#endif // NNE_SINK_H