#pragma once
#include <string>
#include "fatrop/ocp/OCPAbstract.hpp"
#include "fatrop/ocp/UStageEvalAbstract.hpp"
#include <memory>
namespace fatrop
{
        template<typename TUStageEval>
        class UStageOCPImpl : public OCPAbstract
        {
        public:
            UStageOCPImpl(std::vector<std::shared_ptr<TUStageEval>> && ustages, int n_global_parameters): ustages_(std::move(ustages)), n_global_parameters_(n_global_parameters)
            {
                horizon_length_ = ustages_.size();
            }
            fatrop_int get_nx(const fatrop_int k) const override { return ustages_[k]->get_nx(); };
            fatrop_int get_nu(const fatrop_int k) const override { return ustages_[k]->get_nu(); };
            fatrop_int get_ng(const fatrop_int k) const override { return ustages_[k]->get_ng(); };
            fatrop_int get_n_stage_params(const fatrop_int k) const override { return ustages_[k]->get_n_stage_params(); };
            fatrop_int get_n_global_params() const override { return n_global_parameters_; };
            fatrop_int get_default_stage_params(double *stage_params, const fatrop_int k) const override { return 0; };
            fatrop_int get_default_global_params(double *global_params) const override { return 0; };
            fatrop_int get_ng_ineq(const fatrop_int k) const override { return ustages_[k]->get_ng_ineq(); };
            fatrop_int get_horizon_length() const override { return horizon_length_; };
            fatrop_int eval_BAbt(
                const double *states_kp1,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_BAbt(states_kp1, inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_RSQrqt(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *lam_dyn_k,
                const double *lam_eq_k,
                const double *lam_eq_ineq_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_RSQrqt(objective_scale, inputs_k, states_k, lam_dyn_k, lam_eq_k, lam_eq_ineq_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_Ggt(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_Ggt(inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_Ggt_ineq(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                MAT *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_Ggt_ineq(inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_b(
                const double *states_kp1,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_b(states_kp1, inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_g(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_g(inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_gineq(
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_gineq(inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_rq(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_rq(objective_scale, inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int eval_L(
                const double *objective_scale,
                const double *inputs_k,
                const double *states_k,
                const double *stage_params_k,
                const double *global_params,
                double *res,
                const fatrop_int k) override
            {
                return ustages_[k]->eval_L(objective_scale, inputs_k, states_k, stage_params_k, global_params, res);
            };
            fatrop_int get_bounds(double *lower, double *upper, const fatrop_int k) const override
            {
                return ustages_[k]->get_bounds(lower, upper);
            };
            fatrop_int get_initial_xk(double *xk, const fatrop_int k) const override
            {
                // initialization is done higher up
                for (int i = 0; i < get_nx(k); i++)
                {
                    xk[i] = 0;
                }
                return 0;
            };
            fatrop_int get_initial_uk(double *uk, const fatrop_int k) const override
            {
                // initialization is done higher up
                for (int i = 0; i < get_nu(k); i++)
                {
                    uk[i] = 0;
                }
                return 0;
            };

        private:
            std::vector<std::shared_ptr<TUStageEval>> ustages_;
            int horizon_length_;
            int n_global_parameters_;
        };
        // implementation of OCPAbstract, given an OCP
} // namespace fatrop