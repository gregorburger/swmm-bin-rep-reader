#include "reader.h"
#include <inttypes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "enums.h"
#include "parse_types.h"

#define MAGIC 516114522

struct sbrr_t {
    struct stat buf;
    int fd;
    char *map;
    header_t *header;
    footer_t *footer;

//ids
    char **node_ids;
    char **link_ids;
    char **subcatchment_ids;
    int32_t *pollutant_types;

//sizes
    size_t subcatchment_result_len;
    size_t node_result_len;
    size_t link_result_len;
    size_t bytes_per_period;
};

size_t read_ids(char **ids, char *map, size_t _offset, int index ) {
    size_t offset = _offset;
    for (int i = 0; i < index ; i++) {
        swmm_id_t *id = (swmm_id_t*) (map + offset);
        ids[i] = (char*) malloc(id->len+1);
        assert(id->len < 512);
        strncpy(ids[i], id->id, id->len);
        ids[i][id->len] = '\0';
        offset += sizeof(int32_t) + id->len*sizeof(char);
    }
    return offset - _offset;
}

void sanity_check(sbrr handle) {
    assert(handle->header->magic == MAGIC);
    assert(handle->footer->magic == MAGIC);
    assert(sizeof(header_t) == handle->footer->ids_offset);
    int32_t results_size = handle->bytes_per_period*handle->footer->num_periods;
    int32_t results_end = handle->buf.st_size - sizeof(footer_t);
    assert(handle->footer->output_offset == results_end-results_size);
}

sbrr sbrr_create() {
    sbrr handle = (sbrr) malloc(sizeof(struct sbrr_t));
    return handle;
}

int sbrr_open(sbrr handle, const char *binary_report_file) {
    size_t offset;

    handle->fd = open(binary_report_file, O_RDONLY);
    if (handle->fd < 0) {
        free(handle);
        return errno;
    }

    if (fstat(handle->fd, &handle->buf) < 0) {
        free(handle);
        close(handle->fd);
        return errno;
    }

    handle->map = (char *) malloc(handle->buf.st_size);
    read(handle->fd, (void *) handle->map, handle->buf.st_size);

    handle->header = (header_t*) handle->map;
    handle->footer = (footer_t*) (handle->map + handle->buf.st_size-sizeof(footer_t));
    if (handle->header->magic != MAGIC || handle->footer->magic != MAGIC) {
        close(handle->fd);
        free(handle->map);
        free(handle);
        return -1;
    }

    offset = sizeof(header_t);

    handle->subcatchment_ids = (char **) malloc(handle->header->num_subcatchments * sizeof(char **));
    handle->node_ids = (char **) malloc(handle->header->num_nodes * sizeof(char **));
    handle->link_ids = (char **) malloc(handle->header->num_links * sizeof(char **));

    offset += read_ids(handle->subcatchment_ids, handle->map, offset, handle->header->num_subcatchments);
    offset += read_ids(handle->node_ids, handle->map, offset, handle->header->num_nodes);
    offset += read_ids(handle->link_ids, handle->map, offset, handle->header->num_links);

    handle->pollutant_types = malloc(sizeof(int)*handle->header->num_pollutants);
    for (int i = 0; i < handle->header->num_pollutants; i++) {
        int32_t *type = (int32_t*) (handle->map+offset);
        handle->pollutant_types[i] = *type;
        offset += sizeof(int32_t);
    }

    int npol = handle->header->num_pollutants;
    handle->subcatchment_result_len =  SUBCATCH_RESULTS_MAX - 1 + npol;
    handle->node_result_len = NODE_RESULTS_MAX - 1 + npol;
    handle->link_result_len = LINK_RESULTS_MAX - 1 + npol;

    handle->bytes_per_period = sizeof(double)
        + handle->header->num_subcatchments * handle->subcatchment_result_len * sizeof(float)
        + handle->header->num_nodes * handle->node_result_len * sizeof(float)
        + handle->header->num_links * handle->link_result_len * sizeof(float)
        + MAX_SYS_RESULTS * sizeof(float);

    sanity_check(handle);

    return 0;
}

void sbrr_destroy(sbrr handle) {
    free(handle->pollutant_types);
    for (int i = 0; i < handle->header->num_links; ++i) {
        free(handle->link_ids[i]);
    }
    free(handle->link_ids);
    for (int i = 0; i < handle->header->num_nodes; ++i) {
        free(handle->node_ids[i]);
    }
    free(handle->node_ids);
    for (int i = 0; i < handle->header->num_subcatchments; ++i) {
        free(handle->subcatchment_ids[i]);
    }
    free(handle->subcatchment_ids);
    free(handle->map);
    close(handle->fd);
    free(handle);
}

int sbrr_get_num_elements(sbrr handle,
                          ElementType type) {
    switch (type) {
    case Node:
        return handle->header->num_nodes;
        break;
    case Link:
        return handle->header->num_links;
        break;
    case SubCatchement:
        return handle->header->num_subcatchments;
        break;
    case Pollutant:
        return handle->header->num_pollutants;
        break;
    default:
        return -1;
        break;
    }
    return -1;
}

int sbrr_get_num_periods(sbrr handle) {
    return handle->footer->num_periods;
}

const char *sbrr_get_element_id(sbrr handle,
                             int index ,
                             ElementType type) {
    char **ids;
    if (type == Node) ids = handle->node_ids;
    if (type == Link) ids = handle->link_ids;
    if (type == SubCatchement) ids = handle->subcatchment_ids;

    return ids[index];
}

int sbrr_get_element_index(sbrr handle, const char *id, ElementType type) {
    char **ids;
    int len = sbrr_get_num_elements(handle, type);
    if (type == Node) ids = handle->node_ids;
    if (type == Link) ids = handle->link_ids;
    if (type == SubCatchement) ids = handle->subcatchment_ids;

    for (int i = 0; i < len; ++i) {
        if (strcmp(ids[i], id) == 0) {
            return i;
        }
    }
    return -1;
}

double sbrr_get_result_date(sbrr handle, int period) {
    size_t offset = handle->footer->output_offset + (period)*handle->bytes_per_period;
    double *date = (double *) (handle->map + offset);
    return *date;
}

void sbrr_get_element_results(sbrr handle,
                              int period,
                              int index,
                              ElementType type,
                              float **data,
                              int *n) {

    if (type == SubCatchement) {
        size_t offset = handle->footer->output_offset + (period)*handle->bytes_per_period;
        offset += sizeof(double);
        offset += index * handle->subcatchment_result_len*sizeof(float);
        *data = (float *) (handle->map + offset);
        *n = handle->subcatchment_result_len;
        return;
    }
    if (type == Node) {
        size_t offset = handle->footer->output_offset + (period)*handle->bytes_per_period;
        offset += sizeof(double);
        offset += handle->header->num_subcatchments * handle->subcatchment_result_len*sizeof(float);
        offset += index * handle->node_result_len*sizeof(float);
        *data = (float *) (handle->map + offset);
        *n = handle->node_result_len;
        return;
    }
    if (type == Link) {
        size_t offset = handle->footer->output_offset + (period)*handle->bytes_per_period;
        offset += sizeof(double);
        offset += handle->header->num_subcatchments * handle->subcatchment_result_len*sizeof(float);
        offset += handle->header->num_nodes * handle->node_result_len*sizeof(float);
        offset += index * handle->link_result_len*sizeof(float);
        *data = (float *) (handle->map + offset);
        *n = handle->link_result_len;
        return;
    }
    *data = 0;
    *n = 0;
}

void sbrr_get_node_results(sbrr handle, int index, NodeResultType type, float *data, int n) {
    int _n;
    float *_data;
    for (int p = 0; p < n; p++) {
        sbrr_get_element_results(handle, p, index, Node, &_data, &_n);
        data[p] = _data[type];
    }
}

void sbrr_get_link_results(sbrr handle, int index, LinkResultType type, float *data, int n) {
    int _n;
    float *_data;
    for (int p = 0; p < n; p++) {
        sbrr_get_element_results(handle, p, index, Link, &_data, &_n);
        data[p] = _data[type];
    }
}

