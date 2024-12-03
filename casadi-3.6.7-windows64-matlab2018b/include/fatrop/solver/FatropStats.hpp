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
#ifndef FATROPSTATSINCLUDED
#define FATROPSTATSINCLUDED
#include <iostream>
namespace fatrop
{
    struct FatropStats
    {
        double compute_sd_time = 0.0;
        double duinf_time = 0.0;
        double eval_hess_time = 0.0;
        double eval_jac_time = 0.0;
        double eval_cv_time = 0.0;
        double eval_grad_time = 0.0;
        double eval_obj_time = 0.0;
        double initialization_time = 0.0;
        double time_total = 0.0;
        int eval_hess_count = 0;
        int eval_jac_count = 0;
        int eval_cv_count = 0;
        int eval_grad_count = 0;
        int eval_obj_count = 0;
        int iterations_count = 0;
        int return_flag = 0;
        void print(std::ostream& stream)
        {
            stream << "---- stats ----" << std::endl;
            stream << "compute_sd:     " << compute_sd_time << " s"<< std::endl;
            stream << "duinf:          " << duinf_time << " s"<< std::endl;
            stream << "initialization: " << initialization_time << " s  count: "<< iterations_count <<std::endl;
            double time_FE = eval_hess_time + eval_jac_time + eval_cv_time + eval_grad_time + eval_obj_time ;
            stream << "time_FE :       " << time_FE << " s"<< std::endl;
            stream << "    eval hess:  " << eval_hess_time << " s  count: "<< eval_hess_count <<std::endl;
            stream << "    eval jac:   " << eval_jac_time << " s  count: "<< eval_jac_count <<std::endl;
            stream << "    eval cv:    " << eval_cv_time << " s  count: "<< eval_cv_count <<std::endl;
            stream << "    eval grad:  " << eval_grad_time << " s  count: "<< eval_grad_count <<std::endl;
            stream << "    eval obj:   " << eval_obj_time << " s  count: "<< eval_obj_count <<std::endl;
            stream << "rest  :       " <<time_total -  time_FE - initialization_time - duinf_time - compute_sd_time<< " s"<< std::endl;
            stream << "----- "<< std::endl;
            stream << "time_w/o_FE : " << time_total - time_FE << " s" << std::endl;
            stream << "time_FE :       " << time_FE << " s"<< std::endl;
            stream << "time_total : " << time_total << " s  iterations: " << iterations_count << std::endl;
        }
    };
} // namespace fatrop
#endif // FATROPSTATSINCLUDED