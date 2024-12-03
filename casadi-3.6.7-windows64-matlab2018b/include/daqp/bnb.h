#ifndef DAQP_BNB_H 
# define DAQP_BNB_H 

# ifdef __cplusplus
extern "C" {
# endif // ifdef __cplusplus

#include "types.h"
#include "constants.h"
#include "daqp.h"

int daqp_bnb(DAQPWorkspace* work);
int process_node(DAQPNode* node, DAQPWorkspace* work);
int get_branch_id(DAQPWorkspace* work);
void spawn_children(DAQPNode* node, const int branch_id, DAQPWorkspace* work);

void node_cleanup_workspace(int n_clean, DAQPWorkspace* work);
void warmstart_node(DAQPNode* node, DAQPWorkspace* work); 
void save_warmstart(DAQPNode* node, DAQPWorkspace* work);
int add_upper_lower(const int add_id, DAQPWorkspace* work); 

#define LOWER_BIT 16
#define EXTRACT_LOWER_FLAG(x) (x>>(LOWER_BIT-1))
#define REMOVE_LOWER_FLAG(x) (x&~(1<<LOWER_BIT))
#define ADD_LOWER_FLAG(x) (x|(1<<LOWER_BIT))
#define TOGGLE_LOWER_FLAG(x) (x^(1<<LOWER_BIT))

# ifdef __cplusplus
}
# endif // ifdef __cplusplus

#endif //ifndef DAQP_BNB_H 
