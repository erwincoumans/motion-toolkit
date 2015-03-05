#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace guts
{

template <typename T>
struct Singleton
{
	static T& instance()
	{
		static T theInstance;
		return theInstance;
	}
};

};//namespace guts

#endif //SINGLETON_HPP

