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
#ifndef FATROPITERATIONDATAINCLUDED
#define FATROPITERATIONDATAINCLUDED
#include <cmath>
#include <vector>
#include <iostream>
#include <fatrop/solver/FatropPrinter.hpp>
#include "fatrop/auxiliary/Common.hpp"
#include <memory>
namespace fatrop
{
    struct IterationData
    {
        fatrop_int iter = 0;
        double mu = 0.0;
        double objective = 0.0;
        double constraint_violation = 0.0;
        double du_inf = 0.0;
        fatrop_int ls = 0;
        double reg = 0.0;
        double alpha_pr = 0.0;
        double alpha_du = 0.0;
        char type = 'x';
        bool resto = false;
    };
    class Journaller
    {
    public:
        Journaller(const fatrop_int maxiter, const std::shared_ptr<FatropPrinter> &printer);
        void print_iterations(bool no_header = false);
        void push();
        void reset();
        fatrop_int print_count = 0;
        std::vector<IterationData> iterationdata;
        IterationData it_curr;
        std::shared_ptr<FatropPrinter> printer_;
    };
} // namespace fatrop
#endif //  FATROPITERATIONDATAINCLUDED