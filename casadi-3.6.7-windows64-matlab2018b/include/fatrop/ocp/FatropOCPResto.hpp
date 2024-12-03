#pragma once
#include "FatropOCP.hpp"
#include "fatrop/solver/FatropOptions.hpp"
#include <memory>
namespace fatrop
{
    class FatropOCPResto : public FatropNLP
    {
    public:
        FatropOCPResto(const std::shared_ptr<FatropOCP> &orig, const std::shared_ptr<FatropOptions> &opts) : orig_(orig), orig_dims_(orig->get_nlp_dims()), lower_(orig_dims_.nineqs), upper_(orig_dims_.nineqs), x_start_(orig_dims_.nvars), s_start_(orig_dims_.nineqs), x_tmp_(orig_dims_.nvars), s_tmp_(orig_dims_.nineqs), upper_bounded_(orig_dims_.nineqs), lower_bounded_(orig_dims_.nineqs), slack_dummy_(orig_dims_.nineqs), sigma_dummy_(orig_dims_.nineqs), gradb_dummy_(orig_dims_.nineqs), zl_dummy_(orig_dims_.nineqs), zu_dummy_(orig_dims_.nineqs), sigma_cache_(orig_dims_.nineqs * 3), gradb_cache_(orig_dims_.nineqs * 3)
        {
            opts->register_option(DoubleOption::lower_bounded("resto_rho", "Resto L1 penalty parameter", &rho, 1000., 0.0));
            opts->register_option(DoubleOption::lower_bounded("resto_xi", "Resto xi parameter", &xi, 1., 0.0));
            auto lower_v = lower_[0];
            auto upper_v = upper_[0];
            orig_->get_bounds(lower_v, upper_v);
            n_p = orig_dims_.nineqs;
            n_n = orig_dims_.nineqs;
            for (fatrop_int i = 0; i < orig_dims_.nineqs; ++i)
            {
                bool lower_bounded = !std::isinf(lower_v.at(i));
                bool upper_bounded = !std::isinf(upper_v.at(i));
                upper_bounded_[i] = upper_bounded;
                lower_bounded_[i] = lower_bounded;
            }
            this_dims_.nvars = orig_dims_.nvars;
            this_dims_.nineqs = orig_dims_.nineqs + n_n + n_p;
            this_dims_.neqs = orig_dims_.neqs;
        }
        virtual void pre_solve(const FatropVecBF &x_init, const FatropVecBF &s_init) override
        {
            x_start_[0].copy(x_init);
            s_start_[0].copy(s_init.block(0, orig_dims_.nineqs));
        };
        virtual fatrop_int eval_lag_hess(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            const FatropVecBF &lam) override
        {
            FatropVecBF slack_dummy_v = slack_dummy_[0];
            update_slack_vars(slack_vars, slack_dummy_v);
            orig_->eval_lag_hess(0.0, primal_vars, slack_dummy_v, lam);
            // compute delta's
            axpby(xi, primal_vars, -xi, x_start_[0], x_tmp_[0]);
            axpby(xi, slack_vars.block(0, orig_dims_.nineqs), -xi, s_start_[0], s_tmp_[0]);
            // iterate over all RSQrq's

            {
                OCPKKTMemory* OCP =&this->orig_->ocpkktmemory_;
                // horizon length
                fatrop_int K = OCP->K;
                // offsets
                const fatrop_int *offs_ux = (const fatrop_int *)OCP->aux.ux_offs.data();
                OCPMACRO(MAT *, RSQrqt, _p);
                VEC* x_tmp_p = (VEC*) x_tmp_[0];
                OCPMACRO(fatrop_int *, nu, _p);
                OCPMACRO(fatrop_int *, nx, _p);
                for (fatrop_int k = 0; k < K; k++)
                {
                    fatrop_int nu_k = nu_p[k];
                    fatrop_int nx_k = nx_p[k];
                    fatrop_int offs_ux_k = offs_ux[k];
                    // add xi to RSQrq diagonal
                    DIARE(nx_k + nu_k, xi, RSQrqt_p + k, 0, 0);
                    // add gradient to rq
                    ROWAD(nu_k + nx_k, 1.0, x_tmp_p, offs_ux_k, RSQrqt_p+k, nu_k + nx_k, 0);
                }
            }
            return 0;
        };

        virtual fatrop_int eval_constr_jac(
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars) override
        {
            FatropVecBF slack_dummy_v = slack_dummy_[0];
            update_slack_vars(slack_vars, slack_dummy_v);
            orig_->eval_constr_jac(primal_vars, slack_dummy_v);
            return 0;
        };
        void update_slack_vars(const FatropVecBF &slack_vars, FatropVecBF &slack_dummy)
        {
            fatrop_int offs_n = orig_dims_.nineqs;
            fatrop_int offs_p = orig_dims_.nineqs + n_n;
            for (fatrop_int i = 0; i < orig_dims_.nineqs; ++i)
            {
                slack_dummy.at(i) = slack_vars.at(i) - slack_vars.at(i + offs_n) + slack_vars.at(i + offs_p);
            }
        }
        virtual fatrop_int eval_constraint_viol(
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &constraint_violation) override
        {
            FatropVecBF slack_dummy_v = slack_dummy_[0];
            update_slack_vars(slack_vars, slack_dummy_v);
            orig_->eval_constraint_viol(primal_vars, slack_dummy_v, constraint_violation);
            return 0;
        };
        virtual fatrop_int eval_obj_grad(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &gradient_x,
            FatropVecBF &gradient_s) override
        {
            // compute delta's
            axpby(xi, primal_vars, -xi, x_start_[0], gradient_x);
            axpby(xi, slack_vars.block(0, orig_dims_.nineqs), -xi, s_start_[0], gradient_s.block(0, orig_dims_.nineqs));
            gradient_s.block(orig_dims_.nineqs, n_n + n_p) = rho;
            return 0;
        };
        virtual fatrop_int eval_obj(
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            double &res) override
        {
            // compute delta's
            axpby(1.0, primal_vars, -1.0, x_start_[0], x_tmp_[0]);
            axpby(1.0, slack_vars.block(0, orig_dims_.nineqs), -1.0, s_start_[0], s_tmp_[0]);
            res = 0.5*xi*(sumsqr(x_tmp_[0]) + sumsqr(s_tmp_[0]));
            res += rho * sum(slack_vars.block(orig_dims_.nineqs, n_n + n_p));
            return 0;
        };
        void get_initialization(const FatropVecBF &primal_vars_orig, FatropVecBF &intialization)
        {
            intialization.block(0, orig_dims_.nvars).copy(primal_vars_orig.block(0, orig_dims_.nvars));
        }
        virtual fatrop_int eval_dual_inf(
            double obj_scale,
            const FatropVecBF &lam,
            const FatropVecBF &grad_x,
            const FatropVecBF &grad_s,
            FatropVecBF &du_inf_x, FatropVecBF &du_inf_s_wo_z) override
        {
            auto du_inf_s_wo_z_or = du_inf_s_wo_z.block(0, orig_dims_.nineqs);
            orig_->eval_dual_inf(obj_scale, lam, grad_x, grad_s, du_inf_x, du_inf_s_wo_z_or);
            auto lam_I = lam.block(orig_dims_.neqs - orig_dims_.nineqs, orig_dims_.nineqs);
            axpby(1.0, lam_I, 1.0, grad_s.block(orig_dims_.nineqs, n_n), du_inf_s_wo_z.block(orig_dims_.nineqs, n_n));
            axpby(-1.0, lam_I, 1.0, grad_s.block(orig_dims_.nineqs + n_n, n_p), du_inf_s_wo_z.block(orig_dims_.nineqs + n_n, n_p));
            return 0;
        }
        void update_sigma_gradb(double inertia, const FatropVecBF &sigma_s, const FatropVecBF &sigma_n, const FatropVecBF &sigma_p, const FatropVecBF &gradb_s, const FatropVecBF &gradb_n, const FatropVecBF &gradb_p, const FatropVecBF &sigma_update, const FatropVecBF &gradb_update)
        {
            for (int i = 0; i < orig_dims_.nineqs; ++i)
            {
                double sigma_updt = 1.0 / (1.0 / (sigma_s.at(i) + inertia + xi) + 1.0 / (sigma_n.at(i) + inertia) + 1.0 / (sigma_p.at(i) + inertia));
                sigma_update.at(i) = sigma_updt - inertia;
                gradb_update.at(i) = ((gradb_s.at(i)) / (sigma_s.at(i) + inertia + xi) - (gradb_n.at(i)) / (sigma_n.at(i) + inertia) + (gradb_p.at(i)) / (sigma_p.at(i) + inertia)) * sigma_updt;
            }
        }
        void update_delta_snp(double inertia, const FatropVecBF &sigma_s, const FatropVecBF &sigma_n, const FatropVecBF &sigma_p, const FatropVecBF &gradb_s, const FatropVecBF &gradb_n, const FatropVecBF &gradb_p, const FatropVecBF &lam_I, const FatropVecBF &delta_s, const FatropVecBF &delta_n, const FatropVecBF &delta_p)
        {
            for (int i = 0; i < orig_dims_.nineqs; ++i)
            {
                double lam_I_i = lam_I.at(i);
                delta_s.at(i) = (-gradb_s.at(i) + lam_I_i) / (sigma_s.at(i) + inertia+xi);
                delta_n.at(i) = (-gradb_n.at(i) - lam_I_i) / (sigma_n.at(i) + inertia);
                delta_p.at(i) = (-gradb_p.at(i) + lam_I_i) / (sigma_p.at(i) + inertia);
            }
        }

        virtual fatrop_int solve_pd_sys(
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &gradb_total) override
        {
            // (todo) check if quadratic penalty on delta s is treated right
            inertia_correction_w_cache = inertia_correction_w;
            sigma_cache_[0].copy(sigma_total);
            gradb_cache_[0].copy(gradb_total);
            auto lam_I = lam.block(orig_dims_.neqs - orig_dims_.nineqs, orig_dims_.nineqs);
            auto sigma_s = sigma_total.block(0, orig_dims_.nineqs);
            auto sigma_n = sigma_total.block(orig_dims_.nineqs, n_n);
            auto sigma_p = sigma_total.block(orig_dims_.nineqs + n_n, n_p);
            auto gradb_s = gradb_total.block(0, orig_dims_.nineqs);
            auto gradb_n = gradb_total.block(orig_dims_.nineqs, n_n);
            auto gradb_p = gradb_total.block(orig_dims_.nineqs + n_n, n_p);
            auto delta_s_or = delta_s.block(0, orig_dims_.nineqs);
            auto delta_n = delta_s.block(orig_dims_.nineqs, n_n);
            auto delta_p = delta_s.block(orig_dims_.nineqs + n_n, n_p);
            update_sigma_gradb(inertia_correction_w, sigma_s, sigma_n, sigma_p, gradb_s, gradb_n, gradb_p, sigma_dummy_[0], gradb_dummy_[0]);
            int ret = orig_->solve_pd_sys(inertia_correction_w, inertia_correction_c, ux, lam, delta_s.block(0, orig_dims_.nineqs), sigma_dummy_[0], gradb_dummy_[0]);
            update_delta_snp(inertia_correction_w, sigma_s, sigma_n, sigma_p, gradb_s, gradb_n, gradb_p, lam_I, delta_s_or, delta_n, delta_p);
            return ret;
        };
        virtual fatrop_int solve_soc_rhs(
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &cosntraint_violation) override
        {
            auto lam_I = lam.block(orig_dims_.neqs - orig_dims_.nineqs, orig_dims_.nineqs);
            auto sigma_s = sigma_cache_[0].block(0, orig_dims_.nineqs);
            auto sigma_n = sigma_cache_[0].block(orig_dims_.nineqs, n_n);
            auto sigma_p = sigma_cache_[0].block(orig_dims_.nineqs + n_n, n_p);
            auto gradb_s = gradb_cache_[0].block(0, orig_dims_.nineqs);
            auto gradb_n = gradb_cache_[0].block(orig_dims_.nineqs, n_n);
            auto gradb_p = gradb_cache_[0].block(orig_dims_.nineqs + n_n, n_p);
            auto delta_s_or = delta_s.block(0, orig_dims_.nineqs);
            auto delta_n = delta_s.block(orig_dims_.nineqs, n_n);
            auto delta_p = delta_s.block(orig_dims_.nineqs + n_n, n_p);
            int ret = orig_->solve_soc_rhs(ux, lam, delta_s.block(0, orig_dims_.nineqs), cosntraint_violation);
            update_delta_snp(inertia_correction_w_cache + xi, sigma_s, sigma_n, sigma_p, gradb_s, gradb_n, gradb_p, lam_I, delta_s_or, delta_n, delta_p);
            return ret;
        }
        virtual NLPDims get_nlp_dims() const override { return this_dims_; };
        virtual fatrop_int compute_scalings(
            double &obj_scale,
            FatropVecBF &x_scales,
            FatropVecBF &lam_scales,
            const FatropVecBF &grad_curr_x, const FatropVecBF &grad_curr_s) override
        {
            return orig_->compute_scalings(obj_scale, x_scales, lam_scales, grad_curr_x, grad_curr_s);
        };
        virtual fatrop_int initialize_slacks(double mu0,
                                             FatropVecBF &s_curr) override
        {
            auto s_curr_or = s_curr.block(0, orig_dims_.nineqs);
            auto n_curr = s_curr.block(orig_dims_.nineqs, n_n);
            auto p_curr = s_curr.block(orig_dims_.nineqs + n_n, n_p);
            FatropVecBF ineq_viol = s_tmp_[0];
            int ret = orig_->initialize_slacks(mu0, ineq_viol);
            for (int i = 0; i < orig_dims_.nineqs; i++)
            {
                double viol = 0.0;
                viol = (ineq_viol.at(i));
                double n_init = (mu0 - rho * viol) / (2 * rho) + std::sqrt(std::pow((mu0 - rho * viol) / (2 * rho), 2) + mu0 * viol / (2 * rho));
                // if viol >>>> 0 -> n_init = 0 if viol <<< 0 n_init = viol
                n_curr.at(i) = n_init;
                p_curr.at(i) = viol + n_init;
            }
            return 0;
        }
        virtual fatrop_int initialize_dual(
            const FatropVecBF &grad_x,
            const FatropVecBF &grad_s,
            FatropVecBF &dlam,
            const FatropVecBF &zL,
            const FatropVecBF &zU) override
        {
            // todo check if this is correct
            // return orig_->initialize_dual(grad_x, grad_s, dlam, zL.block(0, orig_dims_.nineqs), zU.block(0, orig_dims_.nineqs));
            return 0;
        };
        virtual fatrop_int get_bounds(
            FatropVecBF &lower,
            FatropVecBF &upper) const override
        {
            orig_->get_bounds(lower, upper);
            lower.block(orig_dims_.nineqs, n_n) = 0.0;
            lower.block(orig_dims_.nineqs + n_n, n_p) = 0.0;
            upper.block(orig_dims_.nineqs, n_n) = std::numeric_limits<double>::infinity();
            upper.block(orig_dims_.nineqs + n_n, n_p) = std::numeric_limits<double>::infinity();
            return 0;
        };
        virtual fatrop_int get_initial_sol_guess(
            FatropVecBF &initial) const override
        {
            orig_->get_initial_sol_guess(initial);
            return 0;
        }
        virtual void update_mu(double mu) override
        {
            xi = std::sqrt(mu);
            orig_->update_mu(mu);
        };
        virtual void reset() override
        {
            orig_->reset();
        };
        std::shared_ptr<FatropOCP> orig_;
        NLPDims orig_dims_;
        NLPDims this_dims_;
        FatropMemoryVecBF lower_, upper_;
        FatropMemoryVecBF x_start_, s_start_;
        FatropMemoryVecBF x_tmp_, s_tmp_;
        std::vector<bool> upper_bounded_;
        std::vector<bool> lower_bounded_;
        fatrop_int n_p = 0;
        fatrop_int n_n = 0;
        FatropMemoryVecBF slack_dummy_;
        FatropMemoryVecBF sigma_dummy_;
        FatropMemoryVecBF gradb_dummy_;
        FatropMemoryVecBF zl_dummy_;
        FatropMemoryVecBF zu_dummy_;
        double inertia_correction_w_cache = 0.0;
        FatropMemoryVecBF sigma_cache_;
        FatropMemoryVecBF gradb_cache_;
        double rho = 1e4;
        double xi = 1.0;
    };
};