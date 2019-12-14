#pragma once

#include <vector>
#include <initializer_list>
#include <algorithm>

#include "Math.h"

namespace TUtil {

	//Default storage for curve points
	template<typename T>
	struct CurvePoint
	{		
		T x, y;
	};

	template<typename T, typename CurvePointImpl>
	struct SortCurvePoint
	{
		inline bool operator() (const CurvePointImpl& a, const CurvePointImpl& b)
		{
			return a.x < b.x;
		}
	};

	//Represents a set of points and uses interpolation to model a curve
	template<typename T, typename CurvePointImpl = CurvePoint<T>>//The storage impl can be overridden to use any class with public x and y members (glm::vec2, b2Vec2, etc.)
	class Curve
	{
	public:
		Curve(const Curve& other) = default;
		
		Curve(std::initializer_list<CurvePointImpl> points) : m_Points(points)
		{
			HZ_CORE_ASSERT(m_Points.size() > 0, "initalizerList cannot be empty!");
			std::sort(m_Points.begin(), m_Points.end(), SortCurvePoint<T, CurvePointImpl>());

		}

		T Sample(T point) const
		{
			auto* last = &m_Points[0];
			//We are sampling to the left of defined values. Lock to the value of the first one
			if (point <= last->x) return last->y;
			for (size_t i = 1; i < m_Points.size(); i++)
			{
				if (point < m_Points[i].x)//We found a match!
				{
					return Math::Map(last->x, m_Points[i].x, last->y, m_Points[i].y, point, m_Method);
				}
				last = &m_Points[i];
			}
			//The requested value falls to the right of defined values. Clamp to the last one
			return last->y;
		}

		Math::InterpolationMethod GetInterpMethod() const { return m_Method; }
		void SetInterpMethod(Math::InterpolationMethod method) { m_Method = method; }

	private:
		std::vector<CurvePointImpl> m_Points;
		Math::InterpolationMethod m_Method = Math::InterpolationMethod::LINEAR;
	};

	template<typename CurvePointImpl = CurvePoint<float>>
	using FloatCurve = Curve<float, CurvePointImpl>;

	template<typename CurvePointImpl = CurvePoint<double>>
	using DoubleCurve = Curve<double, CurvePointImpl>;

}
