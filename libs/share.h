#ifndef __SHARE_H_
#define __SHARE_H_

Voiture* create_shared_memory(Shared_data *data_shared, unsigned int total_cars);
void init_semaphore(Shared_data *data_shared);
void detach_shared_memory(Voiture *shared_memory ,int segment_id);
void destroy_semaphore(Shared_data *data_shared);
#endif