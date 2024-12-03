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

// solver data
#ifndef FATROPDATAINCLUDED
#define FATROPDATAINCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "fatrop/templates/NLPAlg.hpp"
#include "fatrop/auxiliary/Common.hpp"
#include "FatropOptions.hpp"
#include "fatrop/solver/FatropPrinter.hpp"
#include <cmath>
#include <memory>
namespace fatrop
{
#define CACHEMACRO(instance, val) instance.evaluated ? instance.value : instance.SetValue(val)
    struct FatropData
    {
        FatropData(const NLPDims &nlpdims, const std::shared_ptr<FatropOptions> &params, const std::shared_ptr<FatropPrinter> &printer);
        void initialize();
        fatrop_int reset();
        fatrop_int reset_caches();
        double e_mu_curr(double mu);
        fatrop_int eval_dual_inf_slack_eqs();
        double eval_compl_slack(double mu);
        double eval_barrier_func(double mu, VEC *s_p);
        double eval_barrier_func_curr(double mu);
        double eval_barrier_func_trial(double mu);
        double eval_barrier_func_backup(double mu);
        double eval_barrier_fo_decr(double mu, VEC *s_p, VEC *delta_s_p);
        double eval_barrier_fo_decr_curr(double mu);
        double eval_barrier_fo_decr_backup(double mu);
        fatrop_int bound_slacks();
        fatrop_int bound_z();
        fatrop_int warmstart_dual();
        fatrop_int modify_dual_bounds(double mu);
        fatrop_int accept_dual_initializiaton();
        fatrop_int update_trial_step(double alpha_primal, double alpha_dual);
        fatrop_int accept_trial_step();
        fatrop_int backup_curr();
        fatrop_int backup_delta();
        fatrop_int restore_backup();
        double constr_viol_max_curr();
        double constr_viol_max_next();
        double constr_viol_sum_curr();
        double constr_viol_sum_backup();
        double constr_viol_sum_next();
        double dual_sum_curr();
        double dual_max_curr();
        double dual_mean_curr();
        double z_sum_curr();
        fatrop_int number_of_bounds();
        double delta_dual_max();
        double dual_inf_max_curr();
        double fo_decr_obj_curr();
        double fo_decr_obj_backup();
        void maximum_step_size(double &alpha_max_pr, double &alpha_max_du, double tau);
        void set_bounds(const std::vector<double> &lowerin, const std::vector<double> &upperin);
        void relax_bounds();
        void relax_bounds_var(double mu);
        void evaluate_barrier_quantities(double mu);
        void compute_delta_z();
        void compute_primal_dual_residu();
        void init_bounds();
        bool small_step_size();


        const NLPDims nlpdims;
        double obj_scale = 1.0;
        fatrop_int n_eqs;
        fatrop_int n_ineqs;
        fatrop_int n_ineqs_r = 0;
        FatropMemoryVecBF memvars;
        FatropMemoryVecBF memeqs;
        FatropMemoryVecBF memineqs;
        FatropVecBF x_curr;
        FatropVecBF x_next;
        FatropVecBF x_backup;
        FatropVecBF x_initial;
        FatropVecBF delta_x;
        FatropVecBF delta_x_backup;
        FatropVecBF delta_x_backup_ls;
        FatropVecBF x_scales;
        FatropVecBF lam_curr;
        FatropVecBF lam_next;
        FatropVecBF lam_backup;
        FatropVecBF lam_calc;
        FatropVecBF lam_calc_backup;
        FatropVecBF lam_calc_backup_ls;
        FatropVecBF lam_scales;
        FatropVecBF lam_init;
        FatropVecBF g_curr;
        FatropVecBF g_next;
        FatropVecBF g_backup;
        FatropVecBF g_soc;
        FatropVecBF grad_curr_x;
        FatropVecBF grad_next_x;
        FatropVecBF grad_backup_x;
        FatropVecBF du_inf_curr;
        FatropVecBF du_inf_curr_s;
        // vectors neccessary for inequality constraints
        FatropVecBF s_curr;
        FatropVecBF s_next;
        FatropVecBF s_backup;
        FatropVecBF delta_s;
        FatropVecBF delta_s_backup;
        FatropVecBF delta_s_backup_ls;
        FatropVecBF zL_curr;
        FatropVecBF zL_next;
        FatropVecBF zL_backup;
        FatropVecBF zL_init;
        FatropVecBF zU_curr;
        FatropVecBF zU_next;
        FatropVecBF zU_backup;
        FatropVecBF zU_init;
        FatropVecBF delta_zL;
        FatropVecBF delta_zU;
        FatropVecBF s_lower_orig;
        FatropVecBF s_upper_orig;
        FatropVecBF s_lower;
        FatropVecBF s_upper;
        FatropVecBF sigma_L;
        FatropVecBF sigma_U;
        FatropVecBF sigma_total;
        FatropVecBF gradb_L;
        FatropVecBF gradb_U;
        FatropVecBF gradb_plus;
        FatropVecBF gradb_total;
        FatropVecBF grad_curr_s;
        FatropVecBF grad_next_s;
        FatropVecBF grad_backup_s;
        FatropVecBF du_inf_curr_s_wo_z;
        // vector<bool> lower_bounded_v;
        // vector<bool> upper_bounded_v;
        struct EvalCache
        {
            struct Instance
            {
                bool evaluated = false;
                double value = 0.0;
                double SetValue(const double value_)
                {
                    value = value_;
                    evaluated = true;
                    return value;
                }
            };
            Instance cv_linf;
            Instance cv_l1;
            Instance lam_linf;
            Instance lam_l1;
            Instance du_inf_linf;
        };
        EvalCache cache_curr;
        EvalCache cache_next;
        double obj_curr = 0.0;
        double obj_backup = 0.0;
        double theta_min = 1e-4;
        const std::shared_ptr<FatropOptions> params;
        const std::shared_ptr<FatropPrinter> printer_;
        // algorithm parameters
        double smax;
        double kappa1;
        double kappa2;
        double kappa_d;
        double kappa_sigma;
        double bound_relax_factor;
        double constr_viol_tol;
        double warm_start_mult_bound_push;
    };
}
#endif // FATROPDATAINCLUDED