#ifndef CLASS_M_H__
#define CLASS_M_H__

namespace zeitgeist
{

#define CLASS(className)	Class_##className

#define DECLARE_CLASS(className)\
	class CLASS(className) : public zeitgeist::Class\
	{\
	public:\
		CLASS(className)() : zeitgeist::Class(#className)	{	DefineClass();	}\
		zeitgeist::Object* CreateInstance() const\
		{\
			zeitgeist::Object *instance = new className();\
			return instance;\
		}\
	private:\
		void DefineClass();\
	};

#define DECLARE_ABSTRACTCLASS(className)\
	class CLASS(className) : public zeitgeist::Class\
	{\
	public:\
		CLASS(className)() : zeitgeist::Class(#className)	{	DefineClass();	}\
	private:\
		void DefineClass();\
	};

#define FUNCTION(functionName)\
	static void functionName(zeitgeist::Object *obj, const zeitgeist::Class::TParameterList &in)

#define DEFINE_FUNCTION(functionName)\
	mFunctions[#functionName] = functionName;

#define DEFINE_BASECLASS(baseClass)\
	mBaseClasses.push_back(#baseClass);

//
// Export stuff
//
#define ZEITGEIST_EXPORT_BEGIN()\
	using namespace boost;\
	using namespace salt;\
	using namespace zeitgeist;\
	extern "C"{\
	SHARED_LIB_EXPORT void Zeitgeist_RegisterBundle(std::list <shared_ptr<Class> > &classes){

#define ZEITGEIST_EXPORT_EX(className, path)\
	classes.push_back(shared_ptr<Class>(new CLASS(className)));

#define ZEITGEIST_EXPORT(className) ZEITGEIST_EXPORT_EX(className, "")

#define ZEITGEIST_EXPORT_END()\
	}}

} // namespace zeitgeist

#endif // CLASS_M_H__
