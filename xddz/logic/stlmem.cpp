
#include "stlmem.h"


memNode* my_mempool::m_free_head[32] = {0};

thread_mutex my_mempool::m_tmutex;
