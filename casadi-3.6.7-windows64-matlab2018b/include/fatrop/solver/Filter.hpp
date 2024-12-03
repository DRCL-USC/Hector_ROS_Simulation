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
#ifndef FILTERINCLUDED
#define FILTERINCLUDED
#include "vector"
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    struct FilterData
    {
        FilterData(){};
        FilterData(const fatrop_int iteration, const double phi, const double theta):iteration(iteration),phi(phi),theta(theta){};
        const fatrop_int iteration = 0;
        /** \brief barrier function value */
        const double phi = 0.0;
        /** \brief constraint violation value */
        const double theta = 0.0;
    };
    class Filter
    {
    public:
        Filter(const fatrop_int size);
        void augment(const FilterData &filterdata);
        /** \brief check if fdin0 is dominated by fdin1 */
        inline bool a_dominmates_b(const FilterData &fdin0, const FilterData &fdin1) const;
        bool is_acceptable(const FilterData &fdin) const;
        void reset();
        fatrop_int size() const
        {
            return filterdata_.size();
        }

    private:
        std::vector<FilterData> filterdata_;
    };
} // namespace fatrop

#endif // FILTERINCLUDED