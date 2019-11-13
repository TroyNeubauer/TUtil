#pragma once
#include <functional>


namespace TUtil{

	//Returns time in seconds. The magnitude is not important. All that matters is if calling this
	//function at different points in time results in a accurate increase
	extern float GetTime();

	template<typename T>
	class CachedFunction
	{
	public:
		CachedFunction(std::function<T()> func, float secondsPerUpdates) : m_Func(func), m_SecondsBetweenUpdates(secondsPerUpdates) {}

		T Get() {
			if (m_LastUpdate == -1.0f || (GetTime() - m_LastUpdate > m_SecondsBetweenUpdates)) {
				m_LastUpdate = GetTime();
				m_CachedValue = m_Func();
			}
			return m_CachedValue;
		}

	private:
		float m_SecondsBetweenUpdates;
		T m_CachedValue;

		float m_LastUpdate = -1.0f;
		std::function<T()> m_Func;
	};
}
