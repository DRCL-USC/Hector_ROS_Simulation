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

// Basic OCP template: initial and terminal constraints eq constraints, Function evaluation provided by Casadi CodeGen
#ifndef OCPTEMPLATEBASICINCLUDED
#define OCPTEMPLATEBASICINCLUDED
#include "OCPAbstract.hpp"
#include <string>
#include <iostream>
#include <fatrop/auxiliary/DynamicLib.hpp>
#include <fatrop/auxiliary/Common.hpp>
#include "fatrop/function_evaluation/CasadiCodegen.hpp"
#include "fatrop/json/json.h"
#include <fstream>
#include <sstream>

namespace fatrop
{
    /* TODO: at this pofatrop_int the StageOCP's implementation implements function evaluation through Casadi Codegen 
    *  it should become an interface that can be implemented by user defined function evaluation methods 
    *  the StageOCP is then characterised by its structure that is a bit less general than the OCPAbstract interface
    *  more in particular it seperates initial, intermediate and terminal stages, where it has different constraints and objectives
    *  see the commented class at the end of this file
    * 
    *  the casadi implementation should become a derived class of this interface
    */ 

    /// BasicOCP is a OCPAbstract implementation that is built on top of CasAdi Codegenerated functions 
    /// it seperates the initial and terminal stages and the intermediate stages, each of these can have different constraints and running objective
    /// the dynamics is the same for all stages
    /// this problem structure corresponds to a single-stage rockit problem, which it is intented to be used for
    class StageOCP : public OCPAbstract
    {
    public:
        StageOCP(const fatrop_int nu, const fatrop_int nx, const fatrop_int ngI, const fatrop_int ng, const fatrop_int ngF, const fatrop_int ng_ineqI, const fatrop_int ng_ineq, const fatrop_int ng_ineqF, const fatrop_int n_stage_params, const fatrop_int n_global_params, const fatrop_int K)
        :
        nu_(nu),
        nx_(nx),
        ngI_(ngI),
        ng_(ng),
        ngF_(ngF),
        ng_ineqI_(ng_ineqI),
        ng_ineq_(ng_ineq),
        ng_ineqF_(ng_ineqF),
        n_stage_params_(n_stage_params),
        n_global_params_(n_global_params),
        K_(K)
        {
        }
        fatrop_int get_nx(const fatrop_int k) const override;
        fatrop_int get_nu(const fatrop_int k) const override;
        fatrop_int get_ng(const fatrop_int k) const override;
        fatrop_int get_ng_ineq(const fatrop_int k) const override;
        fatrop_int get_n_global_params() const override;
        fatrop_int get_n_stage_params(const fatrop_int k) const override;
        fatrop_int get_horizon_length() const override;
        const fatrop_int nu_;
        const fatrop_int nx_;
        const fatrop_int ngI_;
        const fatrop_int ng_;
        const fatrop_int ngF_;
        const fatrop_int ng_ineqI_;
        const fatrop_int ng_ineq_;
        const fatrop_int ng_ineqF_;
        const fatrop_int n_stage_params_;
        const fatrop_int n_global_params_;
        const fatrop_int K_;
    };
    class StageOCPRockit : public StageOCP
    {
    public:
        StageOCPRockit(const fatrop_int nu,
                 const fatrop_int nx,
                 const fatrop_int ngI,
                 const fatrop_int ng,
                 const fatrop_int ngF,
                 const fatrop_int ng_ineqI,
                 const fatrop_int ng_ineq,
                 const fatrop_int ng_ineqF,
                 const fatrop_int n_stage_params,
                 const fatrop_int n_global_params,
                 const fatrop_int K,
                 const EvalCasGen &BAbtf,
                 const EvalCasGen &bkf,
                 const EvalCasGen &RSQrqtIf,
                 const EvalCasGen &rqIf,
                 const EvalCasGen &RSQrqtf,
                 const EvalCasGen &rqf,
                 const EvalCasGen &RSQrqtFf,
                 const EvalCasGen &rqFf,
                 const EvalCasGen &GgtIf,
                 const EvalCasGen &gIf,
                 const EvalCasGen &Ggtf,
                 const EvalCasGen &gf,
                 const EvalCasGen &GgtFf,
                 const EvalCasGen &gFf,
                 const EvalCasGen &Ggt_ineqIf,
                 const EvalCasGen &gineqIf,
                 const EvalCasGen &Ggt_ineqf,
                 const EvalCasGen &gineqf,
                 const EvalCasGen &Ggt_ineqFf,
                 const EvalCasGen &gineqFf,
                 const EvalCasGen &LkIf,
                 const EvalCasGen &Lkf,
                 const EvalCasGen &LFf,
                 const std::vector<double> &bounds_L,
                 const std::vector<double> &bounds_U,
                 const std::vector<double> &stage_params,
                 const std::vector<double> &global_params,
                 const std::vector<double> &initial_u,
                 const std::vector<double> &initial_x);
        fatrop_int get_default_stage_params(double *stage_params_res, const fatrop_int k) const override
        {
            fatrop_int offs = k * n_stage_params_;
            const double *stage_params_p = stage_params.data();
            for (fatrop_int i = 0; i < n_stage_params_; i++)
            {
                stage_params_res[i] = stage_params_p[offs + i];
            }
            return 0;
        }
        fatrop_int get_default_global_params(double *global_params_res) const override
        {
            const double *global_params_p = global_params.data();
            for (fatrop_int i = 0; i < n_global_params_; i++)
            {
                global_params_res[i] = global_params_p[i];
            }
            return 0;
        }
        fatrop_int eval_BAbt(const double *states_kp1,
                       const double *inputs_k,
                       const double *states_k,
                       const double *stage_params_k,
                       const double *global_params,
                       MAT *res,
                       const fatrop_int k) override;
        fatrop_int eval_RSQrqt(const double *objective_scale,
                         const double *inputs_k,
                         const double *states_k,
                         const double *lam_dyn_k,
                         const double *lam_eq_k,
                         const double *lam_ineq_k,
                         const double *stage_params_k,
                         const double *global_params,
                         MAT *res,
                         const fatrop_int k) override;
        fatrop_int eval_Ggt(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override;
        fatrop_int eval_Ggt_ineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            MAT *res,
            const fatrop_int k) override;
        fatrop_int eval_b(
            const double *states_kp1,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *constraint_violation_k,
            const fatrop_int k) override;
        fatrop_int eval_g(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override;
        fatrop_int eval_gineq(
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override;
        fatrop_int eval_rq(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override;

        fatrop_int eval_L(
            const double *objective_scale,
            const double *inputs_k,
            const double *states_k,
            const double *stage_params_k,
            const double *global_params,
            double *res,
            const fatrop_int k) override;
        fatrop_int get_initial_xk(double *xk, const fatrop_int k) const override
        {
            const double *initial_x_p = initial_x.data();
            for (fatrop_int i = 0; i < nx_; i++)
            {
                xk[i] = initial_x_p[i + k * nx_];
            }
            return 0;
        };
        fatrop_int get_initial_uk(double *uk, const fatrop_int k) const override
        {
            const double *initial_u_p = initial_u.data();
            for (fatrop_int i = 0; i < nu_; i++)
            {
                uk[i] = initial_u_p[i + k * nu_];
            }
            return 0;
        };
        fatrop_int set_initial_xk(double *xk, const fatrop_int k)
        {
            double *initial_x_p = initial_x.data();
            for (fatrop_int i = 0; i < nx_; i++)
            {
                initial_x_p[i + k * nx_] = xk[i];
            }
            return 0;
        };
        fatrop_int set_initial_uk(double *uk, const fatrop_int k)
        {
            double *initial_u_p = initial_u.data();
            for (fatrop_int i = 0; i < nu_; i++)
            {
                initial_u_p[i + k * nu_] = uk[i];
            }
            return 0;
        };
        fatrop_int get_bounds(double *lower, double *upper, const fatrop_int k) const override
        {
            const double *bounds_L_p = bounds_L.data();
            const double *bounds_U_p = bounds_U.data();
            fatrop_int offs = 0;
            fatrop_int ngineq = ng_ineq_;
            if (k == 0)
            {
                offs = 0;
            }
            else
            {
                offs = ng_ineqI_ + (k - 1) * ng_ineq_;
            }
            if (k == 0)
            {
                ngineq = ng_ineqI_;
            }
            else if (k == K_ - 1)
            {
                ngineq = ng_ineqF_;
            }

            for (fatrop_int i = 0; i < ngineq; i++)
            {
                lower[i] = bounds_L_p[i + offs];
                upper[i] = bounds_U_p[i + offs];
            }
            return 0;
        };


    private:
        EvalCasGen BAbtf;
        EvalCasGen bkf;
        EvalCasGen RSQrqtIf;
        EvalCasGen rqIf;
        EvalCasGen RSQrqtf;
        EvalCasGen rqf;
        EvalCasGen RSQrqtFf;
        EvalCasGen rqFf;
        EvalCasGen GgtIf;
        EvalCasGen gIf;
        EvalCasGen Ggtf;
        EvalCasGen gf;
        EvalCasGen GgtFf;
        EvalCasGen gFf;
        EvalCasGen Ggt_ineqIf;
        EvalCasGen g_ineqIf;
        EvalCasGen Ggt_ineqf;
        EvalCasGen g_ineqf;
        EvalCasGen Ggt_ineqFf;
        EvalCasGen g_ineqFf;
        EvalCasGen LkIf;
        EvalCasGen Lkf;
        EvalCasGen LFf;
        std::vector<double> initial_x;
        std::vector<double> initial_u;
        std::vector<double> bounds_L;
        std::vector<double> bounds_U;
        std::vector<double> stage_params;
        std::vector<double> global_params;
    };

    class StageOCPBuilder
    {
    public:
        static std::shared_ptr<StageOCP> FromRockitInterface(const std::shared_ptr<DLHandler> &handle, const json::jobject& json_spec)
        {

            // set up ocp
            const bool GN = false;
            // shared_ptr<DLHandler> handle = make_shared<DLHandler>(functions);
            // std::ifstream t(json_spec_file);
            // std::stringstream buffer;
            // buffer << t.rdbuf();
            // json::jobject json_spec = json::jobject::parse(buffer.str());
            const fatrop_int K = json_spec["K"];
            const fatrop_int nx = json_spec["nx"];
            const fatrop_int nu = json_spec["nu"];
            const fatrop_int ngI = json_spec["ngI"];
            const fatrop_int ng = json_spec["ng"];
            const fatrop_int ngF = json_spec["ngF"];
            const fatrop_int ng_ineqI = json_spec["ng_ineqI"];
            const fatrop_int ng_ineq = json_spec["ng_ineq"];
            const fatrop_int ng_ineqF = json_spec["ng_ineqF"];
            const fatrop_int no_stage_params = json_spec["n_stage_params"];
            const fatrop_int no_global_params = json_spec["n_global_params"];
            std::vector<double> lowerI = json_spec["lowerI"].get_number_array<double>("%lf");
            std::vector<double> upperI = json_spec["upperI"].get_number_array<double>("%lf");
            std::vector<double> lower = json_spec["lower"].get_number_array<double>("%lf");
            std::vector<double> upper = json_spec["upper"].get_number_array<double>("%lf");
            std::vector<double> lowerF = json_spec["lowerF"].get_number_array<double>("%lf");
            std::vector<double> upperF = json_spec["upperF"].get_number_array<double>("%lf");
            lower.insert(lower.begin(), lowerI.begin(), lowerI.end());
            upper.insert(upper.begin(), upperI.begin(), upperI.end());
            lower.insert(lower.end(), lowerF.begin(), lowerF.end());
            upper.insert(upper.end(), upperF.begin(), upperF.end());
            std::vector<double> initial_u = json_spec["initial_u"].get_number_array<double>("%lf");
            std::vector<double> initial_x = json_spec["initial_x"].get_number_array<double>("%lf");
            EvalCasGen BAbtf(handle, "BAbt");
            EvalCasGen bkf(handle, "bk");
            EvalCasGen RSQrqtIf = GN ? EvalCasGen(handle, "RSQrqtIGN") : EvalCasGen(handle, "RSQrqtI");
            EvalCasGen rqIf(handle, "rqI");
            EvalCasGen RSQrqtf = GN ? EvalCasGen(handle, "RSQrqtGN") : EvalCasGen(handle, "RSQrqt");
            EvalCasGen rqf(handle, "rqk");
            EvalCasGen RSQrqtFf = GN ? EvalCasGen(handle, "RSQrqtFGN") : EvalCasGen(handle, "RSQrqtF");
            EvalCasGen rqFf(handle, "rqF");
            EvalCasGen GgtIf(handle, "GgtI");
            EvalCasGen gIf(handle, "gI");
            EvalCasGen Ggtf(handle, "Ggt");
            EvalCasGen gf(handle, "g");
            EvalCasGen GgtFf(handle, "GgtF");
            EvalCasGen gFf(handle, "gF");
            EvalCasGen LIf(handle, "LI");
            EvalCasGen Lkf(handle, "Lk");
            EvalCasGen LFf(handle, "LF");
            EvalCasGen GgineqItf(handle, "GgineqIt");
            EvalCasGen gineqIf(handle, "gineqI");
            EvalCasGen Ggineqtf(handle, "Ggineqt");
            EvalCasGen gineqf(handle, "gineq");
            EvalCasGen GgineqFtf(handle, "GgineqFt");
            EvalCasGen gineqFf(handle, "gineqF");
            std::shared_ptr<StageOCP> stageocp = std::make_shared<StageOCPRockit>(nu, nx, ngI, ng, ngF, ng_ineqI, ng_ineq, ng_ineqF, no_stage_params, no_global_params, K,
                                                                  BAbtf,
                                                                  bkf,
                                                                  RSQrqtIf,
                                                                  rqIf,
                                                                  RSQrqtf,
                                                                  rqf,
                                                                  RSQrqtFf,
                                                                  rqFf,
                                                                  GgtIf,
                                                                  gIf,
                                                                  Ggtf,
                                                                  gf,
                                                                  GgtFf,
                                                                  gFf,
                                                                  GgineqItf,
                                                                  gineqIf,
                                                                  Ggineqtf,
                                                                  gineqf,
                                                                  GgineqFtf,
                                                                  gineqFf,
                                                                  LIf,
                                                                  Lkf,
                                                                  LFf, lower, upper,
                                                                  json_spec["stage_params"].get_number_array<double>("%lf"),
                                                                  json_spec["global_params"].get_number_array<double>("%lf"),
                                                                  initial_u, initial_x);
            return stageocp;
        }
    };
}
#endif // OCPTEMPLATEBASICINCLUDED

    // class SingleStageOCPAbstract
    // {
    // public:
    //     // problem dimensions
    //     virtual fatrop_int get_ng_initial() { return 0; };
    //     virtual fatrop_int get_ng_intermediate() { return 0; };
    //     virtual fatrop_int get_ng_terminal() { return 0; };
    //     virtual fatrop_int get_ng_ineq_initial() { return 0; };
    //     virtual fatrop_int get_ng_ineq_intermediate() { return 0; };
    //     virtual fatrop_int get_ng_ineq_terminal() { return 0; }
    //     virtual fatrop_int get_nxk(const fatrop_int k) const = 0;
    //     virtual fatrop_int get_nuk(const fatrop_int k) const = 0;
    //     virtual fatrop_int get_n_global_params() const = 0;
    //     virtual fatrop_int get_n_stage_params_k(const fatrop_int k) const = 0;
    //     // functions related to dynamics
    //     virtual fatrop_int eval_BAbt(const double *states_kp1,
    //                           const double *inputs_k,
    //                           const double *states_k,
    //                           const double *stage_params_k,
    //                           const double *global_params,
    //                           MAT *res) = 0;
    //     virtual fatrop_int eval_RSQrqt_initial(const double *objective_scale,
    //                                     const double *inputs_k,
    //                                     const double *states_k,
    //                                     const double *lam_dyn_k,
    //                                     const double *lam_eq_k,
    //                                     const double *lam_ineq_k,
    //                                     const double *stage_params_k,
    //                                     const double *global_params,
    //                                     MAT *res) = 0;
    //     virtual fatrop_int eval_RSQrqt_intermediate(const double *objective_scale,
    //                                          const double *inputs_k,
    //                                          const double *states_k,
    //                                          const double *lam_dyn_k,
    //                                          const double *lam_eq_k,
    //                                          const double *lam_ineq_k,
    //                                          const double *stage_params_k,
    //                                          const double *global_params,
    //                                          MAT *res) = 0;
    //     virtual fatrop_int eval_RSQrqt_terminal(const double *objective_scale,
    //                                      const double *inputs_k,
    //                                      const double *states_k,
    //                                      const double *lam_dyn_k,
    //                                      const double *lam_eq_k,
    //                                      const double *lam_ineq_k,
    //                                      const double *stage_params_k,
    //                                      const double *global_params,
    //                                      MAT *res) = 0;
    //     virtual fatrop_int eval_Ggtk_initial(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_Ggtk_intermediate(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_Ggtk_terminal(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_Ggt_ineq_initial(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_Ggt_ineq_intermediate(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_Ggt_ineq_terminal(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         MAT *res) { return 0; };
    //     virtual fatrop_int eval_b_initial(
    //         const double *states_kp1,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *constraint_violation_k) = 0;
    //     virtual fatrop_int eval_b_intermediate(
    //         const double *states_kp1,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *constraint_violation_k) = 0;
    //     virtual fatrop_int eval_b_terminal(
    //         const double *states_kp1,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *constraint_violation_k) = 0;
    //     virtual fatrop_int eval_g_initial(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_g_intermediate(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_g_terminal(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_gineq_initial(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_gineq_intermediate(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_gineq_terminal(
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) { return 0; };
    //     virtual fatrop_int eval_rq_initial(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    //     virtual fatrop_int eval_rq_intermediate(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    //     virtual fatrop_int eval_rq_terminal(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    //     virtual fatrop_int eval_L_initial(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    //     virtual fatrop_int eval_L_intermediate(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    //     virtual fatrop_int eval_L_terminal(
    //         const double *objective_scale,
    //         const double *inputs_k,
    //         const double *states_k,
    //         const double *stage_params_k,
    //         const double *global_params,
    //         double *res) = 0;
    // };