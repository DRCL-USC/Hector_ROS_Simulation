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
#ifndef NOSCALINGMETHODINCLUDED
#define NOSCALINGMETHODINCLUDED
#include "fatrop/solver/FatropData.hpp"
#include "fatrop/templates/NLPAlg.hpp"
#include "fatrop/solver/AlgStrategy.hpp"
#include "OCPScalingMethod.hpp"
#include "fatrop/blasfeo_wrapper/LinearAlgebraBlasfeo.hpp"
#include <memory>
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    class OCPNoScaling : public OCPScalingMethod
    {
    public:
        OCPNoScaling(const std::shared_ptr<FatropOptions> &fatrop_params);
        virtual fatrop_int compute_scalings(
            OCPKKTMemory *OCP,
            double &obj_scale,
            FatropVecBF &x_scales,
            FatropVecBF &lam_scales, const FatropVecBF &grad_curr);
    };

} // namespace fatrop
#endif // !SCALINGMETHODINCLUDED