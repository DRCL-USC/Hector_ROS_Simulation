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
#ifndef OCPTEMPLATEINCLUDED
#define OCPTEMPLATEINCLUDED
#include "fatrop/ocp/OCPKKT.hpp"
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "fatrop/auxiliary/FatropVector.hpp"
#include "OCPDims.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    /// @brief  Abstract class for OCP template
    /// @details  This class is used to define the interface for the OCP template.
    /// an implemtation contains all information that is needed by the fatrop algorithm to solve the OCP.
    /// the information includes the problem dimensions, the default parameters, default initial solution guess and evaluation code for stagewise function quantities.
    class OCPAbstract
    {
    public:
        /// @brief number of states for time step k
        /// @param k: time step
        virtual fatrop_int get_nx(const fatrop_int k) const = 0;
        /// @brief number of inputs for time step k
        /// @param k: time step
        virtual fatrop_int get_nu(const fatrop_int k) const = 0;
        /// @brief number of equality constraints for time step k
        /// @param k: time step
        virtual fatrop_int get_ng(const fatrop_int k) const = 0;
        /// @brief  number of stage parameters for time step k
        /// @param k: time step
        virtual fatrop_int get_n_stage_params(const fatrop_int k) const = 0;
        /// @brief  number of global parameters
        virtual fatrop_int get_n_global_params() const = 0;
        /// @brief default stage parameters for time step k
        /// @param stage_params: pointer to array of size n_stage_params_k
        /// @param k: time step
        virtual fatrop_int get_default_stage_params(double *stage_params, const fatrop_int k) const = 0;
        /// @brief default global parameters
        /// @param global_params: pointer to array of size n_global_params
        virtual fatrop_int get_default_global_params(double *global_params) const = 0;
        /// @brief number of inequality constraints for time step k
        /// @param k: time step
        virtual fatrop_int get_ng_ineq(const fatrop_int k) const = 0;
        /// @brief horizon length
        virtual fatrop_int get_horizon_length() const = 0;
        /// @brief  discretized dynamics
        /// it evaluates the vertical concatenation of A_k^T, B_k^T, and b_k^T from the linearized dynamics x_{k+1} = A_k x_k + B_k u_k + b_k. 
        /// The matrix is in column major format.
        /// @param states_kp1: pointer to nx_{k+1}-array states of time step k+1
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to (nu+nx+1 x nu+nx)-matrix 
        /// @param k: time step
        virtual fatrop_int eval_BAbt(
            const double *states_kp1,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) = 0;
        /// @brief  stagewise Lagrangian Hessian
        /// It evaluates is the vertical concatenation of (1) the Hessian of the Lagrangian to the concatenation of (u_k, x_k) (2) the first order derivative of the Lagrangian Hessian to the concatenation of (u_k, x_k). 
        /// The matrix is in column major format.
        /// @param objective_scale: scale factor for objective function (usually 1.0)
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param lam_dyn_k: pointer to array dual variables for dynamics of time step k
        /// @param lam_eq_k: pointer to array dual variables for equality constraints of time step k
        /// @param lam_eq_ineq_k: pointer to array dual variables for inequality constraints of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to (nu+nx+1 x nu+nx)-matrix. 
        /// @param k
        /// @return
        virtual fatrop_int eval_RSQrqt(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *lam_dyn_k,
            const double *lam_eq_k,
            const double *lam_eq_ineq_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) = 0;
        /// @brief stagewise equality constraints Jacobian. 
        /// It evaluates the vertical concatenation of (1) the Jacobian of the equality constraints to the concatenation of (u_k, x_k) (2) the equality constraints evaluated at u_k, x_k.
        /// The matrix is in column major format.
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to (nu+nx+1 x ng)-matrix.
        /// @param k: time step
        /// @return
        virtual fatrop_int eval_Ggt(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) = 0;
        /// @brief stagewise inequality constraints Jacobian. 
        /// It evaluates the vertical concatenation of (1) the Jacobian of the inequality constraints to the concatenation of (u_k, x_k) (2) the inequality constraints evaluated at u_k, x_k. 
        /// The matrix is in column major format.
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params_ko: pointer to array global parameters
        /// @param res: pointer to (nu+nx+1 x ng_ineq)-matrix, column major format
        /// @param k : time step
        /// @return
        virtual fatrop_int eval_Ggt_ineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) = 0;
        /// @brief the dynamics constraint violation (b_k = -x_{k+1} + f_k(u_k, x_k, p_k, p))
        /// @param states_kp1: pointer to array states of time step k+1
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to array nx_{k+1}-vector
        /// @param k: time step
        /// @return
        virtual fatrop_int eval_b(
            const double *states_kp1,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) = 0;
        /// @brief the equality constraint violation (g_k = g_k(u_k, x_k, p_k, p))
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to array ng-vector
        /// @param k: time step
        virtual fatrop_int eval_g(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) = 0;
        /// @brief the inequality constraint violation (g_ineq_k = g_ineq_k(u_k, x_k, p_k, p))
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to array ng_ineq-vector
        /// @param k: time step
        virtual fatrop_int eval_gineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) = 0;
        /// @brief gradient of the objective function (not the Lagrangian!) to the concatenation of (u_k, x_k)
        /// @param objective_scale: pointer to objective scale
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to (nu+nx)-array
        /// @param k: time step
        virtual fatrop_int eval_rq(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) = 0;
        /// @brief objective function value 
        /// @param objective_scale: pointer to array objective scale
        /// @param inputs_k: pointer to array inputs of time step k
        /// @param states_k: pointer to array states of time step k
        /// @param stage_params_k: pointer to array stage parameters of time step k
        /// @param global_params: pointer to array global parameters
        /// @param res: pointer to double
        /// @param k: time step
        virtual fatrop_int eval_L(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) = 0;
        /// @brief the bounds of the inequalites at stage k
        /// @param lower: pointer to ng_ineq-vector
        /// @param upper: pointer to ng_ineq-vector
        /// @param k: time step
        virtual fatrop_int get_bounds(double *lower, double *upper, const fatrop_int k) const = 0;
        /// @brief default initial guess for the states of stage k
        /// @param xk: pointer to states of time step k 
        /// @param k: time step
        virtual fatrop_int get_initial_xk(double *xk, const fatrop_int k) const = 0;
        /// @brief default initial guess for the inputs of stage k
        /// @param uk: pointer to inputs of time step k
        /// @param k: time step
        virtual fatrop_int get_initial_uk(double *uk, const fatrop_int k) const = 0;

        virtual fatrop_int full_eval_lag_hess(double objective_scale,
                                        const double* primal_data,
                                        const double* lam_data,
                                        const double* stageparams_p,
                                        const double* globalparams_p, MAT * RSQrqt_p) {
            return 0;
        }
        virtual fatrop_int full_eval_constr_jac(const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            MAT* BAbt_p, MAT* Ggt_p, MAT* Ggt_ineq_p) {
            return 0;
        }
        virtual fatrop_int full_eval_contr_viol(const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* cv_p) {
            return 0;
        }
        virtual fatrop_int full_eval_obj_grad(double objective_scale, const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* grad_p) {
            return 0;
        }
        virtual fatrop_int full_eval_obj(double objective_scale, const double* primal_data, const double* stageparams_p, const double* globalparams_p,
            double* res) {
            return 0;
        }
    };
};     // namespace fatrop
#endif // OCPTEMPLATEINCLUDED