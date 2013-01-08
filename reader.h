#ifndef READER_H
#define READER_H
#ifndef SWIG
#include <inttypes.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SWIG
#include "enums.h"
#endif

//typedef struct sbrr_t* sbrr;
typedef struct sbrr_t *sbrr;


sbrr    sbrr_create();

int     sbrr_open(sbrr handle, const char *binary_report_file);

void    sbrr_destroy(sbrr handle);

int     sbrr_get_num_elements(sbrr handle,
                              ElementType type);

int     sbrr_get_num_periods(sbrr handle);

const char *
        sbrr_get_element_id(sbrr handle,
                            int index,
                            ElementType type);

int     sbrr_get_element_index(sbrr handle,
                               const char *id,
                               ElementType type);

double  sbrr_get_result_date(sbrr handle,
                             int period);

void    sbrr_get_element_results(sbrr handle,
                                 int period,
                                 int index,
                                 ElementType type,
                                 float **data,
                                 int *n);

#ifndef SWIG
void    sbrr_get_node_results(sbrr handle,
                              int index,
                              NodeResultType type,
                              float *data,
                              int n);

void    sbrr_get_link_results(sbrr handle,
                              int index,
                              LinkResultType type,
                              float *data,
                              int n);

#endif

#ifdef __cplusplus
}
#endif

#endif // READER_H
