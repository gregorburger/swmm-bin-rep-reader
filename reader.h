#ifndef READER_H
#define READER_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SubCatchement,
    Node,
    Link,
    Pollutant
} ElementType;

//typedef struct sbrr_t* sbrr;
typedef struct sbrr_t *sbrr;

int     sbrr_create(sbrr *handle, const char *binary_report_file);

void    sbrr_destroy(sbrr  handle);

int     sbrr_get_num_elements(sbrr handle,
                              ElementType type);

int     sbrr_get_num_periods(sbrr handle);

char    *sbrr_get_element_id(sbrr handle,
                             int index ,
                             ElementType type);

double  sbrr_get_result_date(sbrr handle,
                             int period);

float   *sbrr_get_element_results(sbrr handle,
                                  int period,
                                  int index ,
                                  ElementType type);

#ifdef __cplusplus
}
#endif

#endif // READER_H
