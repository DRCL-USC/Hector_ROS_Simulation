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
#ifndef FUNCTIONEVALUATIONINCLUDED
#define FUNCTIONEVALUATIONINCLUDED
#include <vector>
#include <cstring>
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "fatrop/auxiliary/Common.hpp"

#ifdef ENABLE_MULTITHREADING
#include <omp.h>
#endif

namespace fatrop
{
    /// Class used to evaluate a numerical functions. Functions can be implemented by hand or casadi codegen API or by plain casadi.
    class EvalBase
    {
    public:
        /// number of input vectors of the function
        fatrop_int n_in;
        /// number of columns in output matrix
        fatrop_int out_m;
        /// number of rows in output matrix
        fatrop_int out_n;
        /// number of nonzeros in output matrix
        fatrop_int out_nnz;
        /// sparsity pattern of output matrix sparsity pattern [m,n|0,ncol0, ncol0:1 , ..., | nnz | row_el0, row_el1, ...]
        std::vector<fatrop_int> sparsity_out;
        /// buffer to safe evaluation result, in a buffer we always save a matrix in CCS format with lda==out_m
        #ifndef ENABLE_MULTITHREADING
        std::vector<double> buffer;
        #else
        std::vector<std::vector<double>> buffer = std::vector<std::vector<double>>(omp_get_max_threads());
        #endif
        /// evaluate function and save res in "ccs format with lda==out_m"
        virtual fatrop_int eval_buffer(const double **arg) = 0;
        /// evaluate function and save res in "blasfeo format"
        fatrop_int eval_bf(const double **arg, MAT *bf_mat);
        fatrop_int eval_array(const double **arg, double *array);
        ~EvalBase(){};
    };
};     // namespace fatrop
#endif // FUNCTIONEVALUATIONINCLUDED
