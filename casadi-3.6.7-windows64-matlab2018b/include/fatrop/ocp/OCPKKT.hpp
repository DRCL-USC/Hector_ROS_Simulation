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
#ifndef FATROP_OCP_KKT_INCLUDED
#define FATROP_OCP_KKT_INCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "OCPDims.hpp"
#include "fatrop/auxiliary/VectorUtils.hpp"
#include "fatrop/auxiliary/FatropVector.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    /** \brief this class contains all information to represent the KKT system of an equality constrained OCP */
    class OCPKKTMemory
    {
    public:
        OCPKKTMemory(const OCPDims &dims);
        OCPKKTMemory(const OCPKKTMemory &cpy) = delete;
        fatrop_int K;
        FatropVector<fatrop_int> nu;
        FatropVector<fatrop_int> nx;
        FatropVector<fatrop_int> ng;
        FatropVector<fatrop_int> ng_ineq;
        /// small-scale Hessian
        FatropMemoryMatBF RSQrqt;
        /// small-scale Jacobian dynamics
        FatropMemoryMatBF BAbt;
        /// small-scale Jacobian stagewise eq constraints
        FatropMemoryMatBF Ggt;
        /// small-scale Jacobian stagewise ineq constraints
        FatropMemoryMatBF Ggt_ineq;
        class OCPAux
        {
        public:
            OCPAux(const OCPDims &dims);
            /// offset arrays are used for efficiency
            const FatropVector<fatrop_int> ux_offs;
            /// offset arrays are used for efficiency
            const FatropVector<fatrop_int> g_offs;
            const FatropVector<fatrop_int> dyn_offs;
            const FatropVector<fatrop_int> dyn_eq_offs;
            const FatropVector<fatrop_int> g_ineq_offs;
            const FatropVector<fatrop_int> ineq_offs;
            fatrop_int max_nu;
            fatrop_int max_nx;
            fatrop_int max_ng;
            fatrop_int max_ngineq;
            fatrop_int n_ineqs;
        };
        OCPAux aux;
    };

} // namespace fatrop
#endif // FATROP_OCP_KKT_INCLUDED