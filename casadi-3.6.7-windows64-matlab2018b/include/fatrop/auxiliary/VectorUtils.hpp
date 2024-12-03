/*
 * Fatrop - A fast trajectory optimization solver
 *  Copyright (C) 2022 - 2024 Lander Vanroye, KU Leuven. All rights reserved.
 *
 * This file is part of Fatrop.
 *
 * Fatrop is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fatrop is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Fatrop.  If not, see <http://www.gnu.org/licenses/>. */
#ifndef FATROPAUXILIARYINCLUDED
#define FATROPAUXILIARYINCLUDED
#include <vector>
#include "FatropVector.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    /** \brief function to cumulative sum integer vector, first element is zero */
    template <typename T, typename E>
    FatropVector<T> offsets(const VecExpr<E, T> &a)
    {
        const fatrop_int size_a = a.size();
        FatropVector<T> res(size_a);
        res.at(0) = 0;
        for (fatrop_int i = 1; i < size_a; i++)
        {
            res.at(i) = a.get(i - 1) + res.get(i - 1);
        }
        return res;
    }

    /** \brief returns index of max el of VecExpr */
    template <typename T, typename E>
    fatrop_int maxel(const VecExpr<E, T> &a)
    {
        const fatrop_int size_a = a.size();
        fatrop_int res = 0;
        for (fatrop_int i = 0; i < size_a; i++)
        {
            fatrop_int ai = a.get(i);
            res = ai > res ? ai : res;
        }
        return res;
    }
};     // namespace fatrop
#endif // FATROPAUXINCLUDED