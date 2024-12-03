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

/**
 *  @file LinearAlgebra.hpp
 *  This file contains some interface classes for use in linear algebra.
 *
 */
#ifndef FATROP_LA_INCLUDED
#define FATROP_LA_INCLUDED
#include <iostream>
#include "fatrop/auxiliary/Common.hpp"

namespace fatrop
{
    struct MatrixInd
    {
        fatrop_int ai;
        fatrop_int aj;
    };
    /** \brief Interface class for matrix representations */
    class FatropMat
    {
    public:
        /** \brief Copy of matrix element */
        virtual double get_el(const fatrop_int ai, const fatrop_int aj) const = 0;
        /** \brief Number of rows */
        virtual fatrop_int nrows() const = 0;
        /** \brief Number of cols */
        virtual fatrop_int ncols() const = 0;
        void print();
    };
    // special matrices
    /** \brief Identity matrix */
    class eye : public FatropMat
    {
    public:
        /** \brief Constructor */
        eye(fatrop_int dim) : dim_(dim){};
        /** \brief Copy of matrix element */
        inline double get_el(const fatrop_int ai, const fatrop_int aj) const
        {
            if (ai == aj)
            {
                return 1.0;
            }
            else
            {
                return 0.0;
            }
        };
        /** \brief Number of rows */
        fatrop_int nrows() const { return dim_; };
        /** \brief Number of cols */
        fatrop_int ncols() const { return dim_; };

    private:
        fatrop_int dim_;
    };
    /** \brief Interface class for matrix representations */
    class FatropVec
    {
    public:
        /** \brief Copy of matrix element */
        virtual double get_el(const fatrop_int ai) const = 0;
        /** \brief Number of elements */
        virtual fatrop_int nels() const = 0;
        void print() const;
    };
} // namespace fatrop

#endif // FATROP_LA_INCLUDED