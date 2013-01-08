#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <cstring>
#include <string>
#include <math.h>
#include <vector>

#include "reader.h"
#include "datetime.h"
#include "enums.h"

void check_same_sim(sbrr *h, int count) {
    int num_subs = sbrr_get_num_elements(h[0], SubCatchement);
    int num_nodes = sbrr_get_num_elements(h[0], Node);
    int num_links = sbrr_get_num_elements(h[0], Link);
    for (int i = 0; i < count; i++) {
        assert(num_subs == sbrr_get_num_elements(h[i], SubCatchement));
        assert(num_nodes == sbrr_get_num_elements(h[i], Node));
        assert(num_links == sbrr_get_num_elements(h[i], Link));
    }

    for (int type = 0; type < Link+1; ++type) {
        for (int elem = 0; elem < sbrr_get_num_elements(h[0], (ElementType) type); elem++) {
            const char *id_1 = sbrr_get_element_id(h[0], elem, (ElementType) type);
            for (int j = 1; j < count; ++j) {
                const char *id_j = sbrr_get_element_id(h[j], elem, (ElementType) type);
                assert(strcmp(id_1, id_j) == 0);
            }
        }
    }
}

void print_dates(sbrr handle) {
    for (int i = 0; i < sbrr_get_num_periods(handle); ++i) {
        DateTime d = sbrr_get_result_date(handle, i);
        char date[512];
        char time[512];
        datetime_dateToStr(d, date);
        datetime_timeToStr(d, time);
        printf("%s - %s\n", date, time);
    }
}

struct diff_occurance {
    ElementType type; //which type
    int index; //which element
    const char *id;
    int period; //at what period
    int result_idx; //which result differ
    const char *result_name;
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("provide at least 2 report files\n");
        exit(EXIT_FAILURE);
    }

    int num_inputs = argc-1;

    sbrr *handles = (sbrr*) malloc(sizeof(sbrr)*(num_inputs));

    for (int i = 0; i < num_inputs; ++i) {
        handles[i] = sbrr_create();
        if (sbrr_open(handles[i], argv[i+1]) < 0) {
            perror("opening report file");
            return EXIT_FAILURE;
        }
        //print_dates(handles[i]);
    }

    check_same_sim(handles, num_inputs);

    std::vector<diff_occurance> diffs;

    int n; //stores the number of results

    for (int period = 0; period < sbrr_get_num_periods(handles[0]); ++period) {

        for (int idx = 0; idx < sbrr_get_num_elements(handles[0], Node); ++idx) {
            const char *id = sbrr_get_element_id(handles[0], idx, Node);
            float *results_0;
            sbrr_get_element_results(handles[0], period, idx, Node, &results_0, &n);

            bool found_diff = false;
            for (int h_idx = 1; h_idx < num_inputs; ++h_idx) {
                if (found_diff) break;
                float *results_i;
                sbrr_get_element_results(handles[h_idx], period, idx, Node, &results_i, &n);
                if (results_0[NODE_DEPTH] != results_i[NODE_DEPTH]) {
                    found_diff = true;
                    diff_occurance d = {Node, idx, id, period, NODE_DEPTH, NodeResultTypeName[NODE_DEPTH]};
                    diffs.push_back(d);
                }
            }
        }

        for (int idx = 0; idx < sbrr_get_num_elements(handles[0], Link); ++idx) {
            const char *id = sbrr_get_element_id(handles[0], idx, Link);
            float *results_0;
            sbrr_get_element_results(handles[0], period, idx, Link, &results_0, &n);
            const int wr[] = {LINK_FLOW, LINK_VELOCITY}; //wr = wanted results

            for (size_t r = 0; r < sizeof(wr)/sizeof(wr[0]); ++r) {
                bool found_diff = false;
                for (int h_idx = 1; h_idx < num_inputs; ++h_idx) {
                    if (found_diff) break;
                    float *results_i;
                    sbrr_get_element_results(handles[h_idx], period, idx, Link, &results_i, &n);
                    if (results_0[r] != results_i[r]) {
                        found_diff = true;
                        diff_occurance d = {Link, idx, id, period, (int) r, LinkResultTypeName[r]};
                        diffs.push_back(d);
                    }
                }
            }
        }
    }

    FILE *names = fopen("names.py", "w+");
    //header
    fprintf(names, "names=[");
    for (diff_occurance &d: diffs) {
        fprintf(names, "'%s_%s_%d', ", d.result_name, d.id, d.period);
    }
    fprintf(names, "]\n");
    fclose(names);

    //printf("x=[");

    for(int h_idx = 0; h_idx < num_inputs; ++h_idx) {
        //qprintf("[");
        for (diff_occurance &d: diffs) {
            float *results;
            sbrr_get_element_results(handles[h_idx], d.period, d.index, d.type, &results, &n);
            printf("%.20f ", results[d.result_idx]);
        }
        //printf("], ");
        printf("\n");
    }
    //printf("]");
}
