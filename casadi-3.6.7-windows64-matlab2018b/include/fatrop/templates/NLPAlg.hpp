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
#ifndef NLPINCLUDED
#define NLPINCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    struct NLPDims
    {
        fatrop_int nvars;
        fatrop_int neqs;
        fatrop_int nineqs;
    };
    class FatropNLP
    {
    public:
        virtual fatrop_int eval_lag_hess(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            const FatropVecBF &lam) = 0;
        virtual fatrop_int eval_constr_jac(
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars) = 0;
        virtual fatrop_int eval_constraint_viol(
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &constraint_violation) = 0;
        virtual fatrop_int eval_obj_grad(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &gradient_x,
            FatropVecBF &gradient_s) = 0;
        virtual fatrop_int eval_obj(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            double &res) = 0;
        virtual fatrop_int eval_dual_inf(
            double obj_scale,
            const FatropVecBF &lam,
            const FatropVecBF &grad_x,
            const FatropVecBF &grad_s,
            FatropVecBF &du_inf_x, FatropVecBF& du_inf_s) = 0;
        virtual fatrop_int solve_pd_sys(
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &gradb_total) = 0;
        virtual fatrop_int solve_soc_rhs(
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &cosntraint_violation) = 0;
        virtual NLPDims get_nlp_dims() const = 0;
        virtual fatrop_int compute_scalings(
            double &obj_scale,
            FatropVecBF &x_scales,
            FatropVecBF &lam_scales,
            const FatropVecBF &grad_curr_x, const FatropVecBF& grad_curr_s) = 0;
        virtual fatrop_int initialize_slacks(double mu0,
            FatropVecBF &s_curr) = 0;
        virtual fatrop_int initialize_dual(
            const FatropVecBF &grad_x,
            const FatropVecBF &grad_s,
            FatropVecBF &dlam,
            const FatropVecBF &zL,
            const FatropVecBF &zU) = 0;
        virtual fatrop_int get_bounds(
            FatropVecBF &lower,
            FatropVecBF &upper) const = 0;
        virtual fatrop_int get_initial_sol_guess(
            FatropVecBF &initial) const = 0;
        virtual void pre_solve(const FatropVecBF& x_init,const FatropVecBF& s_init){};
        // virtual fatrop_int GetDefaultParams(
        //     FatropOptions &params) const = 0;
        virtual fatrop_int Callback(FatropVecBF& primal_vars){return 0;};
        virtual void finalize(){};
        virtual void reset(){};
        virtual void update_mu(double mu){};
    };
} // namespace fatrop
#endif // NLPINCLUDED