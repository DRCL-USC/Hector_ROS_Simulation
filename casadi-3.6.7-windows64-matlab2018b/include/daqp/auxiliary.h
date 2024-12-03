#ifndef DAQP_AUX_H 
# define DAQP_AUX_H

# ifdef __cplusplus
extern "C" {
# endif // ifdef __cplusplus

#include "types.h"
#include "constants.h"

void remove_constraint(DAQPWorkspace* work, const int rm_ind);
void add_constraint(DAQPWorkspace *work, const int add_ind, c_float lam);
void compute_primal_and_fval(DAQPWorkspace *work);
int add_infeasible(DAQPWorkspace *work);
int remove_blocking(DAQPWorkspace *work);
void compute_CSP(DAQPWorkspace *work);
void compute_singular_direction(DAQPWorkspace *work);

void reorder_LDL(DAQPWorkspace *work);
void pivot_last(DAQPWorkspace *work);

int activate_constraints(DAQPWorkspace *work);
void deactivate_constraints(DAQPWorkspace *work);

# ifdef __cplusplus
}
# endif // ifdef __cplusplus

#endif //ifndef DAQP_AUX_H
