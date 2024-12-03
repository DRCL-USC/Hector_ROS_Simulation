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
#ifndef OCPLSRICCATIINCLUDED
#define OCPLSRICCATIINCLUDED
#include "OCPKKT.hpp"
#include "OCPLinearSolver.hpp"
#include "fatrop/solver/FatropPrinter.hpp"
#include <cmath>
#include <memory>
#include "fatrop/solver/FatropOptions.hpp"
#include "fatrop/auxiliary/Common.hpp"
#define SUMMATION_ALG kahan_sum
namespace fatrop
{
    bool check_reg(const fatrop_int m, MAT *sA, const fatrop_int ai, const fatrop_int aj);
    class OCPLSRiccati : public OCPLinearSolver
    {
    public:
        OCPLSRiccati(const OCPDims &dims, const std::shared_ptr<FatropOptions> &options, const std::shared_ptr<FatropPrinter> &printer);
            // solve a KKT system
            fatrop_int solve_pd_sys(
                OCPKKTMemory *OCP,
                const double inertia_correction_w,
                const double inertia_correction_c,
                const FatropVecBF &ux,
                const FatropVecBF &lam,
                const FatropVecBF &delta_s,
                const FatropVecBF &sigma_total,
                const FatropVecBF &gradb_total) override;
        // solve a KKT system
        fatrop_int solve_pd_sys_degenerate(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &gradb_total);
        // solve a KKT system
        int
        solve_pd_sys_normal(
            OCPKKTMemory *OCP,
            const double inertia_correction,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &gradb_total);
        fatrop_int get_rhs(
            OCPKKTMemory *OCP,
            const FatropVecBF &gradb_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb) override;
        fatrop_int compute_pd_sys_times_vec(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb) override;
        fatrop_int solve_rhs(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb) override;
        fatrop_int solve_rhs_normal(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb) ;
        fatrop_int solve_rhs_degenerate(
            OCPKKTMemory *OCP,
            const double inertia_correction_w,
            const double inertia_correction_c,
            const FatropVecBF &ux,
            const FatropVecBF &lam,
            const FatropVecBF &delta_s,
            const FatropVecBF &sigma_total,
            const FatropVecBF &rhs_rq,
            const FatropVecBF &rhs_b,
            const FatropVecBF &rhs_g,
            const FatropVecBF &rhs_g_ineq,
            const FatropVecBF &rhs_gradb);
        FatropMemoryMatBF Ppt;
        FatropMemoryMatBF Hh;
        FatropMemoryMatBF AL;
        FatropMemoryMatBF RSQrqt_tilde;
        FatropMemoryMatBF Ggt_stripe;
        FatropMemoryMatBF Ggt_tilde;
        FatropMemoryMatBF GgLt;
        FatropMemoryMatBF RSQrqt_hat;
        FatropMemoryMatBF Llt;
        FatropMemoryMatBF Llt_shift; // needed because feature not implemented yet
        FatropMemoryMatBF GgIt_tilde;
        FatropMemoryMatBF GgLIt;
        FatropMemoryMatBF HhIt;
        FatropMemoryMatBF PpIt_hat;
        FatropMemoryMatBF LlIt;
        FatropMemoryMatBF Ggt_ineq_temp;
        FatropMemoryVecBF rhs_rq;
        FatropMemoryVecBF rhs_b;
        FatropMemoryVecBF rhs_g;
        FatropMemoryVecBF rhs_g_ineq;
        FatropMemoryVecBF rhs_gradb;
        FatropMemoryVecBF rhs_rq2;
        FatropMemoryVecBF rhs_b2;
        FatropMemoryVecBF rhs_g2;
        FatropMemoryVecBF rhs_g_ineq2;
        FatropMemoryVecBF rhs_gradb2;
        FatropMemoryVecBF ux_test;
        FatropMemoryVecBF lam_test;
        FatropMemoryVecBF delta_s_test;
        FatropMemoryVecBF v_Ppt;
        FatropMemoryVecBF v_Hh;
        FatropMemoryVecBF v_AL;
        FatropMemoryVecBF v_RSQrqt_tilde;
        FatropMemoryVecBF v_Ggt_stripe;
        FatropMemoryVecBF v_Ggt_tilde;
        FatropMemoryVecBF v_GgLt;
        FatropMemoryVecBF v_RSQrqt_hat;
        FatropMemoryVecBF v_Llt;
        FatropMemoryVecBF v_Llt_shift;
        FatropMemoryVecBF v_GgIt_tilde;
        FatropMemoryVecBF v_GgLIt;
        FatropMemoryVecBF v_HhIt;
        FatropMemoryVecBF v_PpIt_hat;
        FatropMemoryVecBF v_LlIt;
        FatropMemoryVecBF v_Ggt_ineq_temp;
        MemoryPermMat Pl;
        MemoryPermMat Pr;
        MemoryPermMat PlI;
        MemoryPermMat PrI;
        FatropVector<fatrop_int> gamma;
        FatropVector<fatrop_int> rho;
        fatrop_int rankI = 0;
        // struct LastUsed
        // {
        //     fatrop_int rankI = 0;
        //     double inertia_correction_w = 0;
        //     double inertia_correction_c = 0;
        //     double kappa_d = 0;
        //     double mu = 0;
        // } lastused_;
        std::shared_ptr<FatropOptions> options_;
        std::shared_ptr<FatropPrinter> printer_;
        bool it_ref = true;
        bool perturbed_mode = false;
        double perturbed_mode_param = 1e-6;
        int min_it_ref = 0;
        int max_it_ref = 5;
        double it_ref_acc = 1e-8;
        double lu_fact_tol = 1e-5;
        bool diagnostic = false;
    };
};     // namespace
#endif // OCPLSRICCATIINCLUDED