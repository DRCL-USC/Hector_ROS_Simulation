#ifndef OCPLSSCALER_HPP
#define OCPLSSCALER_HPP
#include "OCPLSRiccati.hpp"
#include <vector>
#include <algorithm>
namespace fatrop
{
    class OCPLSScaler
    {
    public:
        OCPLSScaler(const OCPDims &dims) : K_(dims.K), ng_(dims.ng), nu_(dims.nu), nx_(dims.nx), ng_tot_(dims.n_g_tot), scales_(ng_tot_){};
        void scale_kkt(OCPKKTMemory &kkt)
        {
            MAT *Ggt_p = (MAT *)kkt.Ggt[0];
            int offs = 0;
            for (int k = 0; k < K_; k++)
            {
                for (int i = 0; i < ng_[k]; i++)
                {
                    double scale = 1.0/(std::max)(inf_col(nu_[k] + nx_[k], Ggt_p + k, 0, i), 1e-8); // 1e-8 is a small number to avoid division by zero
                    if (scale > 1e-3 && scale < 1e3) scale = 1.0; // if the scale is not very small or large, we do not scale
                    if(scale != 1.0) COLSC(nu_[k] + nx_[k] + 1,  scale, Ggt_p + k, 0, i);
                    VECEL((VEC*)scales_, offs) = scale;
                    offs++;
                }
            }
        }
        void scale_lam(const FatropVecBF &lam, const int ai)
        {
            VECMUL(ng_tot_, (VEC*)scales_, 0, (VEC*) lam, ai, (VEC*) lam, ai);
        }
        void restore_kkt(OCPKKTMemory &kkt)
        {
            MAT *Ggt_p = (MAT *)kkt.Ggt[0];
            int offs = 0;
            for (int k = 0; k < K_; k++)
            {
                for (int i = 0; i < ng_[k]; i++)
                {
                    double scale = VECEL((VEC*)scales_, offs);
                    if (scale != 1.0) COLSC(nu_[k] + nx_[k] + 1, 1.0/scale, Ggt_p + k, 0, i);
                    offs++;
                }
            }
        }

    private:
        static double inf_col(const int kmax, MAT *A, const int ai, const int aj)
        {
            double res = 0.0;
            for (int i = 0; i < kmax; i++)
            {
                res = (std::max)(res, std::abs(MATEL(A, ai + i, aj)));
            }
            return res;
        }
        const int K_;
        std::vector<int> ng_;
        std::vector<int> nu_;
        std::vector<int> nx_;
        const int ng_tot_;
        VECBF scales_;
    };

} // namespace fatrop
#endif