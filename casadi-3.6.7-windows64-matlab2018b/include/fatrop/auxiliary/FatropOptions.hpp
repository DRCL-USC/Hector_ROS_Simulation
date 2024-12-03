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
#ifndef FATROPOPTIONSINCLUDED
#define FATROPOPTIONSINCLUDED
#include <string>
namespace fatrop
{
    class FatropOption
    {
    public:
        FatropOption(const std::string &name, const std::string &descr) :name_(name), description_(descr){

                                            };
        std::string name_;
        std::string description_;
    };
    class NumericOption : public FatropOption
    {
    public:
        NumericOption(const std::string &name, const std::string &descr, double default_value) : FatropOption(name, descr), value_(default_value), default_value_(default_value) {}
        virtual void set_value(double value)
        {
            value_ = value;
        }
        double GetValue()
        {
            return value_;
        }

    private:
        double value_;
        double default_value_;
    };
    class NumericOptionBounded : public NumericOption
    {
        // decorator for NumericOption that also checks bounds
    public:
        NumericOptionBounded(const std::string &name, const std::string &descr, double default_value, double lower_bound, double upper_bound) : NumericOption(name, descr, default_value), lower_bound_(lower_bound), upper_bound_(upper_bound){};
        virtual void set_value(double value)
        {
            if ((value > lower_bound_) && (value < upper_bound_))
            {
                NumericOption::set_value(value);
            }
        }

    private:
        double lower_bound_;
        double upper_bound_;
    };
} // namespace fatrop
#endif