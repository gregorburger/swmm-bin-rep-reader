#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <cstring>
#include <string>

typedef struct {
    int32_t one;
    int32_t input_area_magic;
    float areas[4096];
} __attribute__((packed)) swmm_catch_area_t;

#if 0
size_t read_catchement_areas(char *map, size_t offset, int num_c) {
    swmm_catch_area_t *area = (swmm_catch_area_t*) (map + offset);
    assert(area->one == 1);
    assert(area->input_area_magic == INPUT_AREA);
    assert(num_c  < 4096);
    for (int i = 0; i < num_c+1; i++) {
        printf("catch[%d].area = %f\n", i, area->areas[i]);
    }
    return 2*sizeof(int32_t) + num_c*sizeof(float);
}
#endif

typedef struct {
    int32_t node_type;
    float invert_elevation;
    float full_depth;
} __attribute__((packed)) swmm_node_info_values_t;

typedef struct {
    int32_t header[4];
    swmm_node_info_values_t values[4096];
} __attribute__((packed)) swmm_node_info_t;

#if 0
size_t read_node_info(char *map, size_t offset, int num_nodes) {
    swmm_node_info_t *node_type = (swmm_node_info_t*) (map+offset);
    assert(node_type->header[0] == 3);
    assert(node_type->header[1] == INPUT_TYPE_CODE);
    assert(node_type->header[2] == INPUT_INVERT);
    assert(node_type->header[3] == INPUT_MAX_DEPTH);

    for (int i = 0; i < num_nodes; ++i) {
        printf("node[%d].type = %d\n", i, node_type->values[i].node_type);
        printf("node[%d].depth = %f\n", i, node_type->values[i].full_depth);
    }

    return 4*sizeof(int32_t) + num_nodes*sizeof(swmm_node_info_values_t);
}
#endif

typedef struct {
    int32_t link_type;
    float offset1, offset2;
    float max_depth, length;
} __attribute__((packed)) swmm_link_info_values_t;

typedef struct {
    int32_t header[6];
    swmm_link_info_values_t values[4096];
} __attribute__((packed)) swmm_link_info_t;

#include "reader.h"
extern "C" {
#include "datetime.h"
#include "enums.h"
}

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    sbrr handle;
    if (sbrr_create(&handle, "out.seq.1") < 0) {
        perror("opening sbrr");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < sbrr_get_num_periods(handle); ++i) {
        DateTime date_time = sbrr_get_result_date(handle, i);
        char date_string[512];
        char time_string[512];
        datetime_dateToStr(date_time, date_string);
        datetime_timeToStr(date_time, time_string);
        printf("%s %s\n", date_string, time_string);
        int subc_size = MAX_SUBCATCH_RESULTS + sbrr_get_num_elements(handle, Pollutant);
        printf("[SUBCATCHMENTS]\n");
        for (int j = 0; j < sbrr_get_num_elements(handle, SubCatchement); ++j) {
            float *results = sbrr_get_element_results(handle, i, j, SubCatchement);
            char *id = sbrr_get_element_id(handle, j, SubCatchement);
            printf("%s:\t", id);
            for (int r = 0; r < subc_size; ++r) {
                printf("%5.2f\t", results[r]);
            }
            printf("\n");
        }
    }

    sbrr_destroy(handle);
}
