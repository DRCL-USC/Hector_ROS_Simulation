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
#ifndef OCPINCLUDED
#define OCPINCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "OCPDims.hpp"
#include "fatrop/ocp/OCPKKT.hpp"
#include "fatrop/solver/FatropData.hpp"
#include <memory>
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    /** \brief interface class for OCP operations*/
    class OCP
    {
    public:
        virtual fatrop_int eval_lag_hess(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &lam) = 0;
        virtual fatrop_int eval_constr_jac(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars) = 0;
        virtual fatrop_int eval_contr_viol(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &constraint_violation) = 0;
        virtual fatrop_int eval_ineqs(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            FatropVecBF &constraint_violation) = 0;
        virtual fatrop_int eval_obj_grad(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            FatropVecBF &gradient_x 
            ) = 0;
        virtual fatrop_int eval_obj(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            double &res) = 0;
        virtual fatrop_int integrate_dynamics(
            OCPKKTMemory *OCP,
            const fatrop_int k,
            const FatropVecBF &uk,
            const FatropVecBF &xk,
            FatropVecBF &xkp1) = 0;
        virtual OCPDims get_ocp_dims() const = 0;
        virtual fatrop_int get_bounds(
            FatropVecBF &lower,
            FatropVecBF &upper) const = 0;
        virtual fatrop_int get_initial_sol_guess(
            FatropVecBF &initial) const = 0;
        virtual void reset() = 0;
        // virtual fatrop_int GetDefaultParams(
        //     FatropOptions &params) const = 0;
        virtual void set_parameters(const std::vector<double> &stage_params_in, const std::vector<double> &global_params_in) = 0;
        virtual void set_initial_sol_guess(const std::shared_ptr<FatropData> &fatropdata, std::vector<double> &initial_u, std::vector<double> &initial_x) = 0;
        virtual void get_solution(const std::shared_ptr<FatropData> &fatropdata, std::vector<double> &u, std::vector<double> &x) = 0;
    };
} // namespace fatrop
#endif // OCPINCLUDED