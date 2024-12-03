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
#ifndef DUINFEVALINCLUDED
#define DUINFEVALINCLUDED
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include "OCPKKT.hpp"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
#define OCPMACRO(type, name, suffix) type name##suffix = ((type)OCP->name)
    class DuInfEvaluator
    {
    public:
        fatrop_int evaluate(
            OCPKKTMemory *OCP,
            double obj_scale,
            const FatropVecBF &lam,
            const FatropVecBF &grad,
            FatropVecBF &du_inf);
    };
}
#endif //  DUINFEVALINCLUDED