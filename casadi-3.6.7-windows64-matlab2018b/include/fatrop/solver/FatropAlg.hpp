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
#ifndef FATROPALGINCLUDED
#define FATROPALGINCLUDED
#include "fatrop/templates/NLPAlg.hpp"
#include "FatropData.hpp"
#include "Filter.hpp"
#include "LineSearch.hpp"
#include <cmath>
#include "IterationData.hpp"
#include <memory>
#include "FatropStats.hpp"
#include <limits>
#include <fatrop/solver/FatropPrinter.hpp>
#include "fatrop/auxiliary/Common.hpp"
// #include "AlgorithmQuantities.hpp"
// #ifdef ENABLE_MULTITHREADING
// #include "fatrop/auxiliary/Worker.hpp"
// #endif

namespace fatrop
{
    // TODO: get rid of FatropApplication
    class FatropAlg 
    {
    public:
        FatropAlg(
            const std::shared_ptr<FatropNLP> &fatropnlp,
            const std::shared_ptr<FatropData> &fatropdata,
            const std::shared_ptr<FatropOptions> &fatropparams,
            const std::shared_ptr<Filter> &filter,
            const std::shared_ptr<LineSearch> &linesearch,
            const std::shared_ptr<Journaller> &journaller, 
            const std::shared_ptr<FatropPrinter> &printer, 
            const std::shared_ptr<FatropAlg>&orig_, const std::shared_ptr<FatropAlg>&resto_alg_, bool resto_problem);
        void initialize() ;
        void reset() ;
        void set_bounds(const std::vector<double> &lower, const std::vector<double> &upper) ;
        void set_initial(const std::vector<double> &initial) ;
        void get_solution(std::vector<double> &sol) ;
        fatrop_int optimize() ;
        fatrop_int eval_lag_hess();
        fatrop_int eval_constr_jac();
        fatrop_int eval_constr_viol_curr();
        fatrop_int eval_constr_viol_trial();
        fatrop_int eval_obj_grad_curr();
        double eval_objective_curr();
        double eval_objective_trial();
        fatrop_int eval_dual_infeasiblity();
        fatrop_int perform_initializiation_dual();
        fatrop_int solve_pd_sys(double inertia_correction_w, double inertia_correction_c, double mu);
        fatrop_int start_resto_alg(double mu, int iter);
        fatrop_int return_from_resto_alg(double mu);
        bool resto_stop_crit();
        std::shared_ptr<FatropNLP> fatropnlp_;
        std::shared_ptr<FatropData> fatropdata_;
        std::shared_ptr<FatropOptions> fatropoptions_;
        std::shared_ptr<Filter> filter_;
        std::shared_ptr<LineSearch> linesearch_;
        std::shared_ptr<Journaller> journaller_;
        std::shared_ptr<FatropPrinter> printer_;
        FatropStats get_stats() 
        {
            return stats;
        };

    public:
        double tol;
        double acceptable_tol;
        fatrop_int acceptable_iter;
        fatrop_int maxiter;
        void set_resto_alg(const std::shared_ptr<FatropAlg> &resto_alg)
        {
            resto_alg_ = resto_alg;
        };

    private:
        double lammax;
        double mu0;
        double kappa_eta;
        double kappa_mu;
        double theta_mu;
        double delta_w0;
        double delta_wmin;
        double kappa_wmin;
        double kappa_wplus;
        double kappa_wplusem;
        double delta_c_stripe;
        double kappa_c;
        double kappa_d;
        double theta_min;
        fatrop_int max_watchdog_steps;
        bool warm_start_init_point;
        bool recalc_y;
        double recalc_y_feas_tol;
        // bool first_try_watchdog;
        FatropStats stats;
        std::weak_ptr<FatropAlg> orig_;
        std::shared_ptr<FatropAlg> resto_alg_;
        bool resto_problem_ = false;
        fatrop_int start_iter_ = 0;
        fatrop_int iter_count_ = 0;
    };
} // namespace fatrop
#endif // FATROPALGINCLUDED