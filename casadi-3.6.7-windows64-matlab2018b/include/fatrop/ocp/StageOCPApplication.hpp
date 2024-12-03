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
#ifndef BASICOCPAPPLICATIONINCLUDED
#define BASICOCPAPPLICATIONINCLUDED
#include "fatrop/solver/FatropStats.hpp"
#include "fatrop/solver/FatropAlg.hpp"
#include "fatrop/ocp/StageOCPExpressions.hpp"
#include <map>
#include <fstream>
#include <sstream>
#include <string>

namespace fatrop
{
    // forward declarations to hide the implementation details
    class Journaller;
    class FatropAlg;
    class StageOCP;
    class NLPDims;
    class FatropVecBF;
    class FatropNLP;
    class FatropOptions;
    class FatropData;
    class OCPAdapter;
    class FatropPrinter;
    class OCPAbstract;
    class OCPDims;
    class OCP;
    class FatropSolution
    {
    public:
        // void GetPrimalSolution(vector<double> &result);
        // defautl copy constructor
        FatropSolution(const FatropSolution &other) = default;
        const std::vector<double> &primal_solution() { return sol_primal_; };

    protected:
        FatropSolution();
        void set_dims(const NLPDims &dims);
        void set_primal_solution(const FatropVecBF &sol);
        void set_solution(const FatropVecBF &sol_primal, const FatropVecBF &sol_dual, const FatropVecBF &sol_zL, const FatropVecBF &sol_zU);

    protected:
        std::vector<double> sol_primal_;
        std::vector<double> sol_dual_;
        std::vector<double> sol_zL_;
        std::vector<double> sol_zU_;
        friend class NLPApplication;
    };
    // TODO move this class to a separate file
    class NLPApplication
    {
    public:
        NLPApplication();

    protected:
        void build(const std::shared_ptr<FatropNLP> &nlp, const std::shared_ptr<FatropNLP> &nlp_resto);

    public:
        fatrop_int optimize() const;
        // TODO: make this protected and use last_solution instead and choose other name
        const FatropVecBF &last_solution_primal() const;
        const FatropVecBF &last_solution_dual() const;
        const FatropVecBF &last_solution_zL() const;
        const FatropVecBF &last_solution_zU() const;
        FatropVecBF &initial_guess_primal() const;
        FatropVecBF &initial_guess_dual() const;
        FatropVecBF &initial_guess_zL() const;
        FatropVecBF &initial_guess_zU() const;
        FatropStats get_stats() const;
        NLPDims get_nlp_dims();
        template <typename T>
        void set_option(const std::string &option_name, T value);

    public:
        void set_initial(const FatropSolution &initial_guess) const;
        void set_initial(const std::vector<double> &initial_guess_primal_) const;
        const FatropOptions &get_options() const;

    protected:
        std::shared_ptr<FatropOptions> fatropoptions_;
        std::shared_ptr<FatropData> fatropdata_;
        std::shared_ptr<FatropNLP> nlp_;
        bool dirty = true;
        std::shared_ptr<FatropPrinter> printer_;
        friend class OcpSolverDriver;
        
    private:
        const std::shared_ptr<OCPAbstract> ocp_;
        std::shared_ptr<Journaller> journaller_;
        std::shared_ptr<FatropAlg> fatropalg_;
    };

    class OCPApplication : public NLPApplication
    {
    public:
        OCPApplication(const std::shared_ptr<OCP> &ocp);
        OCPApplication();
        void build();
    public:
        using NLPApplication::set_initial;
        void set_initial(std::vector<double> &initial_u, std::vector<double> &initial_x);
        OCPDims get_ocp_dims();
        std::shared_ptr<OCP> ocp_;
    };

    // TODO move this class to a separate file
    class OCPAbstractApplication : public OCPApplication
    {
    public:
        OCPAbstractApplication(const std::shared_ptr<OCPAbstract> &ocp);
        void set_params(const std::vector<double> &global_params, const std::vector<double> &stage_params);

    protected:
        std::shared_ptr<OCPAdapter> adapter;

    protected:
        std::vector<double> &global_parameters();
        std::vector<double> &stage_parameters();
    };

    struct StageOCPSolution : public FatropSolution
    {
    public:
        StageOCPSolution(const std::shared_ptr<OCP> &app);
        void get_u(std::vector<double> &result) const;
        void get_x(std::vector<double> &result) const;
        int get_nx() const
        {
            return nx;
        };
        int get_nu() const
        {
            return nu;
        };
        int get_K() const
        {
            return K;
        };

    protected:
        StageOCPSolution();
        void set_dims(const OCPDims &dims);
        void set_parameters(const std::vector<double> &global_params, const std::vector<double> &stage_params);
        fatrop_int nx;
        fatrop_int nu;
        fatrop_int n_stage_params;
        fatrop_int n_global_params;
        fatrop_int K;

    public:
        // todo make this deprecated, only use Eval
        void sample(const StageControlGridSampler &sampler, std::vector<double> &result) const;
        std::vector<double> evaluate(const StageExpressionEvaluatorBase &evaluator) const;
        void evaluate(const StageExpressionEvaluatorBase &evaluator, std::vector<double> &result) const;

    protected:
        std::vector<double> global_params;
        std::vector<double> stage_params;
        friend class StageOCPApplication;
    };

    // struct StageOCPApplicationAbstract : public OCPApplication
    // {
    //     StageOCPApplicationAbstract(const shared_ptr<StageOCP> &ocp);
    // };
    class StageOCPApplication : public OCPAbstractApplication
    {
    public:
        StageOCPApplication(const std::shared_ptr<StageOCP> &ocp);

        void set_initial_u(const std::vector<double> &initial_guess_u) const;
        void set_initial_x(const std::vector<double> &initial_guess_x) const;

    public:
        class AppParameterSetter : public ParameterSetter
        {
        public:
            AppParameterSetter(const std::shared_ptr<OCPAdapter> &adapter, const std::shared_ptr<ParameterSetter> &ps);

        public:
            void set_value(const double value[]);
            void set_value(const std::initializer_list<double> il_);
            void set_value(const std::vector<double> il_)
            {
                set_value(il_.data());
            };

        private:
            const std::shared_ptr<OCPAdapter> adapter_;
        };

    public:
        StageExpressionEvaluatorFactory get_expression(const std::string &sampler_name);
        StageExpressionEvaluatorFactory get_expression_evaluator(const std::shared_ptr<StageExpression> &expr);
        AppParameterSetter get_parameter_setter(const std::string &setter_name);
        void set_value(const std::string &setter_name, const std::vector<double>& value);
        void build();
        fatrop_int optimize();
        const StageOCPSolution &last_solution() const;
        // const FatropSolution &last_solution() const { return last_solution(); };
        const std::vector<std::string> parameter_names() const;
        const std::vector<std::string> stage_expression_names() const;

    public:
        const fatrop_int nx_;
        const fatrop_int nu_;
        const fatrop_int n_stage_params_;
        const fatrop_int K_;

    private:
        StageOCPSolution last_solution_;

    protected:
        std::map<std::string, std::shared_ptr<StageExpression>> stage_expressions;
        std::map<std::string, std::shared_ptr<ParameterSetter>> param_setters;
        friend class StageOCPApplicationFactory;
    };

    class StageOCPApplicationFactory
    {
    public:
        static StageOCPApplication from_rockit_interface(const std::string &functions, const std::string &json_spec_file);
    };
}
#endif // BASICOCPAPPLICATIONINCLUDED