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

#ifndef OCPCINTERFACEINCLUDED
#define OCPCINTERFACEINCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Symbol visibility in DLLs */
#ifndef FATROP_SYMBOL_EXPORT
#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#if defined(STATIC_LINKED)
#define FATROP_SYMBOL_EXPORT
#else
#define FATROP_SYMBOL_EXPORT __declspec(dllexport)
#endif
#elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#define FATROP_SYMBOL_EXPORT __attribute__((visibility("default")))
#else
#define FATROP_SYMBOL_EXPORT
#endif
#endif

#include <blasfeo.h>

#define fatrop_int int

#ifndef FATROP_OCP_SOLVER_IMPLEMENTATION
    // Opaque types
    typedef struct FatropOcpCSolver FatropOcpCSolver;
#endif

struct FatropOcpCDims
{
    const fatrop_int *ux_offs;
    const fatrop_int *g_offs;
    const fatrop_int *dyn_offs;
    const fatrop_int *dyn_eq_offs;
    const fatrop_int *g_ineq_offs;
    const fatrop_int *ineq_offs;
    fatrop_int max_nu;
    fatrop_int max_nx;
    fatrop_int max_ng;
    fatrop_int max_ngineq;
    fatrop_int n_ineqs;
    fatrop_int K;
    const fatrop_int *nu;
    const fatrop_int *nx;
    const fatrop_int *ng;
    const fatrop_int *ng_ineq;
};

struct FatropOcpCStats
{
    double compute_sd_time;
    double duinf_time;
    double eval_hess_time;
    double eval_jac_time;
    double eval_cv_time;
    double eval_grad_time;
    double eval_obj_time;
    double initialization_time;
    double time_total;
    int eval_hess_count;
    int eval_jac_count;
    int eval_cv_count;
    int eval_grad_count;
    int eval_obj_count;
    int iterations_count;
    int return_flag;
};

// Function pointer types
typedef fatrop_int (*FatropOcpCGetDim_k)(fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCGetDim)(void* user_data);
typedef fatrop_int (*FatropOcpCGetDouble_k)(double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCGetDouble)(double*, void* user_data);

typedef fatrop_int (*FatropOcpCEval_BAbt_k)(const double*, const double*, const double*, const double*, const double*, struct blasfeo_dmat*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_RSQrqt_k)(const double*, const double*, const double*, const double*, const double*, const double*, const double*, const double*, struct blasfeo_dmat*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_Ggt_k)(const double*, const double*, const double*, const double*, struct blasfeo_dmat*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_Ggt_ineq_k)(const double*, const double*, const double*, const double*, struct blasfeo_dmat*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_b_k)(const double*, const double*, const double*, const double*, const double*, double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_g_k)(const double*, const double*, const double*, const double*, double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_gineq_k)(const double*, const double*, const double*, const double*, double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_rq_k)(const double*, const double*, const double*, const double*, const double*, double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCEval_L_k)(const double*, const double*, const double*, const double*, const double*, double*, fatrop_int, void* user_data);
typedef fatrop_int (*FatropOcpCGetBounds_k)(double*, double*, fatrop_int, void* user_data);

typedef fatrop_int (*FatropOcpCFullEvalLagHess)(double, const double*, const double*, const double*, const double*, struct blasfeo_dmat *, const struct FatropOcpCDims*, void*);
typedef fatrop_int (*FatropOcpCFullEvalConstrJac)(const double*, const double*, const double*, struct blasfeo_dmat *, struct blasfeo_dmat *, struct blasfeo_dmat *, const struct FatropOcpCDims*, void*);
typedef fatrop_int (*FatropOcpCFullEvalContrViol)(const double*, const double*, const double*, double *, const struct FatropOcpCDims*, void*);
typedef fatrop_int (*FatropOcpCFullEvalObjGrad)(double, const double*, const double*, const double*, double *, const struct FatropOcpCDims*, void*);
typedef fatrop_int (*FatropOcpCFullEvalObj)(double, const double*, const double*, const double*, double *, const struct FatropOcpCDims*, void*);

typedef void (*FatropOcpCWrite)(const char* msg, int num);
typedef void (*FatropOcpCFlush)(void);


struct FatropOcpCInterface
{
    /// @brief number of states for time step k
    /// @param k: time step
    FatropOcpCGetDim_k get_nx;
    /// @brief number of inputs for time step k
    /// @param k: time step
    FatropOcpCGetDim_k get_nu;
    /// @brief number of equality constraints for time step k
    /// @param k: time step
    FatropOcpCGetDim_k get_ng;
    /// @brief  number of stage parameters for time step k
    /// @param k: time step
    FatropOcpCGetDim_k get_n_stage_params;
    /// @brief number of global parameters
    FatropOcpCGetDim get_n_global_params;
    /// @brief default stage parameters for time step k
    /// @param stage_params: pointer to array of size n_stage_params_k
    /// @param k: time step
    FatropOcpCGetDouble_k get_default_stage_params;
    /// @brief default global parameters
    /// @param global_params: pointer to array of size n_global_params
    FatropOcpCGetDouble get_default_global_params;
    /// @brief number of inequality constraints for time step k
    /// @param k: time step
    FatropOcpCGetDim_k get_ng_ineq;
    /// @brief horizon length
    FatropOcpCGetDim get_horizon_length;
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
    FatropOcpCEval_BAbt_k eval_BAbt;
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
    FatropOcpCEval_RSQrqt_k eval_RSQrqt;
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
    FatropOcpCEval_Ggt_k eval_Ggt;
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
    FatropOcpCEval_Ggt_ineq_k eval_Ggt_ineq;
    /// @brief the dynamics constraint violation (b_k = -x_{k+1} + f_k(u_k, x_k, p_k, p))
    /// @param states_kp1: pointer to array states of time step k+1
    /// @param inputs_k: pointer to array inputs of time step k
    /// @param states_k: pointer to array states of time step k
    /// @param stage_params_k: pointer to array stage parameters of time step k
    /// @param global_params: pointer to array global parameters
    /// @param res: pointer to array nx_{k+1}-vector
    /// @param k: time step
    /// @return
    FatropOcpCEval_b_k eval_b;
    /// @brief the equality constraint violation (g_k = g_k(u_k, x_k, p_k, p))
    /// @param inputs_k: pointer to array inputs of time step k
    /// @param states_k: pointer to array states of time step k
    /// @param stage_params_k: pointer to array stage parameters of time step k
    /// @param global_params: pointer to array global parameters
    /// @param res: pointer to array ng-vector
    /// @param k: time step
    FatropOcpCEval_g_k eval_g;
    /// @brief the inequality constraint violation (g_ineq_k = g_ineq_k(u_k, x_k, p_k, p))
    /// @param inputs_k: pointer to array inputs of time step k
    /// @param states_k: pointer to array states of time step k
    /// @param stage_params_k: pointer to array stage parameters of time step k
    /// @param global_params: pointer to array global parameters
    /// @param res: pointer to array ng_ineq-vector
    /// @param k: time step
    FatropOcpCEval_gineq_k eval_gineq;
    /// @brief gradient of the objective function (not the Lagrangian!) to the concatenation of (u_k, x_k)
    /// @param objective_scale: pointer to objective scale
    /// @param inputs_k: pointer to array inputs of time step k
    /// @param states_k: pointer to array states of time step k
    /// @param stage_params_k: pointer to array stage parameters of time step k
    /// @param global_params: pointer to array global parameters
    /// @param res: pointer to (nu+nx)-array
    /// @param k: time step
    FatropOcpCEval_rq_k eval_rq;
    /// @brief objective function value 
    /// @param objective_scale: pointer to array objective scale
    /// @param inputs_k: pointer to array inputs of time step k
    /// @param states_k: pointer to array states of time step k
    /// @param stage_params_k: pointer to array stage parameters of time step k
    /// @param global_params: pointer to array global parameters
    /// @param res: pointer to double
    /// @param k: time step
    FatropOcpCEval_L_k eval_L;
    /// @brief the bounds of the inequalites at stage k
    /// @param lower: pointer to ng_ineq-vector
    /// @param upper: pointer to ng_ineq-vector
    /// @param k: time step
    FatropOcpCGetBounds_k get_bounds;
    /// @brief default initial guess for the states of stage k
    /// @param xk: pointer to states of time step k 
    /// @param k: time step
    FatropOcpCGetDouble_k get_initial_xk;
    /// @brief default initial guess for the inputs of stage k
    /// @param uk: pointer to inputs of time step k
    /// @param k: time step
    FatropOcpCGetDouble_k get_initial_uk;

    FatropOcpCFullEvalLagHess full_eval_lag_hess;
    FatropOcpCFullEvalConstrJac full_eval_constr_jac;
    FatropOcpCFullEvalContrViol full_eval_contr_viol;
    FatropOcpCFullEvalObjGrad full_eval_obj_grad;
    FatropOcpCFullEvalObj full_eval_obj;

    void* user_data;
};

/* 
    * @brief Create a new OCP solver
    * @param ocp_interface: pointer to the OCP interface
    * @param write: function pointer to write function (can be zero to indicate stdout)
    * @param flush: function pointer to flush function (can be zero)
    * @return pointer to the OCP solver
    
*/
FATROP_SYMBOL_EXPORT struct FatropOcpCSolver* fatrop_ocp_c_create(struct FatropOcpCInterface* ocp_interface, FatropOcpCWrite write, FatropOcpCFlush flush);

FATROP_SYMBOL_EXPORT fatrop_int fatrop_ocp_c_solve(struct FatropOcpCSolver*);

/* -1 for not found, 0 for double, 1 for int, 2 for bool, 3 for string */
FATROP_SYMBOL_EXPORT int fatrop_ocp_c_option_type(const char* name);
FATROP_SYMBOL_EXPORT int fatrop_ocp_c_set_option_double(struct FatropOcpCSolver* s, const char* name, double val);
FATROP_SYMBOL_EXPORT int fatrop_ocp_c_set_option_bool(struct FatropOcpCSolver* s, const char* name, int val);
FATROP_SYMBOL_EXPORT int fatrop_ocp_c_set_option_int(struct FatropOcpCSolver* s, const char* name, int val);
FATROP_SYMBOL_EXPORT int fatrop_ocp_c_set_option_string(struct FatropOcpCSolver* s, const char* name, const char* val);
FATROP_SYMBOL_EXPORT const struct blasfeo_dvec* fatrop_ocp_c_get_primal(struct FatropOcpCSolver* s);
FATROP_SYMBOL_EXPORT const struct blasfeo_dvec* fatrop_ocp_c_get_dual(struct FatropOcpCSolver* s);
FATROP_SYMBOL_EXPORT const struct FatropOcpCDims* fatrop_ocp_c_get_dims(struct FatropOcpCSolver* s);
FATROP_SYMBOL_EXPORT const struct FatropOcpCStats* fatrop_ocp_c_get_stats(struct FatropOcpCSolver* s);
FATROP_SYMBOL_EXPORT void fatrop_ocp_c_destroy(struct FatropOcpCSolver*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // OCPCINTERFACEINCLUDED
