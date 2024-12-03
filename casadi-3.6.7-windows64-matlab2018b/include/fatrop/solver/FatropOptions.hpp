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
#ifndef FATROPPARAMSINCLUDED
#define FATROPPARAMSINCLUDED
#include <string>
#include <map>
#include <type_traits>
#include <iostream>
#include <vector>
#include "fatrop/auxiliary/Common.hpp"
namespace fatrop
{
    template <typename T>
    struct Option
    {
    public:
        Option(){};
        Option(const std::string &name, const std::string &description, T *value, T default_value);
        void set(const T &new_value) const;
        void set_default() const;
        std::string name_;
        std::string description_;
        T *value = NULL;
        T default_value_;
    };


    template <typename T>
    struct NumberOption: public Option<T>
    {
    public:
        // DoubleOption operator=(const DoubleOption &other) = default;
        NumberOption(){};
        NumberOption(const std::string &name, const std::string &description, T *value, T default_value, bool lower_bound_inclusive, T lower_bound, bool upper_bound_inclusive, T upper_bound); 
        static NumberOption<T> lower_bounded(const std::string &name, const std::string &description, T *value, T default_value, T lower_bound);
        static NumberOption<T> upper_bounded(const std::string &name, const std::string &description, T *value, T default_value, T upper_bound);
        static NumberOption<T> un_bounded(const std::string &name, const std::string &description, T *value, T default_value);
        static NumberOption<T> box_bounded(const std::string &name, const std::string &description, T *value, T default_value, T lower_bound, T upper_bound);
        void set(const T &new_value) const;
        bool lower_bound_inclusive_;
        T lower_bound_;
        bool upper_bound_inclusive_;
        T upper_bound_;
    };
   // define Numeric option as Option<double>
    typedef NumberOption<double> DoubleOption;
    typedef NumberOption<fatrop_int> IntegerOption;
    typedef Option<std::string> StringOption;
    typedef Option<bool> BooleanOption;

    class FatropOptions
    {
    public:
        FatropOptions();
        // the following options are shared between different algorithm components:
        int maxiter = 1000; // TODO this value cannot be changed to a value larger than the one used for building the solver
        double kappa_d = 1e-5;
        template <typename T>
        void set(const std::string &option_name, T value) const;
        template <typename T>
        void prebuilt_set(const std::string &option_name, T value);

    public:
        bool has_option(const std::string &option_name) const;
        void register_option(const DoubleOption &option);
        void register_option(const IntegerOption &option);
        void register_option(const BooleanOption &option);
        void register_option(const StringOption &option);
        friend auto operator<<(std::ostream &os, const FatropOptions &m) -> std::ostream &;
        std::map<std::string, std::vector<DoubleOption>> numeric_options;
        std::map<std::string, std::vector<IntegerOption>> integer_options;
        std::map<std::string, std::vector<BooleanOption>> boolean_options;
        std::map<std::string, std::vector<StringOption>> string_options;
        std::map<std::string, std::string> prebuilt_string;
        std::map<std::string, double> prebuilt_double;
    };

} // namespace fatrop
#endif // FatropOptions