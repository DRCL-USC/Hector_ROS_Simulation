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
#ifndef OCPEVALUATORINCLUDED
#define OCPEVALUATORINCLUDED
#include "OCPKKT.hpp"
#include "OCPAbstract.hpp"
#include "fatrop/solver/FatropData.hpp"
#include "OCP.hpp"
#include <memory>
#include "fatrop/auxiliary/Common.hpp"
#define OCPMACRO(type, name, suffix) type name##suffix = ((type)OCP->name)
#define AUXMACRO(type, name, suffix) type name##suffix = ((type)OCP->aux.name)
#define SOLVERMACRO(type, name, suffix) type name##suffix = ((type)name)
#ifdef ENABLE_MULTITHREADING
#include <omp.h>
#endif

// Example: you can make a for loop parallel with the following code. But to be further investigated to do this in an efficient way...
// #ifdef ENABLE_MULTITHREADING
// #pragma omp parallel for
// <your for loop>
// #endif

namespace fatrop
{
    class OCPAdapter : public OCP // public OCP -> also include KKTmemory, OCPDims, ...
    {
    public:
        OCPAdapter(const std::shared_ptr<OCPAbstract> &ocptempl_, const std::shared_ptr<FatropOptions> &options) : K(ocptempl_->get_horizon_length()),
                                                                                                                   nuexpr(TransformRange<fatrop_int>(0, K, [&ocptempl_](fatrop_int k)
                                                                                                                                                     { return ocptempl_->get_nu(k); })),
                                                                                                                   nxexpr(TransformRange<fatrop_int>(0, K, [&ocptempl_](fatrop_int k)
                                                                                                                                                     { return ocptempl_->get_nx(k); })),
                                                                                                                   ngexpr(TransformRange<fatrop_int>(0, K, [&ocptempl_](fatrop_int k)
                                                                                                                                                     { return ocptempl_->get_ng(k); })),
                                                                                                                   ngineqexpr(TransformRange<fatrop_int>(0, K, [&ocptempl_](fatrop_int k)
                                                                                                                                                         { return ocptempl_->get_ng_ineq(k); })),
                                                                                                                   nstageparamsexpr(TransformRange<fatrop_int>(0, K, [&ocptempl_](fatrop_int k)
                                                                                                                                                               { return ocptempl_->get_n_stage_params(k); })),
                                                                                                                   offs_stageparams(offsets(nstageparamsexpr)), stageparams(sum(nstageparamsexpr), 0.0), globalparams(ocptempl_->get_n_global_params(), 0.0), options(options), ocptempl(ocptempl_)
        {
#ifdef ENABLE_MULTITHREADING
            // check if environment variable OMP_NUM_THREADS is set
            if (getenv("OMP_NUM_THREADS") == NULL)
            {
                throw std::runtime_error("Environment variable OMP_NUM_THREADS is not set. Please set it to the number of threads you want to use or compile fatrop with multithreading disabled.");
            }
#endif

            // initialize the default parameters
            ocptempl_->get_default_global_params(globalparams.data());
            fatrop_int offs = 0;
            for (fatrop_int k = 0; k < K; k++)
            {
                ocptempl_->get_default_stage_params(stageparams.data() + offs, k);
                offs += ocptempl_->get_n_stage_params(k);
            }
            // initialize gradbuf
            // for (fatrop_int k = 0; k < K; k++)
            //     gradbuf.emplace_back(ocptempl_->get_nuk(k) + ocptempl_->get_nxk(k));
            x_dummy = std::vector<double>(maxel(nxexpr), 0.0);
        }
        void reset() override
        {
        }
        void print_kkt_matrix(OCPKKTMemory *OCP);
        fatrop_int eval_lag_hess(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            const FatropVecBF &lam) override;
        fatrop_int eval_constr_jac(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars) override;
        fatrop_int eval_contr_viol(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            const FatropVecBF &slack_vars,
            FatropVecBF &constraint_violation) override;
        fatrop_int eval_ineqs(
            OCPKKTMemory *OCP,
            const FatropVecBF &primal_vars,
            FatropVecBF &constraint_violation) override;
        fatrop_int eval_obj_grad(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            FatropVecBF &gradient_x) override;
        fatrop_int eval_obj(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &primal_vars,
            double &res) override;
        fatrop_int integrate_dynamics(
            OCPKKTMemory *OCP,
            const fatrop_int k,
            const FatropVecBF &uk,
            const FatropVecBF &xk,
            FatropVecBF &xkp1) override;
        OCPDims get_ocp_dims() const override
        {
            return OCPDims(ocptempl->get_horizon_length(), nuexpr, nxexpr, ngexpr, ngineqexpr, nstageparamsexpr, ocptempl->get_n_global_params());
        }

    public:
        void set_parameters(const std::vector<double> &stage_params_in, const std::vector<double> &global_params_in) override;
        void set_initial_sol_guess(const std::shared_ptr<FatropData> &fatropdata, std::vector<double> &initial_u, std::vector<double> &initial_x) override;
        void get_solution(const std::shared_ptr<FatropData> &fatropdata, std::vector<double> &u, std::vector<double> &x) override;
        double *get_global_parameters()
        {
            if (globalparams.size() == 0)
                return nullptr;
            return globalparams.data();
        }
        double *get_stage_parameters()
        {
            if (stageparams.size() == 0)
                return nullptr;
            return stageparams.data();
        }
        std::vector<double> &get_global_parameters_vec()
        {
            return globalparams;
        }
        std::vector<double> &get_stage_parameters_vec()
        {
            return stageparams;
        }
        fatrop_int get_bounds(
            FatropVecBF &lower,
            FatropVecBF &upper) const override
        {
            fatrop_int offs = 0;
            double *lower_p = ((VEC *)lower)->pa;
            double *upper_p = ((VEC *)upper)->pa;
            for (fatrop_int k = 0; k < K; k++)
            {
                ocptempl->get_bounds(lower_p + offs, upper_p + offs, k);
                offs += ocptempl->get_ng_ineq(k);
            }
            return 0;
        };
        fatrop_int get_initial_sol_guess(
            FatropVecBF &initial) const override
        {
            fatrop_int offs = 0;
            for (fatrop_int k = 0; k < K; k++)
            {
                ocptempl->get_initial_uk(((VEC *)initial)->pa + offs, k);
                offs += ocptempl->get_nu(k);
                ocptempl->get_initial_xk(((VEC *)initial)->pa + offs, k);
                offs += ocptempl->get_nx(k);
            }
            return 0;
        }
        // virtual fatrop_int GetDefaultParams(
        //     FatropOptions &params)
        // {
        // };

    public:
        fatrop_int K;
        FatropVector<fatrop_int> nuexpr;
        FatropVector<fatrop_int> nxexpr;
        FatropVector<fatrop_int> ngexpr;
        FatropVector<fatrop_int> ngineqexpr;
        FatropVector<fatrop_int> nstageparamsexpr;
        FatropVector<fatrop_int> offs_stageparams;
        std::vector<double> stageparams;
        std::vector<double> globalparams;
        std::vector<double> x_dummy;
        // std::vector<VECBF> gradbuf;
        std::shared_ptr<FatropOptions> options;

    private:
        std::shared_ptr<OCPAbstract> ocptempl;
    };
} // namespace fatrop

#endif // OCPEVALUATORINCLUDED