#ifndef DAQP_FACTORIZATION_H
# define DAQP_FACTORIZATION_H

# ifdef __cplusplus
extern "C" {
# endif // ifdef __cplusplus

#include "types.h"
#include "constants.h"

void update_LDL_add(DAQPWorkspace *work, const int add_ind);
void update_LDL_remove(DAQPWorkspace *work, const int rm_ind);

# ifdef __cplusplus
}
# endif // ifdef __cplusplus

#endif //ifndef DAQP_FACTORIZATION_H
