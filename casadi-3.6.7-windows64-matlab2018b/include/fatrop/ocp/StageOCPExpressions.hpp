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
#ifndef BASICOCPSAMPLERSINCLUDED
#define BASICOCPSAMPLERSINCLUDED
#include <memory>
#include <vector>
#include <fatrop/auxiliary/Common.hpp>
// todo change the name of this file to evaluators
namespace fatrop
{
    class EvalBase;
    class StageExpression
    {
    public:
        virtual void evaluate(const double *u, const double *x, const double *global_params, const double *stage_params, double *res) const = 0;
        virtual fatrop_int n_rows() const = 0;
        virtual fatrop_int n_cols() const= 0;
        fatrop_int size() const;
    };
    class IndexEpression : public StageExpression
    {
    public:
        IndexEpression(const bool control, const std::vector<fatrop_int> offsets_in, const std::vector<fatrop_int> offsets_out);
        void evaluate(const double *u, const double *x, const double *global_params, const double *stage_params, double *res) const override;
        fatrop_int n_rows() const override;
        fatrop_int n_cols() const override;

    private:
        const fatrop_int _no_var;
        const std::vector<fatrop_int> _offsets_in;
        const std::vector<fatrop_int> _offsets_out;
        const bool _control;
    };
    class EvalBaseSE : public StageExpression
    {
    public:
        EvalBaseSE(const std::shared_ptr<EvalBase> &evalbase);
        void evaluate(const double *u, const double *x, const double *global_params, const double *stage_params, double *res) const override;
        fatrop_int n_rows() const override;
        fatrop_int n_cols() const override;

    private:
        std::shared_ptr<EvalBase> evalbase_;
        const fatrop_int n_rows_;
        const fatrop_int n_cols_;
    };
    class StageExpressionEvaluatorBase
    {
    public:
        virtual fatrop_int size() const = 0;
        virtual fatrop_int n_rows() const= 0;
        virtual fatrop_int n_cols() const = 0;
        virtual fatrop_int evaluate(const std::vector<double> &solution, const std::vector<double> &global_params, const std::vector<double> &stage_params, std::vector<double> &result) const = 0;
        std::vector<double> evaluate(const std::vector<double> &solution, const std::vector<double> &global_params, const std::vector<double> &stage_params) const
        {
            std::vector<double> res(size());
            evaluate(solution, global_params, stage_params, res);
            return res;
        };
        virtual ~StageExpressionEvaluatorBase() = default;

    protected:
        // virtual fatrop_int Evaluate()
    };
    class StageControlGridSampler : public StageExpressionEvaluatorBase
    {
    public:
        StageControlGridSampler(fatrop_int nu, fatrop_int nx, fatrop_int no_stage_params, fatrop_int K, const std::shared_ptr<StageExpression> &eval);
        fatrop_int evaluate(const std::vector<double> &solution, const std::vector<double> &global_params, const std::vector<double> &stage_params, std::vector<double> &result) const;
        fatrop_int size() const;
        fatrop_int n_rows() const;
        fatrop_int n_cols() const;
        fatrop_int K() const;
        const fatrop_int nu;
        const fatrop_int nx;
        const fatrop_int no_stage_params;
        const fatrop_int K_;

    private:
        std::shared_ptr<StageExpression> eval_;
    };

    class OCPTimeStepSampler : public StageExpressionEvaluatorBase
    {
    public:
        OCPTimeStepSampler(fatrop_int nu, fatrop_int nx, fatrop_int no_stage_params, fatrop_int K, fatrop_int k, const std::shared_ptr<StageExpression> &eval) : nu(nu), nx(nx), no_stage_params(no_stage_params), K_(K), k_(k), eval_(eval)
        {
        }
        fatrop_int evaluate(const std::vector<double> &solution, const std::vector<double> &global_params, const std::vector<double> &stage_params, std::vector<double> &result) const
        {
            eval_->evaluate(solution.data() + (k_ < K_ - 1 ? k_ * (nu + nx) : (K_ - 2) * (nu + nx)), solution.data() + (k_ < K_ - 1 ? k_ * (nu + nx) + nu : (K_ - 1) * (nu + nx)), global_params.data(), stage_params.data() + k_ * no_stage_params, result.data());
            return 0;
        }
        fatrop_int size() const
        {
            return eval_->size();
        }
        fatrop_int n_rows() const
        {
            return eval_->n_rows();
        }
        fatrop_int n_cols() const
        {
            return eval_->n_cols();
        }
        const fatrop_int nu;
        const fatrop_int nx;
        const fatrop_int no_stage_params;
        const fatrop_int K_;
        const fatrop_int k_;

    private:
        std::shared_ptr<StageExpression> eval_;
    };
    class StageExpressionEvaluatorFactory
    {
    public:
        StageExpressionEvaluatorFactory(const std::shared_ptr<StageExpression> &eval, fatrop_int nu, fatrop_int nx, fatrop_int no_stage_params, fatrop_int K) : nu(nu), nx(nx), no_stage_params(no_stage_params), K(K), eval_(eval){};
        // at_t0()
        OCPTimeStepSampler at_t0() const
        {
            return OCPTimeStepSampler(nu, nx, no_stage_params, K, 0, eval_);
        }
        // at_tf()
        OCPTimeStepSampler at_tf() const
        {
            return OCPTimeStepSampler(nu, nx, no_stage_params, K, K - 1, eval_);
        }
        // at_tk(fatrop_int k)
        OCPTimeStepSampler at_tk(const fatrop_int k) const
        {
            return OCPTimeStepSampler(nu, nx, no_stage_params, K, k, eval_);
        }
        // evaluate at control grid
        StageControlGridSampler at_control() const
        {
            return StageControlGridSampler(nu, nx, no_stage_params, K, eval_);
        }
        const fatrop_int nu;
        const fatrop_int nx;
        const fatrop_int no_stage_params;
        const fatrop_int K;
        std::shared_ptr<StageExpression> eval_;
    };

    class ParameterSetter
    {
    public:
        ParameterSetter(const std::vector<fatrop_int> &offsets_in, const std::vector<fatrop_int> &offsets_out, const fatrop_int no_stage_params, const fatrop_int no_var, const fatrop_int K, const bool global);
        void set_value(std::vector<double> &global_params, std::vector<double> &stage_params, const double value[]);
        void set_value(std::vector<double> &global_params, std::vector<double> &stage_params, const std::initializer_list<double> il_);

    private:
        const std::vector<fatrop_int> _offsets_in;
        const std::vector<fatrop_int> _offsets_out;
        const fatrop_int no_stage_params;

    protected:
        const fatrop_int _no_var;
        const fatrop_int K;
        const bool _global;
    };

} // namespace fatrop

#endif //  BASICOCPSAMPLERSINCLUDED