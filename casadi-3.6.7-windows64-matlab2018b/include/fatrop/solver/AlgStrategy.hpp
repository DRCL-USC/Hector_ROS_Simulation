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
#ifndef ALGSTRATEGYINCLUDED
#define ALGSTRATEGYINCLUDED
#include "FatropOptions.hpp"
#include <memory>
namespace fatrop
{
    class AlgStrategy
    {
    public:
        AlgStrategy(const std::shared_ptr<FatropOptions> &fatrop_params) : fatrop_params_(fatrop_params){};
        std::shared_ptr<FatropOptions> fatrop_params_;
        void initialize() {};
    };
};
#endif // !ALGSTRATEGYINCLUDED