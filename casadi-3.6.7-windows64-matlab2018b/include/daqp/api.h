#ifndef DAQP_API_H
# define DAQP_API_H

# ifdef __cplusplus
extern "C" {
# endif // ifdef __cplusplus

#include "daqp.h"
#include "daqp_prox.h"
#include "bnb.h"

typedef struct{
    c_float *x;
    c_float *lam;
    c_float fval;
    c_float soft_slack;

    int exitflag;
    int iter;
    int nodes;
    c_float solve_time;
    c_float setup_time;

}DAQPResult;

void daqp_solve(DAQPResult* res, DAQPWorkspace *work);
void daqp_quadprog(DAQPResult* res, DAQPProblem* qp,DAQPSettings* settings);

int setup_daqp(DAQPProblem *qp, DAQPWorkspace* work, c_float* setup_time);
int setup_daqp_ldp(DAQPWorkspace *work, DAQPProblem* qp);
int setup_daqp_bnb(DAQPWorkspace* work, int nb, int ns);
void allocate_daqp_settings(DAQPWorkspace *work);
void allocate_daqp_workspace(DAQPWorkspace *work, int n, int ns);

void free_daqp_ldp(DAQPWorkspace *work);
void free_daqp_workspace(DAQPWorkspace *work);
void free_daqp_bnb(DAQPWorkspace* work);

void daqp_extract_result(DAQPResult* res, DAQPWorkspace* work);
void daqp_default_settings(DAQPSettings *settings);

# ifdef __cplusplus
}
# endif // ifdef __cplusplus

#endif //ifndef DAQP_API_H
