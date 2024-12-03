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
#define PRIORITY1 PrintPriority<1>()
#ifndef FATROPPRINTERINCLUDED
#define FATROPPRINTERINCLUDED
#include <iostream>
#include <string>
#include <vector>
namespace fatrop
{
    static std::ostream nullstream(nullptr);
    template <int Priority>
    struct PrintPriority
    {
        PrintPriority(){};
        const int priority = Priority;
    };
    template <int priority>
    std::ostream &operator<<(std::ostream &os, const PrintPriority<priority> &p)
    {
        if (p.priority >= -1)
            return os;
        return nullstream;
    }
    class FatropPrinter
    {
    public:
        FatropPrinter(const int priority = 0, std::ostream &stream = std::cout) : priority_(priority), stream_(stream),
            printf_buffer_(256) {};
        std::ostream &
        level(const int p)
        {
            if (p <= priority_)
                return stream_;
            return nullstream;
        }
        int &print_level() { return priority_; }


        int printf(const char *fmt, ...);

    private:
        int priority_;
        std::ostream& stream_;
        std::vector<char> printf_buffer_;
    };

} // namespace fatrop
#endif //  FATROPITERATIONDATAINCLUDED