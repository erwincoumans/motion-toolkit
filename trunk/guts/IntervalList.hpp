/*
 * GUTS - Generic Utilities 
 * Copyright (c) 2006, Gino van den Bergen, DTECTA 
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

#ifndef GUTS_INTERVALLIST_HPP
#define GUTS_INTERVALLIST_HPP

#include <vector>
#include <algorithm>

#include <consolid.h>

#include "Invariant.hpp"

namespace guts
{
    template <typename Scalar>
    class IntervalList
    {
    public:
        typedef Scalar Scalar_type;

        IntervalList()
        {
            GUARD_INVARIANT_AT_EXIT(this);
        }

        void addInterval(Scalar lb, Scalar ub);

        size_t getNumIntervals() const
        {
            return mList.size() / 2;
        }

        void getInterval(Scalar& lb, Scalar& ub, size_t index) const
        {
            ASSERT(index < getNumIntervals());

            lb = mList[2 * index];
            ub = mList[2 * index + 1];
        }
        
        INVARIANT
        {
            ASSERT(mList.size() % 2 == 0);
            ASSERT(std::is_sorted(mList.begin(), mList.end());
        }

    private:
        typedef std::vector<Scalar> EndpointList_t;

        EndpointList_t mList;
    };

    template <typename Scalar>
    void IntervalList<Scalar>::addInterval(Scalar lb, Scalar ub)
    {
        GUARD_INVARIANT(this);

        ASSERT(!(ub < lb));

        bool inside = false;
        EndpointList_t::iterator it = mList.begin(); 
        while (it != mList.end() && *it < lb)
        {
            inside = !inside;
            ++it;
        }

        if (!inside)
        {
            it = mList.insert(it, lb);
            ++it;
        }

        ASSERT(it == mList.end() || *it >= lb); 

        EndpointList_t::iterator first = it; 

        while (*it != mList.end() && *it < ub)
        {
            inside = !inside;
            ++it;
        }

        it = mList.erase(first, it);

        if (!inside)
        {
            it = mList.insert(it, ub);
        }
    }
}

#endif
