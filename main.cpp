#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <cstring>
#include <string>

#define MAGIC 516114522

enum FlowUnitsType {
#define X(name, desc) name,
#include "flow_units.def"
#undef X
};

const char* const FlowUnitsToDesc[] {
#define X(name, desc) desc,
#include "flow_units.def"
#undef X
};

enum ConcentrationUnitTypes {
#define X(name, desc) name,
#include "concentration_units.def"
#undef X
};

const char* const ConcentrationUnitToDesc[] {
#define X(name, desc) desc,
#include "concentration_units.def"
#undef X
};

enum InputDataType {
#define X(name) name,
#include "input_data_type.def"
#undef X
};

typedef struct {
    int32_t magic;
    int32_t version;
    int32_t flow_units;
    int32_t num_subcatchments;
    int32_t num_nodes;
    int32_t num_links;
    int32_t num_pollutants;
} __attribute__((packed)) header_t;

typedef struct {
    int32_t one;
    int32_t input_area_magic;
    float areas[4096];
} __attribute__((packed)) swmm_catch_area_t;

typedef struct {
    int32_t len;
    char id[512];
} __attribute__((packed)) swmm_id_t;

size_t read_ids(void *map, size_t _offset, int num) {
    size_t offset = _offset;
    for (int i = 0; i < num; i++) {
        char cid[512];
        swmm_id_t *id = (swmm_id_t*) (map + offset);
        assert(id->len < 512);
        strncpy(cid, id->id, id->len);
        cid[id->len] = '\0';
        printf("%s\n", cid);
        offset += sizeof(int32_t) + id->len*sizeof(char);
    }
    return offset - _offset;
}

size_t read_catchement_areas(void *map, size_t offset, int num_c) {
    swmm_catch_area_t *area = (swmm_catch_area_t*) (map + offset);
    assert(area->one == 1);
    assert(area->input_area_magic == INPUT_AREA);
    assert(num_c  < 4096);
    for (int i = 0; i < num_c+1; i++) {
        printf("catch[%d].area = %f\n", i, area->areas[i]);
    }
    return 2*sizeof(int32_t) + num_c*sizeof(float);
}

typedef struct {
    int32_t node_type;
    float invert_elevation;
    float full_depth;
} __attribute__((packed)) swmm_node_info_values_t;

typedef struct {
    int32_t header[4];
    swmm_node_info_values_t values[4096];
} __attribute__((packed)) swmm_node_info_t;

size_t read_node_info(void *map, size_t offset, int num_nodes) {
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

typedef struct {
    int32_t link_type;
    float offset1, offset2;
    float max_depth, length;
} __attribute__((packed)) swmm_link_info_values_t;

typedef struct {
    int32_t header[6];
    swmm_link_info_values_t values[4096];
} __attribute__((packed)) swmm_link_info_t;

size_t read_link_info(void *map, size_t offset, int num_links) {
    swmm_link_info_t *info = (swmm_link_info_t*) (map+offset);

    assert(info->header[0] == 5);
    assert(info->header[1] == INPUT_TYPE_CODE);
    assert(info->header[2] == INPUT_OFFSET);
    assert(info->header[3] == INPUT_OFFSET);
    assert(info->header[4] == INPUT_MAX_DEPTH);
    assert(info->header[5] == INPUT_LENGTH);

    for (int i = 0; i < num_links; ++i) {
        printf("link[%d].type = %d\n", i, info->values[i].link_type);
    }

    return sizeof(int32_t)*6 + sizeof(float)*4*num_links;
}

typedef struct {
    int32_t ids_offset;
    int32_t input_offset;
    int32_t output_offset;
    int32_t num_periods;
    int32_t error_code;
    int32_t magic;
} __attribute__((packed)) footer_t;

int main(int argc, char *argv[]) {
    (void) argc;
    (void) argv;

    int fd;
    void *map;
    struct stat buf;

    fd = open("out.seq.1", O_RDONLY);
    if (fd < 0) {
        perror("open file");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &buf) < 0) {
        perror("statting");
        exit(EXIT_FAILURE);
    }


    map = mmap(0, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) {
        perror("mmapping file");
        exit(EXIT_FAILURE);
    }

    header_t *header = (header_t*) map;

    assert(header->magic == MAGIC);
    printf("Magic Number: %d\n", header->magic);
    printf("Version: %d\n", header->version);
    printf("Flow Units: %s\n", FlowUnitsToDesc[header->flow_units]);
    printf("Number of SubCatchments: %d\n", header->num_subcatchments);
    printf("Number of Nodes: %d\n", header->num_nodes);
    printf("Number of Links: %d\n", header->num_links);
    printf("Number of Pollutants: %d\n", header->num_pollutants);

    footer_t *footer = (footer_t*) (map+buf.st_size-sizeof(footer_t));
    assert(footer->magic == MAGIC);
    printf("Number of Periods: %d\n", footer->num_periods);
    printf("Error Code: %d\n", footer->error_code);

    //exit(EXIT_FAILURE);

    size_t offset = sizeof(header_t);

    //LOAD IDS

    printf("\n[SUBCATCHMENTS]\n\n");
    offset += read_ids(map, offset, header->num_subcatchments);

    printf("\n[NODES]\n\n");
    offset += read_ids(map, offset, header->num_nodes);

    printf("\n[LINKS]\n\n");
    offset += read_ids(map, offset, header->num_links);

    printf("\n[POLLUTATNS]\n\n");
    offset += read_ids(map, offset, header->num_pollutants);

    for (int i = 0; i < header->num_pollutants; i++) {
        int32_t *type = (int32_t*) (map+offset);
        offset += sizeof(int32_t);
        printf("Pollutaton %d has type %s\n", i, ConcentrationUnitToDesc[*type]);
    }

    //LOAD CATCHMENT AREAS
    offset += read_catchement_areas(map, offset, header->num_subcatchments);

    //LOAD NODE TYPES MAX_DEPTH
    offset += read_node_info(map, offset, header->num_nodes);

    //load link infos
    offset += read_link_info(map, offset, header->num_links);

    //ignore catchment configuration
    offset += sizeof(int32_t)*(7+header->num_pollutants);

    //ignore node configuration
    offset += sizeof(int32_t)*(7+header->num_nodes);

    //ignore link configuration
    offset += sizeof(int32_t)*(6+header->num_links);

    //ignore system configuration
    offset += sizeof(int32_t)*15;

    double *report_date = (double *) (map+offset);
    offset += sizeof(double);
    printf("report date %f\n", *report_date);

    int32_t *report_step = (int32_t*) (map+offset);
    offset += sizeof(int32_t);
    printf("report step %d\n", report_step);
}
