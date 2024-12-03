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
#ifndef CASADICODEGENINCLUDED
#define CASADICODEGENINCLUDED
#include <vector>
#include <string>
#include <memory>
#include "FunctionEvaluation.hpp"
#include "fatrop/auxiliary/DynamicLib.hpp"

#ifdef ENABLE_MULTITHREADING
#include <omp.h>
#endif

/* Typedefs */
typedef long long int casadi_int;
typedef void (*signal_t)(void);
typedef casadi_int (*getint_t)(void);
typedef int (*work_t)(casadi_int *sz_arg, casadi_int *sz_res, casadi_int *sz_iw, casadi_int *sz_w);
typedef const casadi_int *(*sparsity_t)(casadi_int ind);
typedef int (*eval_t)(const double **arg, double **res, casadi_int *iw, double *w, int mem);
typedef int (*casadi_checkout_t)(void);
typedef void (*casadi_release_t)(int);
namespace fatrop
{
    class EvalCasGen : public EvalBase
    {
    public:
        EvalCasGen();
        /// constructor from file
        EvalCasGen(const std::shared_ptr<DLHandler> &handle, const std::string &function_name);
        EvalCasGen(
            const signal_t incref,
            const signal_t decref,
            const casadi_checkout_t checkout,
            const casadi_release_t release,
            const getint_t n_in_fcn,
            const getint_t n_out_fcn,
            const sparsity_t sp_in,
            const sparsity_t sp_out,
            const work_t work,
            const eval_t eval);
/// pointer to result_buffer
#ifndef ENABLE_MULTITHREADING
        // double *output_buffer_p;
#else
        // std::vector<double *> output_buffer_p = std::vector<double *>(omp_get_max_threads());
#endif
        /// pointer to casadi codegen evalutation function
        eval_t eval; // !! multhithreading of this function not yet supported
        /// casadi int work vector
        casadi_int *iw;
        /// casadi double work vector
        double *w;
        /// increase reference counter
        signal_t incref;
        /// decrease reference counter
        signal_t decref;
        /// input size
        int *input_size;
        /// release casadi memory
        casadi_release_t release;
        /// thread local mem id
        int mem;
        /// double work vector
#ifndef ENABLE_MULTITHREADING
        std::vector<double> work_vector_d;
        std::vector<double*> res_vec;
        std::vector<const double*> arg_vec;
        /// int work vector
        std::vector<casadi_int> work_vector_i;
#else
        std::vector<std::vector<double>> work_vector_d;
        std::vector<std::vector<double*>> res_vec;
        std::vector<std::vector<const double*>> arg_vec;
        std::vector<std::vector<casadi_int>> work_vector_i;
#endif
        /// evaluate function and save res in "ccs format with lda==out_m"
        int eval_buffer(const double **arg);
        /// for reference counting of handle pointer
        std::shared_ptr<DLHandler> handle;
        ~EvalCasGen();
    };
    // define a macro that instantiates an EvalCasGen object with a given name

} // fatrop

#endif // CASADICODEGENINCLUDED