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

#ifndef MT_TRIGONOMETRIC_HPP
#define MT_TRIGONOMETRIC_HPP

#include <complex>

#include <moto/Metric.hpp>
#include <moto/ScalarTraits.hpp>
#include <moto/Vector4.hpp>
#include <moto/ScalarTraits.hpp>

namespace mt
{
    template <typename Scalar> Scalar radians(Scalar degs);
    template <typename Scalar> Scalar degrees(Scalar rads);
    
    template <typename Vector> typename Vector::ScalarType angle(const Vector& v1, const Vector& v2);
    
    template <typename Vector, typename Scalar> 
    Vector slerp(const Vector& u1, const Vector& u2, typename Vector::ScalarType t); 

    template <typename Vector> typename Vector::ScalarType angle3(const Vector& v1, const Vector& v2);
    
    template <typename Vector, typename Scalar> 
    Vector slerp3(const Vector& u1, const Vector& u2, typename Vector::ScalarType t); 


    template <typename Scalar> std::complex<Scalar> euler(Scalar theta);

    template <typename Scalar> Vector4<Scalar> fromAxisAngle(const Vector3<Scalar>& axis, Scalar theta);
    template <typename Scalar> void toAxisAngle(Vector3<Scalar>& axis, Scalar& theta, const Vector4<Scalar>& u);

    template <typename Scalar> Vector4<Scalar> exp(const Vector3<Scalar>& v);
    template <typename Scalar> Vector3<Scalar> logUnit(const Vector4<Scalar>& u);
	
	template <typename Scalar1, typename Scalar2> 
    Vector4<Scalar1> logLerpUnit(const Vector4<Scalar1>& u1, const Vector4<Scalar1>& u2, Scalar2 t);
      

    template <typename Scalar> Vector4<Scalar> exp(const Vector4<Scalar>& q); 
    template <typename Scalar> Vector4<Scalar> log(const Vector4<Scalar>& q); 
    
    template <typename Scalar1, typename Scalar2> 
    Vector4<Scalar1> logLerp(const Vector4<Scalar1>& q1, const Vector4<Scalar1>& q2, Scalar2 t);

    template <typename Scalar1, typename Scalar2> Vector4<Scalar1> integrate(const Vector4<Scalar1>& q0, const Vector3<Scalar1>& v, Scalar2 h);
    template <typename Scalar1, typename Scalar2> Vector3<Scalar1> velocity(const Vector4<Scalar1>& q0, const Vector4<Scalar1>& q1, Scalar2 h);

    template <typename Scalar>
    Vector3<Scalar> dihedral(const Vector3<Scalar>& u1, const Vector3<Scalar>& u2);


    template <typename Scalar>
    Scalar radians(Scalar degs)
    {
        static const Scalar RADS_PER_DEG = ScalarTraits<Scalar>::pi() / Scalar(180);
        return RADS_PER_DEG * degs;
    } 

    template <typename Scalar>
    Scalar degrees(Scalar rads)
    {
        static const Scalar DEGS_PER_RAD = Scalar(180) / ScalarTraits<Scalar>::pi();
        return DEGS_PER_RAD * rads;
    }
    
    template <typename Vector>
    typename Vector::ScalarType angle(const Vector& v1, const Vector& v2)
    {
        return acos(dot(v1, v2) * rsqrt(lengthSquared(v1) * lengthSquared(v2)));
    }
    
    template <typename Vector>
    Vector slerp(const Vector& u1, const Vector& u2, typename Vector::ScalarType t)
    {
        typedef typename Vector::ScalarType Scalar;
        Scalar costheta = dot(u1, u2);
        Scalar theta = acos(costheta);
        if (!iszero(theta))
        {
            Scalar r = rsqrt(Scalar(1) - square(costheta));
            Scalar s0 = sin((Scalar(1) - t) * theta) * r;
            Scalar s1 = sin(t * theta) * r;   
            return u1 * s0 + u2 * s1;
        }
        else
        {
            return u1;
        }
    }    
    
    template <typename Vector>
    typename Vector::ScalarType angle3(const Vector& v1, const Vector& v2)
    {
        return acos(dot3(v1, v2) * rsqrt(lengthSquared3(v1) * lengthSquared3(v2)));
    }
    
    template <typename Vector>
    Vector slerp3(const Vector& u1, const Vector& u2, typename Vector::ScalarType t)
    {
        typedef typename Vector::ScalarType Scalar;
        Scalar costheta = dot3(u1, u2);
        Scalar theta = acos(costheta);
        if (!iszero(theta))
        {
            Scalar r = rsqrt(Scalar(1) - square(costheta));
            Scalar s0 = sin((Scalar(1) - t) * theta) * r;
            Scalar s1 = sin(t * theta) * r;   
            return u1 * s0 + u2 * s1;
        }
        else
        {
            return u1;
        }
    } 


    template <typename Scalar>
    FORCEINLINE 
    std::complex<Scalar> euler(Scalar theta)
    {
        return std::complex<Scalar>(cos(theta), sin(theta)); 
    } 
    
#if HAVE_SINCOS

    FORCEINLINE 
    std::complex<float> euler(float angle)
    {
        float sine, cosine;
        sincosf(angle, &sine, &cosine);
        return std::complex<float>(cosine, sine);
    }

    FORCEINLINE 
    std::complex<double> euler(double angle)
    {
        double sine, cosine;
        sincos(angle, &sine, &cosine);
        return std::complex<double>(cosine, sine);
    }

    FORCEINLINE 
    std::complex<long double> euler(long double angle)
    {
        long double sine, cosine;
        sincosl(angle, &sine, &cosine);
        return std::complex<long double>(cosine, sine);
    }

#endif
    
    template <typename Scalar>
    FORCEINLINE
    Vector4<Scalar> fromAxisAngle(const Vector3<Scalar>& axis, Scalar theta)
    {
        std::complex<Scalar> c = euler(theta * Scalar(0.5));
        Scalar r = lengthSquared(axis);
        ASSERT(ispositive(r));
        Scalar s = c.imag() * rsqrt(r);
        return Vector4<Scalar>(axis * s, c.real());
    }

	template <typename Scalar>
    FORCEINLINE
    void toAxisAngle(Vector3<Scalar>& axis, Scalar& theta, const Vector4<Scalar>& u)
    {
        theta = acos(u.w) * Scalar(2);
        axis = normalize(xyz(u));
    }
    
    template <typename Scalar>
    Vector4<Scalar> exp(const Vector3<Scalar>& v)
    {
        Scalar theta = length(v);
        if (ispositive(theta))
        {
            std::complex<Scalar> c = euler(theta);
            Scalar s = c.imag() / theta;
            return Vector4<Scalar>(v * s, c.real());
        }
        else
        {
            return Identity();
        }
    }
   
    template <typename Scalar>
    Vector3<Scalar> logUnit(const Vector4<Scalar>& u)
    {
        Scalar theta = acos(u.w);
        ASSERT(isfinite(theta));
        return normalize(xyz(u)) * theta;
    }

	template <typename Scalar1, typename Scalar2>
    Vector4<Scalar1> logLerpUnit(const Vector4<Scalar1>& u1, const Vector4<Scalar1>& u2, Scalar2 t)
    {
        return exp(lerp(logUnit(u1), logUnit(u2), t));  
    } 
    

	template <typename Scalar>
    Vector4<Scalar> exp(const Vector4<Scalar>& q)
    {
        const Vector3<Scalar>& v = xyz(q); 
        Scalar rho = exp(q.w); 
        Scalar theta = length(v);
        if (ispositive(theta))
        {
            std::complex<Scalar> c = rho * euler(theta); // polar
            Scalar s = c.imag() / theta;
            return Vector4<Scalar>(v * s, c.real());
        }
        else
        {
            return Vector4<Scalar>(Scalar(), Scalar(), Scalar(), rho);
        }
    }

    template <typename Scalar>
    Vector4<Scalar> log(const Vector4<Scalar>& q)
    {
        Scalar r = length(q);
        Scalar s = ispositive(r) ? q.w / r : Scalar();
        Scalar theta = acos(s);
        ASSERT(isfinite(theta));
        return Vector4<Scalar>(normalize(xyz(q)) * theta, log(r));
    }
    
    template <typename Scalar1, typename Scalar2>
    Vector4<Scalar1> logLerp(const Vector4<Scalar1>& q1, const Vector4<Scalar1>& q2, Scalar2 t)
    {
        return exp(lerp(log(q1), log(q2), t));  
    }  

    template <typename Scalar1, typename Scalar2>
    FORCEINLINE 
    Vector4<Scalar1> integrate(const Vector4<Scalar1>& q0, const Vector3<Scalar1>& v, Scalar2 h)
    {
        return normalize(q0 + mul(v * (h * Scalar2(0.5)) , q0));
    }

    template <typename Scalar1, typename Scalar2>
    FORCEINLINE 
    Vector3<Scalar1> velocity(const Vector4<Scalar1>& q0, const Vector4<Scalar1>& q1, Scalar2 h)
    {
        return xyz(mul(q1 - q0, conjugate(q0))) * (Scalar2(2) / h);
    }

  
    template <typename Scalar>
    Vector3<Scalar> dihedral(const Vector3<Scalar>& u1, const Vector3<Scalar>& u2)
    {
        // This is the same as
        //return logUnit(Vector4<Scalar>(cross(u1, u2), dot(u1, u2))); 

        Scalar theta = acos(dot(u1, u2));
        ASSERT(isfinite(theta));
        return normalize(cross(u1, u2)) * theta; 
    }

    template <typename Scalar>
    FORCEINLINE
    Vector4<Scalar> euler(Scalar theta, Scalar phi, Scalar rho)
    {
        std::complex<Scalar> c1 = euler(theta);
        std::complex<Scalar> c2 = euler(phi);
        Vector3<Scalar> axis(c1.real() * c2.imag(), c1.imag() * c2.imag(), c2.real());
        return fromAxisAngle(axis, rho);  
    }

}

#endif

