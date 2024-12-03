
    class BFGSUpdater
    {
    public:
        BFGSUpdater(const int m) : m(m), Bk_prev(m, m), tmp1(m, 2), tmp2(m, 2), vk(m), yk_tilde(m) {}
        int update(MAT *Bip1, VEC *si, VEC *yi)
        {
            MAT *Bk_prev_p = Bk_prev;
            VEC *vk_p = vk;
            VEC *yk_tilde_p = yk_tilde;

            // powell update formula
            // compute update
            double sts = DOT(m, si, 0, si, 0);
            double sty = DOT(m, si, 0, yi, 0);
            double yty = DOT(m, yi, 0, yi, 0);
            if (sts == 0.0 || first_time)
            {
                skips = 0;
                reset();
                GECP(m, m, Bk_prev_p, 0, 0, Bip1, 0, 0);
                return 0;
            }
            GEMV_N(m, m, 1.0, Bk_prev_p, 0, 0, si, 0, 0.0, vk_p, 0, vk_p, 0);
            double stv = DOT(m, si, 0, vk_p, 0);
            double beta = -1.0 / stv;
#define SKIPPING
#define ALT_RANK1
#ifdef SKIPPING
            double alpha_tilde = 1.0 / sty;
            AXPBY(m, 1.0, yi, 0, 0.0, vk_p, 0, yk_tilde_p, 0);
            if (sty <= 1e-8 * std::sqrt(yty) * std::sqrt(sts))
            {
                int ret = 0;
                if (skips >= 1)
                {
                    reset(1.0);
                    ret = 1;
                    // std::cout << "resetting Bk" << std::endl;
                }
                GECP(m, m, Bk_prev_p, 0, 0, Bip1, 0, 0);
                skips++;
                return ret;
            }
#else
            double theta = sty > 0.2 * stv ? 1.0 : (0.8 * stv) / (stv - sty);
            AXPBY(m, theta, yi, 0, 1.0 - theta, vk_p, 0, yk_tilde_p, 0);
            double sty_tilde = DOT(m, si, 0, yk_tilde_p, 0);
            double alpha_tilde = 1.0 / sty_tilde;
#endif
#ifdef ALT_RANK1

            COLIN(m, yk_tilde_p, 0, tmp1, 0, 0);
            VECSC(m, alpha_tilde, yk_tilde_p, 0);
            COLIN(m, yk_tilde_p, 0, tmp2, 0, 0);

            COLIN(m, vk_p, 0, tmp1, 0, 1);
            VECSC(m, beta, vk_p, 0);
            COLIN(m, vk_p, 0, tmp2, 0, 1);

            SYRK_LN(m, 2, 1.0, tmp1, 0, 0, tmp2, 0, 0, 1.0, Bk_prev_p, 0, 0, Bip1, 0, 0);
            TRTR_L(m, Bip1, 0, 0, Bip1, 0, 0);
#else
            GER(m, m, alpha_tilde, yk_tilde_p, 0, yk_tilde_p, 0, Bk_prev_p, 0, 0, Bip1, 0, 0);
            GER(m, m, beta, vk_p, 0, vk_p, 0, Bip1, 0, 0, Bip1, 0, 0);
#endif
            // save the previous Bk
            GECP(m, m, Bip1, 0, 0, Bk_prev_p, 0, 0);
            skips = 0;
            return 0;
        }
        void reset(double alpha = 1.0, bool reset_skips = true)
        {
            // std:: cout << "resetting Bk" << std::endl;
            MAT *Bk_prev_p = Bk_prev;
            // identity matrix for B0
            GESE(m, m, 0.0, Bk_prev_p, 0, 0);
            DIARE(m, alpha, Bk_prev_p, 0, 0);
        }
        const int m;
        MATBF Bk_prev;
        MATBF tmp1;
        MATBF tmp2;
        VECBF vk;
        VECBF yk_tilde;
        int skips = 0;
        bool first_time = true;
    };
    class OCPBFGSUpdater : public BFGSUpdater
    {
    public:
        OCPBFGSUpdater(int nu, int nx, int nxp1, int ng, int ng_ineq, bool first, bool last) : BFGSUpdater(nu + nx), nu(nu), nx(nx), nxp1(nxp1), ng(ng), ng_ineq(ng_ineq), first(first), last(last), BAt_prev(nu + nx, nxp1), Gt_prev(nu + nx, ng), Gt_ineq_prev(nu + nx, ng_ineq), ux_prev(nu + nx), grad_obj_prev(nu + nx), s(nu + nx), y(nu + nx) {reset();}
        int update(MAT *Bkp1, VEC *ux, int a_ux, VEC *grad_obj, int a_grad_obj, MAT *BAbt, VEC *lam_dyn, int a_lam_dyn, MAT *Ggt, VEC *lam_eq, int a_lam_eq, MAT *Ggt_ineq, VEC *lam_ineq, int a_lam_ineq)
        {
            VEC *ux_prev_p = ux_prev;
            VEC *grad_obj_prev_p = grad_obj_prev;
            VEC *s_p = s;
            VEC *y_p = y;
            MAT *BAt_prev_p = BAt_prev;
            MAT *Gt_prev_p = Gt_prev;
            MAT *Gt_ineq_prev_p = Gt_ineq_prev;

            // compute s
            AXPBY(nu + nx, 1.0, ux, a_ux, -1.0, ux_prev_p, 0, s_p, 0);

            // compute y
            AXPBY(nu + nx, 1.0, grad_obj, a_grad_obj, 0.0, y_p, 0, y_p, 0);
            if (!last)
            {
                // contribution from dynamics
                GEMV_N(nu + nx, nxp1, 1.0, BAbt, 0, 0, lam_dyn, a_lam_dyn, 1.0, y_p, 0, y_p, 0);
            }
            // contribution from equality constraints
            GEMV_N(nu + nx, ng, 1.0, Ggt, 0, 0, lam_eq, a_lam_eq, 1.0, y_p, 0, y_p, 0);
            // contribution from inequality constraints
            GEMV_N(nu + nx, ng_ineq, 1.0, Ggt_ineq, 0, 0, lam_ineq, a_lam_ineq, 1.0, y_p, 0, y_p, 0);
            // save in last row
            ROWIN(nu + nx, 1.0, y_p, 0, Bkp1, nu + nx, 0);
            if (!first_time)
            {
                AXPBY(nu + nx, -1.0, grad_obj_prev_p, 0, 1.0, y_p, 0, y_p, 0);
                if (!last)
                {
                    // contribution from dynamics
                    GEMV_N(nu + nx, nxp1, -1.0, BAt_prev_p, 0, 0, lam_dyn, a_lam_dyn, 1.0, y_p, 0, y_p, 0);
                }
                // contribution from equality constraints
                GEMV_N(nu + nx, ng, -1.0, Gt_prev_p, 0, 0, lam_eq, a_lam_eq, 1.0, y_p, 0, y_p, 0);
                // contribution from inequality constraints
                GEMV_N(nu + nx, ng_ineq, -1.0, Gt_ineq_prev_p, 0, 0, lam_ineq, a_lam_ineq, 1.0, y_p, 0, y_p, 0);
            }
            // call BFGS update
            int ret = BFGSUpdater::update(Bkp1, s_p, y_p);
            // save ux and grad_obj
            VECCP(nu + nx, ux, a_ux, ux_prev_p, 0);
            VECCP(nu + nx, grad_obj, a_grad_obj, grad_obj_prev_p, 0);
            // save BAt, Gt, Gt_ineq
            GECP(nu + nx, nxp1, BAbt, 0, 0, BAt_prev_p, 0, 0);
            GECP(nu + nx, ng, Ggt, 0, 0, Gt_prev_p, 0, 0);
            GECP(nu + nx, ng_ineq, Ggt_ineq, 0, 0, Gt_ineq_prev_p, 0, 0);
            first_time = false;
            // blasfeo_print_dmat(nu+nx+1, nu+nx, Bkp1, 0, 0);
            return ret;
        }
        void reset()
        {
            first_time = true;
            BFGSUpdater::reset();
        }
        const int nu, nx, nxp1, ng, ng_ineq;
        const bool first, last;
        MATBF BAt_prev;
        MATBF Gt_prev;
        MATBF Gt_ineq_prev;
        VECBF ux_prev;
        VECBF grad_obj_prev;
        VECBF s;
        VECBF y;
    };