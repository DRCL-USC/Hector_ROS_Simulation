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
#ifndef FATROP_BLASFEO_INCLUDED
#define FATROP_BLASFEO_INCLUDED

// macros
extern "C"
{
    void blasfeo_ref_drowpe(int kmax, int *ipiv, struct blasfeo_dmat *sA);
    void blasfeo_ref_drowpei(int kmax, int *ipiv, struct blasfeo_dmat *sA);
}
#define MAT blasfeo_dmat
#define VEC blasfeo_dvec
#define MEMSIZE_MAT blasfeo_memsize_dmat
#define CREATE_MAT blasfeo_create_dmat
// #define ROWPE blasfeo_drowpe
#define ROWPE blasfeo_ref_drowpe
#define VECPE blasfeo_dvecpe
#define VECPEI blasfeo_dvecpei
// #define ROWPEI blasfeo_drowpei
#define ROWPEI blasfeo_ref_drowpei
#define COLPE blasfeo_dcolpe
#define COLPEI blasfeo_dcolpei
#define CREATE_MAT blasfeo_create_dmat
#define MATEL BLASFEO_DMATEL
#define ROWSW blasfeo_drowsw
#define COLSW blasfeo_dcolsw
#define GEAD blasfeo_dgead
#define GECP blasfeo_dgecp
#define GESC blasfeo_dgesc
#define VECCP blasfeo_dveccp
#define VECCPSC blasfeo_dveccpsc
// #define TRSM_LLNN blasfeo_dtrsm_llnn
#define TRSM_RLTN blasfeo_dtrsm_rltn
#define VECEL BLASFEO_DVECEL
#define MEMSIZE_VEC blasfeo_memsize_dvec
#define CREATE_VEC blasfeo_create_dvec
#define GEMM_NT blasfeo_dgemm_nt
#define GEAD blasfeo_dgead
#define SYRK_LN_MN blasfeo_dsyrk_ln_mn
#define SYRK_LN blasfeo_dsyrk_ln
#define GETR blasfeo_dgetr
#define TRTR_L blasfeo_dtrtr_l
#define POTRF_L_MN blasfeo_dpotrf_l_mn
#define ROWEX blasfeo_drowex
#define ROWIN blasfeo_drowin
#define COLIN blasfeo_dcolin
#define ROWAD fatrop_drowad
#define TRSV_LTN blasfeo_dtrsv_ltn
#define TRSV_LNN blasfeo_dtrsv_lnn
#define TRSV_UTN blasfeo_dtrsv_utn
#define GEMV_T blasfeo_dgemv_t
#define GEMV_N blasfeo_dgemv_n
#define VECSE blasfeo_dvecse
#define VECSC blasfeo_dvecsc
#define PACKMAT blasfeo_pack_dmat
#define UNPACKVEC blasfeo_unpack_dvec
#define PACKVEC blasfeo_pack_dvec
#define PMAT PermMat
#define AXPY blasfeo_daxpy
#define AXPBY blasfeo_daxpby
#define DOT blasfeo_ddot
#define GESE blasfeo_dgese
#define DIARE blasfeo_ddiare
#define COLSC blasfeo_dcolsc
#define VECMUL blasfeo_dvecmul
#define VECMULACC blasfeo_dvecmulacc
#define GER blasfeo_dger

// functions not implemented by blasfeo_hp
#define GEADTR fatrop_dgead_transposed
#define VECCPR fatrop_dveccp_reversed
#define ROWAD fatrop_drowad
#define TRSV_UNU fatrop_dtrsv_unu
#define TRSV_UTU fatrop_dtrsv_utu

#if defined(BLASFEO_REF_API)
#define TRSM_RLNN blasfeo_dtrsm_rlnn
#else
void blasfeo_ref_dtrsm_rlnn_copy(int m, int n, double alpha, struct MAT *sA, int ai, int aj, struct MAT *sB, int bi, int bj, struct MAT *sD, int di, int dj);
#define TRSM_RLNN blasfeo_ref_dtrsm_rlnn_copy
#endif

#ifndef __GNUC__

#define MAX(a, b) max(a, b)
#define MIN(a, b) min(a, b)

#else

#define MAX(a, b)                   \
    (                               \
        {                           \
            __typeof__(a) _a = (a); \
            __typeof__(b) _b = (b); \
            _a > _b ? _a : _b;      \
        })
#define MIN(a, b)                   \
    (                               \
        {                           \
            __typeof__(a) _a = (a); \
            __typeof__(b) _b = (b); \
            _a < _b ? _a : _b;      \
        })

#endif

#include <iostream>
extern "C"
{
#include <blasfeo.h>
}
#include "fatrop/auxiliary/LinearAlgebra.hpp"
#include "fatrop/auxiliary/FatropVector.hpp"
#include "fatrop/auxiliary/Common.hpp"
#include <cmath>
#if DEBUG
#include <assert.h>
#endif
namespace fatrop
{
    void fatrop_dcolsc(fatrop_int kmax, double alpha, struct blasfeo_dmat *sA, fatrop_int ai, fatrop_int aj);
    // copy elements from sx to sy but in reversed order to avoid aliasing issues in recursion
    void fatrop_dveccp_reversed(fatrop_int m, struct blasfeo_dvec *sx, fatrop_int xi, struct blasfeo_dvec *sy, fatrop_int yi);
    // for debugging purposes
    // void fatrop_potrf_l_mn(fatrop_int m, fatrop_int n, struct blasfeo_dmat *sC, fatrop_int ci, fatrop_int cj, struct blasfeo_dmat *sD, fatrop_int di, fatrop_int dj);
    void test();
    /** \brief D <= alpha * B * A^{-1} , with A lower triangular employing explicit inverse of diagonal, fatrop uses its own (naive) implementation since it  not implemented yet in blasfeo */
    void fatrop_dtrsm_rlnn(fatrop_int m, fatrop_int n, double alpha, MAT *sA, fatrop_int offs_ai, fatrop_int offs_aj, MAT *sB, fatrop_int offs_bi, fatrop_int offs_bj, MAT *sD, fatrop_int offs_di, fatrop_int offs_dj);
    /** \brief D <= alpha * B * A^{-1} , with A lower triangular employing explicit inverse of diagonal, fatrop uses its own (naive) implementation since it  not implemented yet in blasfeo */
    void fatrop_dtrsm_rlnn_alt(fatrop_int m, fatrop_int n, double alpha, MAT *sA, fatrop_int offs_ai, fatrop_int offs_aj, MAT *sB, fatrop_int offs_bi, fatrop_int offs_bj, MAT *sD, fatrop_int offs_di, fatrop_int offs_dj);
    /** \brief B <= B + alpha*A^T (B is mxn) */
    void fatrop_dgead_transposed(fatrop_int m, fatrop_int n, double alpha, struct blasfeo_dmat *sA, fatrop_int offs_ai, fatrop_int offs_aj, struct blasfeo_dmat *sB, fatrop_int offs_bi, fatrop_int offs_bj);
    void fatrop_identity(const fatrop_int m, MAT *sA, const fatrop_int ai, const fatrop_int aj);
    void fatrop_drowad(fatrop_int kmax, double alpha, struct blasfeo_dvec *sx, fatrop_int xi, struct blasfeo_dmat *sA, fatrop_int ai, fatrop_int aj);
    /** \brief this class is used for blasfeo matrices*/
    class FatropMatBF : public FatropMat
    {
    public:
        /** \brief constructor memory still has to be allocated*/
        FatropMatBF(const fatrop_int nrows, const fatrop_int ncols, const fatrop_int row_offset, const fatrop_int col_offset);
        /** \brief constructor memory already allocated*/
        FatropMatBF(const fatrop_int nrows, const fatrop_int ncols, const fatrop_int row_offset, const fatrop_int col_offset, MAT *matbf);
        /** \brief constructor memory already allocated*/
        FatropMatBF(MAT *matbf);
        /** \brief type conversion to blasfeo matrix pointer*/
        inline explicit operator MAT *() const
        {
            return this->mat_;
        }
        /** \brief acces to element of matrix */
        inline double &at(const fatrop_int ai, const fatrop_int aj) const
        {
#if DEBUG
            assert(ai < nrows_);
            assert(aj < ncols_);
#endif
            return MATEL(mat_, ai + row_offset_, aj + col_offset_);
        };
        /** \brief get element of matrix */
        inline double get_el(const fatrop_int ai, const fatrop_int aj) const { return this->at(ai, aj); };
        /** \brief get number of rows */
        inline fatrop_int nrows() const { return nrows_; };
        /** \brief get number of cols */
        inline fatrop_int ncols() const { return ncols_; };
        /** \brief copies all elements from a given fatrop_matrix to this matrix*/
        void operator=(const FatropMat &fm);
        /** \brief set data pointer*/
        void set_datap(MAT *matbf)
        {
            mat_ = matbf;
        }
        /** \brief take a block of size (p,q), starting at (i,j)*/
        FatropMatBF block(const fatrop_int i, const fatrop_int j, const fatrop_int p, const fatrop_int q) const
        {
            return FatropMatBF(p, q, row_offset_ + i, col_offset_ + j, this->mat_);
        }

    private:
        MAT *mat_ = NULL;
        const fatrop_int row_offset_;
        const fatrop_int col_offset_;
        const fatrop_int nrows_;
        const fatrop_int ncols_;
    };

    class MATBF
    {
    public:
        MATBF(const int m, const int n) : m_(m), n_(n)
        {
            blasfeo_allocate_dmat(m_, n_, &mat_);
        }
        MATBF(MATBF &&other) : m_(other.m_), n_(other.n_)
        {
            mat_ = other.mat_;
            other.mat_.pA = nullptr;
            other.mat_.mem = nullptr;
            other.mat_.dA = nullptr;
        }
        ~MATBF()
        {
            blasfeo_free_dmat(&mat_);
        }
        operator MAT *()
        {
            return &mat_;
        }
        MAT mat_;
        const int m_;
        const int n_;
    };

    class VECBF
    {
    public:
        VECBF(const int m) : m_(m)
        {
            blasfeo_allocate_dvec(m_, &vec_);
            // zero out vector
            blasfeo_dvecse(m_, 0.0, &vec_, 0);
        }
        VECBF(VECBF &&other) : m_(other.m_)
        {
            vec_ = other.vec_;
            other.vec_.pa = nullptr;
            other.vec_.mem = nullptr;
        }
        ~VECBF()
        {
            blasfeo_free_dvec(&vec_);
        }
        operator VEC *()
        {
            return &vec_;
        }
        bool has_inf()
        {
            for (int i = 0; i < m_; i++)
            {
                if (std::isinf(VECEL(&vec_, i)))
                {
                    return true;
                }
            }
            return false;
        }
        VEC vec_;
        const int m_;
    };

    /** \brief this class is used for the allocation of a blasfeo matrix, the dimsensions are set from a vector */
    class FatropMemoryMatBF
    {
    public:
        /** \brief constuction for allocation on fatrop_memory_allocator*/
        FatropMemoryMatBF(const FatropVector<fatrop_int> &nrows, const FatropVector<fatrop_int> &ncols, fatrop_int N);
        // TODO: if rvalue-reference is used -> unecessary copy, use move sementics instead.;
        FatropMemoryMatBF(const fatrop_int nrows, const fatrop_int ncols, fatrop_int N);
        /** \brief calculate memory size*/
        fatrop_int memory_size() const;
        /** \brief set up memory element and advance pointer */
        void set_up();
        /** \brief get fatrop matrix bf */
        FatropMatBF operator[](const fatrop_int N) const;
        /** \brief get first blasfeo_xmat* struct */
        explicit operator MAT *() const
        {
            return mat;
        }
        FatropMemoryMatBF(const FatropMemoryMatBF &cpy) = delete;
        FatropMemoryMatBF &operator=(const FatropMemoryMatBF &) = delete;
        ~FatropMemoryMatBF();

    private:
        void *mem = NULL;
        MAT *mat;
        const fatrop_int N_;
        const FatropVector<fatrop_int> nrows_;
        const FatropVector<fatrop_int> ncols_;
    };
    /** this class is used for blasfeo vectors*/
    class FatropVecBF : public FatropVec
    {
    public:
        /** \brief constructor memory still has to be allocated*/
        FatropVecBF(const fatrop_int nels, const fatrop_int offset);
        /** \brief constructor memory already allocated*/
        FatropVecBF(const fatrop_int nels, const fatrop_int offset, VEC *vecbf);
        /** \brief type conversion to blasfeo vector pointer*/
        explicit operator VEC *() const;
        /** \brief access to element of matrix */
        inline double &at(const fatrop_int ai) const
        {
#if DEBUG
            assert(ai < nels_);
#endif
            return VECEL(vec_, ai + offset_);
        };
        /** \brief get element of vector */
        double get_el(const fatrop_int ai) const;
        /** \brief get number of elements */
        fatrop_int nels() const;
        /** \brief get offset */
        fatrop_int offset() const;
        /** \brief copies all elements from a given fatrop_vector to this vector*/
        void operator=(const FatropVec &fm);
        void operator=(const double &val)
        {
            blasfeo_dvecse(nels(), val, vec_, offset());
        }

        void copy(const FatropVecBF &fm) const;
        void copyto(std::vector<double> &dest) const;
        void operator=(const std::vector<double> &fm);
        /** \brief set data pointer*/
        void set_datap(VEC *vecbf);
        /** \brief take a block of size (p), starting at (i)*/
        FatropVecBF block(const fatrop_int i, const fatrop_int p) const;
        void SwapWith(FatropVecBF &vb);
        void SetConstant(double constant) const;
        friend double sum(const FatropVecBF &va)
        {
            double ret = 0.0;
            for (int i = 0; i < va.nels(); i++)
            {
                ret += va.at(i);
            }
            return ret;
        }
        bool has_inf() const
        {
            for (int i = 0; i < nels(); i++)
            {
                if (std::isinf(VECEL(vec_, i)))
                {
                    return true;
                }
            }
            return false;
        }
        bool has_nan() const
        {
            for (int i = 0; i < nels(); i++)
            {
                if (std::isnan(VECEL(vec_, i)))
                {
                    return true;
                }
            }
            return false;
        }

    protected:
        VEC *vec_ = NULL;
        const fatrop_int offset_;
        const fatrop_int nels_;
    };

    void axpy(const double alpha, const FatropVecBF &va, const FatropVecBF &vb, const FatropVecBF &vc);
    void copy(const FatropVecBF &va, const FatropVecBF &vb);
    void axpby(const double alpha, const FatropVecBF &va, const double beta, const FatropVecBF &vb, const FatropVecBF &vc);
    double dot(const FatropVecBF &va, FatropVecBF &vb);
    double Linf(const FatropVecBF &va);
    double LinfScaled(const FatropVecBF &va, const FatropVecBF &scales);
    double minabs(const FatropVecBF &va);
    double L1(const FatropVecBF &va);
    double sumsqr(const FatropVecBF &va);

    /** \brief this class is used for the allocation of a blasfeo vector, the dimsensions are set from a vector */
    class FatropMemoryVecBF
    {
    public:
        /** \brief constuction for allocation on MemoryAllocator*/
        FatropMemoryVecBF(const FatropVector<fatrop_int> &nels, fatrop_int N);
        // TODO: if rvalue-reference is used -> unecessary copy, use move sementics instead.;
        FatropMemoryVecBF(const fatrop_int nels, fatrop_int N = 1);
        /** \brief calculate memory size*/
        fatrop_int memory_size() const;
        /** \brief set up memory element and advance pointer */
        void set_up();
        /** \brief get fatrop matrix bf */
        FatropVecBF operator[](const fatrop_int N) const;
        /** \brief get first blasfeo_xmat* struct */
        explicit operator VEC *() const
        {
            return vec;
        }
        FatropMemoryVecBF(const FatropMemoryVecBF &cpy) = delete;
        FatropMemoryVecBF &operator=(const FatropMemoryVecBF &) = delete;
        ~FatropMemoryVecBF();

    private:
        void *mem = NULL;
        VEC *vec;
        const fatrop_int N_;
        const FatropVector<fatrop_int> nels_;
    };

    /** \brief this class represents a permutation matrix */
    class PermMat : public FatropMat
    {
    public:
        /** \brief constructor memory still has to be allocated */
        PermMat(const fatrop_int dim);
        ;
        /** \brief constructor memory already allocated */
        PermMat(const fatrop_int dim, fatrop_int *data);
        ;
        /** \brief get number of rows */
        fatrop_int nrows() const { return dim_; };
        /** \brief get number of columns */
        fatrop_int ncols() const { return dim_; };
        /** \brief get element of matrix represented by this permutation matrix - only used for debugging and testing purposes */
        double get_el(const fatrop_int ai, const fatrop_int aj) const;
        void print(const fatrop_int kmax) const;
        /** \brief set data pointer*/
        void set_datap(fatrop_int *data);
        /** \brief set data point*/
        void set_datap(const fatrop_int i, const fatrop_int val);
        /** \brief apply row permutation*/
        void PM(const fatrop_int kmax, MAT *M) const;
        /** \brief apply vec permutation*/
        void PV(const fatrop_int kmax, VEC *V, const fatrop_int offs) const;
        /** \brief apply vec permutation*/
        void PtV(const fatrop_int kmax, VEC *V, const fatrop_int offs) const;
        /** \brief apply row permutation on partial matrix*/
        void PM(const fatrop_int kmax, const fatrop_int n, MAT *M, const fatrop_int ai, const fatrop_int aj) const;
        /** \brief apply inverse row permutation*/
        void
        PtM(const fatrop_int kmax, MAT *M) const;
        /** \brief apply inverse col permutation*/
        void MP(const fatrop_int kmax, MAT *M) const;
        /** \brief apply col permutation*/
        void MPt(const fatrop_int kmax, MAT *M) const;
        /** fatrop_int pointer of permutation vector */
        explicit operator fatrop_int *() { return data_; };

        // private:
        const fatrop_int dim_;
        fatrop_int *data_ = NULL;
    };

    /** \brief this class is used for the allocation of a permutation matrix */
    class MemoryPermMat : public PermMat
    {
    public:
        /** \brief constructor */
        MemoryPermMat(const fatrop_int dim, const fatrop_int N);
        /** \brief calculate needed memory size*/
        fatrop_int memory_size() const;
        /** \brief set up memory*/
        void set_up();
        explicit operator PermMat *()
        {
            return perm_p;
        };
        MemoryPermMat(const MemoryPermMat &cpy) = delete;
        MemoryPermMat &operator=(const MemoryPermMat &) = delete;
        ~MemoryPermMat();

    private:
        void *mem = NULL;
        const fatrop_int dim_;
        const fatrop_int N_;
        PermMat *perm_p;
    };
    MatrixInd max_el(fatrop_int m, fatrop_int n, MAT *matr, fatrop_int ai, fatrop_int aj);

    /** \brief Function to calculate LU factorization result is saved in A, L is lower unitriangular */
    void LU_FACT(const fatrop_int m, const fatrop_int n, const fatrop_int n_max, fatrop_int &rank, MAT *A, PMAT *Pl_p, PMAT *Pr_p, double tol = 1e-8);
    /** \brief Function to calculate LU factorization but A, and result (L and U) are transposed, all indices refer to the dimensions of the original A matrix (and not the transposed one) */
    void LU_FACT_transposed(const fatrop_int m, const fatrop_int n, const fatrop_int n_max, fatrop_int &rank, MAT *At, PMAT *Pl_p, PMAT *Pr_p, double tol = 1e-5);
    void fatrop_dtrsv_unu(const fatrop_int m, const fatrop_int n, blasfeo_dmat *sA, const fatrop_int ai, const fatrop_int aj, blasfeo_dvec *sx, const fatrop_int xi, blasfeo_dvec *sz, const fatrop_int zi);
    void fatrop_dtrsv_utu(const fatrop_int m, blasfeo_dmat *sA, const fatrop_int ai, const fatrop_int aj, blasfeo_dvec *sx, const fatrop_int xi, blasfeo_dvec *sz, const fatrop_int zi);
} // namespace fatrop
#endif // FATROP_BLASFEO_INCLUDED