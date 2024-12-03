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
#ifndef NLPLSINCLUDED
#define NLPLSINCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "OCPKKT.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    class OCPLinearSolver
    {
    public:
        virtual fatrop_int solve_pd_sys(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &gradb_total) = 0;
        virtual fatrop_int compute_pd_sys_times_vec(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb_total) = 0;
        virtual fatrop_int solve_rhs(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb_total) = 0;
        virtual fatrop_int get_rhs(
            OCPKKTMemory *OCP,
            const FatropVecBF &gradb_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb) = 0;
    };

} // namespace fatrop
#endif //  NLPLSINCLUDED