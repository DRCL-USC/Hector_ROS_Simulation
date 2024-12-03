#ifndef DAQP_TYPES_H
# define DAQP_TYPES_H

# ifdef __cplusplus
extern "C" {
# endif // ifdef __cplusplus

#ifdef DAQP_SINGLE_PRECISION
typedef float c_float;
#else
typedef double c_float;
#endif

typedef struct{

    // Data for the QP problem
    //
    // min  0.5 x'*H*x + f'x
    // s.t  lbA <= A*x <= ubA
    //      lb  <=  x  <= ub
    //
    // n  - dimension of x
    // m  - total number of constraints
    // ms - number of simple bounds
    // blower = [lb; lbA];
    // bupper = [ub; ubA];
    // (The number of rows in A is hence m-ms)

    // sense define the state of the constraints 
    // (active, immutable, upper/lower, soft, binary).

    int n;
    int m;
    int ms;

    c_float* H;
    c_float* f;

    c_float* A;
    c_float* bupper;
    c_float* blower;

    int* sense; 

}DAQPProblem;

typedef struct{
    c_float primal_tol;
    c_float dual_tol;
    c_float zero_tol;
    c_float pivot_tol;
    c_float progress_tol;

    int cycle_tol;
    int iter_limit;
    c_float fval_bound;

    c_float eps_prox;
    c_float eta_prox;

    c_float rho_soft;

    c_float rel_subopt;
    c_float abs_subopt;
}DAQPSettings;


typedef struct{
    int bin_id;
    int depth;
    int WS_start;
    int WS_end;
}DAQPNode;

typedef struct{
    int* bin_ids;
    int nb;
    int neq;

    DAQPNode* tree;
    int  n_nodes;

    int* tree_WS;
    int nWS;
    int n_clean;
    int* fixed_ids;

    int nodecount;
    int itercount;
}DAQPBnB;

typedef struct{
    DAQPProblem* qp;
    // LDP data 
    int n; // Number of primal variables
    int m; // Number of constraints  
    int ms; // Number of simple bounds
    c_float *M; // M' M is the Hessian of the dual objective function (dimensions: n x m)  
    c_float *dupper; // Linear part of dual objective function (dimensions: m x 1) 
    c_float *dlower; // Linear part of dual objective function (dimensions: m x 1) 
    c_float *Rinv; // Inverse of upper cholesky factor of primal Hessian 
    c_float *v; // v = R'\f (used to transform QP to LDP 
    int *sense; // State of constraints  
    c_float *scaling; // normalizations 
    c_float *RinvD; // in case Rinv is diagonal


    // Iterates
    c_float *x; // The final primal solution
    c_float *xold; // The latest primal solution (used for proximal-point iteratios)

    c_float* lam; // Dual iterate 
    c_float* lam_star; // Current constrained stationary point 
    c_float* u; // Stores Mk' lam_star
    c_float fval;

    // LDL factors (Mk Mk' = L D L')
    c_float *L;
    c_float *D;
    // Intermittent variables (LDL')
    c_float* xldl; // Solution to L xdldl = -dk
    c_float* zldl; // zldl_i = xldl_i/D_i
    int reuse_ind; // How much work that can be saved when solving Mk Mk' lam* = -dk

    int *WS; // Working set, size: maximum number of constraints (n+ns+1)
    int n_active; // Number of active contraints 

    int iterations;
    int sing_ind; // Flag for denoting whether Mk Mk' is singular or not 


    // Soft constraint
    c_float soft_slack;
#ifdef SOFT_WEIGHTS
    // The softened objective is given by
    //    min  0.5 x'*H*x + f'x + 0.5 su'su+0.5*sl'sl,
    // and the softened constraints are given by (similar for simple bounds)
    //    lbA-rho_ls*sl <= A*x <= ubA+rho_us*su,
    // with the bounds sl >= d_ls, su >= d_us
    // note that lbA/ubA is assumed to be shifted with rho_ls*d_ls and rho_us*d_us
    // since the slacks are assumed to be active at their bounds by default.

    // size of the following is m; values are only used if index set to SOFT.
    c_float *d_ls;
    c_float *d_us;
    c_float *rho_ls;
    c_float *rho_us;
#endif

    // Settings
    DAQPSettings* settings;

    // BnB
    DAQPBnB* bnb;
}DAQPWorkspace;

# ifdef __cplusplus
}
# endif // ifdef __cplusplus

#endif //ifndef DAQP_TYPES_H
