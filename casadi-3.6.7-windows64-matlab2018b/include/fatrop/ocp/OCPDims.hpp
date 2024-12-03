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
#ifndef FATROP_OCPDIMS_INCLUDED
#define FATROP_OCPDIMS_INCLUDED
#include "fatrop/auxiliary/FatropVector.hpp"
#include "fatrop/auxiliary/Common.hpp"
#include "fatrop/templates/NLPAlg.hpp"
#include <vector>
namespace fatrop
{
    /** \brief  this class contains the problem dimensions of a standard ocp*/
    struct OCPDims : public NLPDims
    {
    public:
        OCPDims(const fatrop_int K, const FatropVector<fatrop_int> &nu, const FatropVector<fatrop_int> &nx, const FatropVector<fatrop_int> &ng, const FatropVector<fatrop_int> &ng_ineq, const FatropVector<fatrop_int> &n_stage_params, const fatrop_int n_global_parameters) : NLPDims({sum(nx + nu), sum(ng + ng_ineq + nx) - nx.get(0), sum(ng_ineq)}),
                                                                                                                                                                                                                                                                                 K(K), nu(nu), nx(nx), ng(ng), ng_ineq(ng_ineq), n_stage_params(n_stage_params), n_global_params(n_global_parameters),
                                                                                                                                                                                                                                                                                 n_u_tot(sum(nu)),
                                                                                                                                                                                                                                                                                 n_x_tot(sum(nx)),
                                                                                                                                                                                                                                                                                 n_b_tot(n_x_tot - nx.get(0)),
                                                                                                                                                                                                                                                                                 n_g_tot(sum(ng)),
                                                                                                                                                                                                                                                                                 n_g_ineq_tot(sum(ng_ineq)), n_stage_params_tot(sum(n_stage_params)){};
        /// horizon length
        const fatrop_int K;
        /// input vector size
        const FatropVector<fatrop_int> nu;
        /// state vector size
        const FatropVector<fatrop_int> nx;
        // number of stagewise equality constraints
        const FatropVector<fatrop_int> ng;
        // number of stagewise inequality constraints
        const FatropVector<fatrop_int> ng_ineq;
        // number of stage parameters
        const FatropVector<fatrop_int> n_stage_params;
        // number of global parameters
        const fatrop_int n_global_params;
        const fatrop_int n_u_tot;
        const fatrop_int n_x_tot;
        const fatrop_int n_b_tot;
        const fatrop_int n_g_tot;
        const fatrop_int n_g_ineq_tot;
        const fatrop_int n_stage_params_tot;
    };
} // namespace fatrop
#endif // FATROP_OCPDIMS_INCLUDED