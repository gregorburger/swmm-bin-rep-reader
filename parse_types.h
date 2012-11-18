#ifndef PARSE_TYPES_H
#define PARSE_TYPES_H

typedef struct {
    int32_t len;
    char id[512];
} __attribute__((packed)) swmm_id_t;

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
    int32_t ids_offset;
    int32_t input_offset;
    int32_t output_offset;
    int32_t num_periods;
    int32_t error_code;
    int32_t magic;
} __attribute__((packed)) footer_t;

#endif // PARSE_TYPES_H
