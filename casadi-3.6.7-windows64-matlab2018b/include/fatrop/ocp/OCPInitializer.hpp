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
#ifndef OCPINITIALIZERINCLUDED
#define OCPINITIALIZERINCLUDED
#include "OCPKKT.hpp"
#include "fatrop/auxiliary/Common.hpp"
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#define OCPMACRO(type, name, suffix) type name##suffix = ((type)OCP->name)
#define AUXMACRO(type, name, suffix) type name##suffix = ((type)OCP->aux.name)
#define SOLVERMACRO(type, name, suffix) type name##suffix = ((type)name)
namespace fatrop
{
    class OCPInitializer
    {
    public:
        /** \brief this method adapts KKT system for initialization, JAC and GRAD are assumed evaluated !! */
        fatrop_int modify_kkt_ls_dual_estimate(
            OCPKKTMemory *OCP,
            const FatropVecBF &grad);
        fatrop_int intialize_slack_variables(
            OCPKKTMemory *OCP,
            FatropVecBF &s);
    };
} // namespace fatrop
#endif //  OCPINITIALIZERINCLUDED