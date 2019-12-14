#pragma once
#include <stdint.h>

#include <cmath>

namespace TUtil {
	namespace Math {
		uint64_t LogFloor(uint64_t value);

		template<typename T>
		T PI() { return static_cast<T>(3.141592653589793238462643383279502884L); }


		//See https://www.desmos.com/calculator/ke8hwwsimp
		//For a visual representation of each function
		enum class InterpolationMethod
		{
			LINEAR, COSINE, EXPONENTIAL, SQRT, POLYNOMIAL
		};



		template<typename T>
		inline T Interpolate(T a, T b, T f, InterpolationMethod method = InterpolationMethod::LINEAR);

		//Returns a value between 0 and 1 that denotes where value falls in the range [min, max]
		//0 means value is min, 1 means value is max, 0.5 means value is the average of min and max, etc.
		template<typename T> 
		T Normalize(T min, T max, T value);

		//Maps a value in a particular range to a new range using interpolation
		template<typename T> 
		T Map(T sourceMin, T sourceMax, T destMin, T destMax, T value, InterpolationMethod method = InterpolationMethod::LINEAR);




		//==================== IMPLEMENTATIONS ====================
		template<typename T>
		static T Lerp(T a, T b, T f) { return a + f * (b - a); }

		template<typename T>
		T Interpolate(T a, T b, T f, InterpolationMethod method)
		{
			if		(method == InterpolationMethod::LINEAR)				   Lerp(a, b, f);
			else if (method == InterpolationMethod::COSINE)			return Lerp(a, b, static_cast<T>(0.5) * static_cast<T>(static_cast<T>(1.0) - cos(PI<T>() * f)) );
			else if (method == InterpolationMethod::EXPONENTIAL)	return Lerp(a, b, pow(static_cast<T>(2.0), f) - static_cast<T>(1.0));
			else if (method == InterpolationMethod::SQRT)			return Lerp(a, b, sqrt(f));
			else if (method == InterpolationMethod::POLYNOMIAL)		return Lerp(a, b, pow(f, static_cast<T>(2.0)));
			else													return a;
		}

		template<typename T>
		T Normalize(T min, T max, T value) {
			return (value - min) / (max - min);
		}

		template<typename T>
		T Map(T sourceMin, T sourceMax, T destMin, T destMax, T value, InterpolationMethod method)
		{
			T n = Normalize(sourceMin, sourceMax, value);
			return Interpolate(destMin, destMax, n, method);
		}

	}

}
