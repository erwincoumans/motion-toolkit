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

#ifndef MT_RANDOM_HPP
#define MT_RANDOM_HPP

#include <consolid/rand32.h>
#include <complex>

#include <moto/Scalar.hpp>
#include <moto/ScalarTraits.hpp>
#include <moto/Vector2.hpp>
#include <moto/Vector3.hpp>
#include <moto/Vector4.hpp>

namespace mt
{
    template <typename Scalar>
    struct Random
    {  
        static Scalar uniform();
        static Scalar uniform(Scalar a, Scalar b);
        static Scalar angle();  
		static Vector2<Scalar> uniformVector2();
        static Vector2<Scalar> uniformVector2(Scalar a, Scalar b);
        static Vector3<Scalar> uniformVector3();
        static Vector3<Scalar> uniformVector3(Scalar a, Scalar b);
        static Vector3<Scalar> direction();
        static Vector4<Scalar> rotation();
    };

    
    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::uniform()
    {
        return Scalar(rand32()) / Scalar(std::numeric_limits<unsigned long>::max());
    }
    
    template <>
    FORCEINLINE
    float Random<float>::uniform()
    {
        return frand32();
    }
    
    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::uniform(Scalar a, Scalar b)
    {
        return lerp(a, b, Random<Scalar>::uniform());
    }

    template <typename Scalar>
    FORCEINLINE
    Scalar Random<Scalar>::angle()
    {
        return Random<Scalar>::uniform(-ScalarTraits<Scalar>::pi(), ScalarTraits<Scalar>::pi());
    } 

	template <typename Scalar>
    Vector2<Scalar> Random<Scalar>::uniformVector2()
    {
        Scalar x = Random<Scalar>::uniform();
        Scalar y = Random<Scalar>::uniform();
        return Vector2<Scalar>(x, y);
    }

    template <typename Scalar>
    Vector2<Scalar> Random<Scalar>::uniformVector2(Scalar a, Scalar b)
    {
        Scalar x = Random<Scalar>::uniform(a, b);
        Scalar y = Random<Scalar>::uniform(a, b);
        return Vector2<Scalar>(x, y);
    }
    
        
    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::uniformVector3()
    {
        Scalar x = Random<Scalar>::uniform();
        Scalar y = Random<Scalar>::uniform();
        Scalar z = Random<Scalar>::uniform();
        return Vector3<Scalar>(x, y, z);
    }

    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::uniformVector3(Scalar a, Scalar b)
    {
        Scalar x = Random<Scalar>::uniform(a, b);
        Scalar y = Random<Scalar>::uniform(a, b);
        Scalar z = Random<Scalar>::uniform(a, b);
        return Vector3<Scalar>(x, y, z);
    }
    
    template <typename Scalar>
    Vector3<Scalar> Random<Scalar>::direction()
    {
        Scalar z = Random<Scalar>::uniform(Scalar(-1), Scalar(1));
        Scalar r = sqrt(Scalar(1) - square(z));
        Scalar t = Random<Scalar>::angle();
        std::complex<Scalar> e = std::polar(r, t);
        return Vector3<Scalar>(e.real(), e.imag(), z);
    }
    
    template <typename Scalar>
    Vector4<Scalar> Random<Scalar>::rotation()
    {
        // From: "Uniform Random Rotations", Ken Shoemake, Graphics Gems III, 
        //       pg. 124-132
        Scalar x0 = Random<Scalar>::uniform();
        Scalar r1 = sqrt(Scalar(1) - x0);
        Scalar r2 = sqrt(x0);
        Scalar t1 = Random<Scalar>::angle();
        Scalar t2 = Random<Scalar>::angle();
        std::complex<Scalar> c1 = std::polar(r1, t1);
        std::complex<Scalar> c2 = std::polar(r2, t2);
        return Vector4<Scalar>(c1.imag(), c1.real(), c2.imag(), c2.real());
    }
}

#endif
