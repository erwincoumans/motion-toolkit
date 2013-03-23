/*
 * MoTo - Motion Toolkit 
 * Copyright (c) 2006-2010 Gino van den Bergen, DTECTA 
 * http://www.dtecta.com
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Gino van den Bergen makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef MT_SCALAR_HPP
#define MT_SCALAR_HPP

#include <consolid/consolid.h>

#include <cmath>
#include <cstring>

#include <guts/StaticAssert.hpp>

#undef min
#undef max

#define CONSCIENCE_NAGGING 0
#define APPROX_RECIPROCAL_SQRT 0

namespace mt
{  
    template <typename Scalar> Scalar acos(Scalar x);
    template <typename To, typename From> To bitCast(From from);
    
    template <typename Scalar> bool isnan(Scalar a); 
    template <typename Scalar> bool isinf(Scalar a); 
    template <typename Scalar> bool isfinite(Scalar a);  
    template <typename Scalar> bool ispositive(Scalar a); 
    template <typename Scalar> bool isnegative(Scalar a);   
    template <typename Scalar> bool iszero(Scalar a);  
    template <typename Scalar> bool islessnn(Scalar a, Scalar b);
   
    template <typename Scalar> bool operator!=(Scalar a, Scalar b); 
    template <typename Scalar> bool operator>(Scalar a, Scalar b);  
    template <typename Scalar> bool operator<=(Scalar a, Scalar b); 
    template <typename Scalar> bool operator>=(Scalar a, Scalar b); 
 
    template <typename Scalar> Scalar min(Scalar a, Scalar b); 
    template <typename Scalar> Scalar max(Scalar a, Scalar b);

    template <typename Scalar> Scalar& minimize(Scalar& a, Scalar b); 
    template <typename Scalar> Scalar& maximize(Scalar& a, Scalar b); 

    template <typename Scalar> Scalar clamp(Scalar x, Scalar a, Scalar b);
    template <typename Scalar> Scalar saturate(Scalar x);

    template <typename Element, typename Scalar> 
    Element lerp(const Element& a, const Element& b, Scalar t);   

    template <typename Scalar> Scalar sign(Scalar x);
    template <typename Scalar> Scalar rsqrt(Scalar a);

    template <typename Scalar> Scalar square(Scalar x);
    template <typename Scalar> Scalar cube(Scalar x); 

    template <typename Scalar> Scalar smooth(Scalar x);
   
    // All other functions are simply dumped into our namespace.

    using std::abs;
    using std::asin;
    using std::atan;
    using std::atan2; 
    using std::ceil;
    using std::cos;
    using std::cosh;
    using std::exp;
    using std::floor;    
    using std::fmod;
    using std::frexp;
    using std::ldexp;
    using std::log;
    using std::log10;
    using std::modf;
    using std::pow;
    using std::sin;
    using std::sinh;
    using std::sqrt;   
    using std::tan;
    using std::tanh;
    

    template <typename Scalar>
    FORCEINLINE
    Scalar acos(Scalar x)
    {
        return std::acos(clamp(x, Scalar(-1), Scalar(1)));
    }

    template <typename To, typename From> 
    FORCEINLINE
    To bitCast(From from)
    {
        STATIC_ASSERT(sizeof(To) == sizeof(From));

#if CONSCIENCE_NAGGING
        To result;
        std::memcpy(&result, &from, sizeof(To));
        return result;     
#else 
        union { To to; From from; } pun;
        pun.from = from;
        return pun.to;  
#endif
    }
 
    template <typename Scalar>
    FORCEINLINE
    bool isnan(Scalar a) 
    {
        return !(a == a);
    }

    template <typename Scalar>
    FORCEINLINE
    bool isinf(Scalar a) 
    {
        return !isnan(a) && isnan(a - a);
    }

    template <typename Scalar>
    FORCEINLINE
    bool isfinite(Scalar a) 
    {
        return !isnan(a) && !isnan(a - a);
    }

    template <typename Scalar>
    FORCEINLINE
    bool ispositive(Scalar a) 
    {
        return Scalar() < a;
    }

    template <typename Scalar>
    FORCEINLINE
    bool isnegative(Scalar a) 
    {
        return a < Scalar();
    }

    template <typename Scalar>
    FORCEINLINE
    bool iszero(Scalar a) 
    {
        return a == Scalar();
    }
  
    template <typename Scalar>
    FORCEINLINE
    bool isequalnn(Scalar a, Scalar b) 
    {
#if 0
        // Enable this check to see islessnn gets any negative values. 
        ASSERT(ispositive(Scalar(1) / a) && ispositive(Scalar(1) / b));
#endif
        return a == b;
    }

    template <typename Scalar>
    FORCEINLINE
    bool islessnn(Scalar a, Scalar b) 
    {
#if 0
        // Enable this check to see islessnn gets any negative values. 
        ASSERT(ispositive(Scalar(1) / a) && ispositive(Scalar(1) / b));
#endif
        return a < b;
    } 


  
    template <typename Scalar> 
    FORCEINLINE 
    bool operator!=(Scalar a, Scalar b)
    {
        return !(a == b);
    }

    template <typename Scalar> 
    FORCEINLINE 
    bool operator>(Scalar a, Scalar b)
    {
        return b < a;
    }

    template <typename Scalar> 
    FORCEINLINE 
    bool operator<=(Scalar a, Scalar b)
    {
        return !(b < a);
    }

    template <typename Scalar> 
    FORCEINLINE 
    bool operator>=(Scalar a, Scalar b)
    {
        return !(a < b);
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar max(Scalar a, Scalar b) 
    {
        return a < b ? b : a;
    }
        
    template <typename Scalar>
    FORCEINLINE
    Scalar min(Scalar a, Scalar b) 
    {
        return b < a ? b : a;
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar& maximize(Scalar& a, Scalar b) // b may be NaN
    {
        if (a < b)
        {
            a = b;
        }
        return a;
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar& minimize(Scalar& a, Scalar b) // b may be NaN
    {
        if (b < a)
        {
            a = b;
        }
        return a;
    } 
    
    template <typename Scalar>
    FORCEINLINE
    Scalar clamp(Scalar x, Scalar a, Scalar b)
    {
        ASSERT(!(b < a));
        return x < a ? a : (b < x ? b : x);
    }
    
    template <typename Scalar>
    FORCEINLINE
    Scalar saturate(Scalar x)
    {
        return clamp(x, Scalar(), Scalar(1));
    }  

    template <typename Scalar>
    FORCEINLINE
    Scalar sign(Scalar x)
    {
        return ispositive(x) ? Scalar(1) : iszero(x) ? Scalar() : Scalar(-1);
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar rsqrt(Scalar a) 
    {
        ASSERT(ispositive(a));
        return Scalar(1) / sqrt(a);
    }
        

    template <typename Element, typename Scalar>
    FORCEINLINE
    Element lerp(const Element& a, const Element& b, Scalar t)
    {
        return a * (Scalar(1) - t) + b * t;
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar square(Scalar x)
    {
        return x * x;
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar cube(Scalar x)
    {
        return x * x * x;
    } 

    template <typename Scalar>
    FORCEINLINE
    Scalar smoothstep(Scalar a, Scalar b, Scalar x)
    {  
        ASSERT(!(b < a));
        x = saturate((x - a) / (b - a));
        return (Scalar(3) - Scalar(2) * x) * x * x;
    } 

#if USE_IEEE_754
       
    FORCEINLINE
    bool isnan(float a) 
    {
        return (bitCast<int32_t>(a) & 0x7fffffff) > 0x7f800000;
    }

    FORCEINLINE
    bool isinf(float a) 
    {
        return (bitCast<int32_t>(a) & 0x7fffffff) == 0x7f800000;
    }

    FORCEINLINE
    bool isfinite(float a) 
    {
        return (bitCast<int32_t>(a) & 0x7fffffff) < 0x7f800000;
    }

    FORCEINLINE
    bool ispositive(float a) 
    {
        return bitCast<int32_t>(a) > 0;
    }

    FORCEINLINE
    bool isnegative(float a) 
    {
        return (bitCast<int32_t>(a) ^ int32_t(0x80000000)) > 0;
    }

    FORCEINLINE
    bool iszero(float a) 
    {
        return (bitCast<int32_t>(a) & 0x7fffffff) == 0;
    }
 
#if 1
    // Quick and dirty float comparison using a cast to integers.
    // NB: This function is limited to non-negative numbers.
    // NB2: Negative zero is regarded negative in this context 
   
    FORCEINLINE
    bool isequalnn(float a, float b) 
    { 
        ASSERT((bitCast<int32_t>(a) & int32_t(0x80000000)) == 0 && (bitCast<int32_t>(b) & int32_t(0x80000000)) == 0);
        return bitCast<int32_t>(a) == bitCast<int32_t>(b);
    } 

    FORCEINLINE
    bool islessnn(float a, float b) 
    {  
		ASSERT((bitCast<int32_t>(a) & int32_t(0x80000000)) == 0 && (bitCast<int32_t>(b) & int32_t(0x80000000)) == 0);
        return bitCast<int32_t>(a) < bitCast<int32_t>(b);
    } 
#endif

    FORCEINLINE
    float sign(float x) 
    {
        int32_t i = bitCast<int32_t>(x) ;
        int32_t nosign = i & 0x7fffffff;
        return bitCast<float>((nosign != 0) * ((i & int32_t(0x80000000)) | 0x3f800000));  
    }

#if APPROX_RECIPROCAL_SQRT

    FORCEINLINE
    float rsqrt(float x) 
    {
        ASSERT(ispositive(x)); 
        float xhalf = x * 0.5f;
        int32_t i = bitCast<int32_t>(x);
        i = 0x5f3759df - (i >> 1);
        x = bitCast<float>(i);
        x = x * (1.5f - x * x * xhalf);
        return x;
    }

#endif

#endif

}

#endif
