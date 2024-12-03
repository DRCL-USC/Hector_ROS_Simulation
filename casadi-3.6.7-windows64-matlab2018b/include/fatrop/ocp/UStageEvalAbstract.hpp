#pragma once
extern "C"
{
#include <blasfeo.h>
}
#define MAT blasfeo_dmat
namespace fatrop
{
        class UStageEvalAbstract
        {
        public:
            /// @brief number of states for time step k
            /// @param k: time step
            virtual int get_nx() const = 0;
            /// @brief number of inputs for time step k
            /// @param k: time step
            virtual int get_nu() const = 0;
            /// @brief number of equality constraints for time step k
            /// @param k: time step
            virtual int get_ng() const = 0;
            /// @brief  number of stage parameters for time step k
            /// @param k: time step
            virtual int get_n_stage_params() const = 0;
            /// @brief default global parameters
            /// @param global_params: pointer to array of size n_global_params
            virtual int get_ng_ineq() const = 0;
            /// @brief  discretized dynamics
            /// it evaluates the vertical concatenation of A_k^T, B_k^T, and b_k^T from the linearized dynamics x_{k+1} = A_k x_k + B_k u_k + b_k.
            /// The matrix is in column major format.
            /// @param states_kp1: pointer to nx_{k+1}-array states of time step k+1
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to (nu+nx+1 x nu+nx)-matrix
            virtual int eval_BAbt(
                const double *states_kp1,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res) = 0;
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
            virtual int eval_RSQrqt(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *lam_dyn_k,
                const double *lam_eq_k,
                const double *lam_eq_ineq_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res) = 0;
            /// @brief stagewise equality constraints Jacobian.
            /// It evaluates the vertical concatenation of (1) the Jacobian of the equality constraints to the concatenation of (u_k, x_k) (2) the equality constraints evaluated at u_k, x_k.
            /// The matrix is in column major format.
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to (nu+nx+1 x ng)-matrix.
            /// @return
            virtual int eval_Ggt(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res) = 0;
            /// @brief stagewise inequality constraints Jacobian.
            /// It evaluates the vertical concatenation of (1) the Jacobian of the inequality constraints to the concatenation of (u_k, x_k) (2) the inequality constraints evaluated at u_k, x_k.
            /// The matrix is in column major format.
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params_ko: pointer to array global parameters
            /// @param res: pointer to (nu+nx+1 x ng_ineq)-matrix, column major format
            virtual int eval_Ggt_ineq(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res) = 0;
            /// @brief the dynamics constraint violation (b_k = -x_{k+1} + f_k(u_k, x_k, p_k, p))
            /// @param states_kp1: pointer to array states of time step k+1
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to array nx_{k+1}-vector
            /// @return
            virtual int eval_b(
                const double *states_kp1,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res) = 0;
            /// @brief the equality constraint violation (g_k = g_k(u_k, x_k, p_k, p))
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to array ng-vector
            virtual int eval_g(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res) = 0;
            /// @brief the inequality constraint violation (g_ineq_k = g_ineq_k(u_k, x_k, p_k, p))
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to array ng_ineq-vector
            virtual int eval_gineq(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res) = 0;
            /// @brief gradient of the objective function (not the Lagrangian!) to the concatenation of (u_k, x_k)
            /// @param objective_scale: pointer to objective scale
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to (nu+nx)-array
            virtual int eval_rq(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res) = 0;
            /// @brief objective function value
            /// @param objective_scale: pointer to array objective scale
            /// @param inputs_k: pointer to array inputs of time step k
            /// @param states_k: pointer to array states of time step k
            /// @param stage_params_k: pointer to array stage parameters of time step k
            /// @param global_params: pointer to array global parameters
            /// @param res: pointer to double
            virtual int eval_L(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res) = 0;
            /// @brief the bounds of the inequalites at stage k
            /// @param lower: pointer to ng_ineq-vector
            /// @param upper: pointer to ng_ineq-vector
            virtual int get_bounds(double *lower, double *upper) const = 0;
            /// @brief default initial guess for the states of stage k
            /// @param xk: pointer to states of time step k
        };
}