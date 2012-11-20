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
    int period; //at what period
    int result_idx; //which result differ
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("provide at least 2 report files\n");
        exit(EXIT_FAILURE);
    }

    int num_inputs = argc-1;

    sbrr *handles = (sbrr*) malloc(sizeof(sbrr)*(num_inputs));

    for (int i = 0; i < num_inputs; ++i) {
        if (sbrr_create(&handles[i], argv[i+1]) < 0) {
            perror("opening report file");
            return EXIT_FAILURE;
        }
        //print_dates(handles[i]);
    }

    check_same_sim(handles, num_inputs);

    std::vector<diff_occurance> diffs;

    for (int period = 0; period < sbrr_get_num_periods(handles[0]); ++period) {

        for (int type = 0; type < Pollutant; ++type) {

            //printf("[%s]\n", ElementTypeDesc[type]);
            for (int idx = 0; idx < sbrr_get_num_elements(handles[0], SubCatchement); ++idx) {
                const char *id = sbrr_get_element_id(handles[0], idx, (ElementType) type);
                (void) id;
                const float *results_0 = sbrr_get_element_results(handles[0], period, idx, (ElementType) type);
                for (int r = 0; r < ElementTypeMax[type]; ++r) {
                    bool found_diff = false;
                    for (int h_idx = 1; h_idx < num_inputs; ++h_idx) {
                        if (found_diff) break;
                        const float *results_i = sbrr_get_element_results(handles[h_idx], period, idx, (ElementType) type);
                        if (results_0[r] != results_i[r]) {
                            //printf("%d of %20s in period %d: diff %2.20f\n", r, id, period, fabs(results_0[r] - results_i[r]));
                            //printf("found difference at period %d with element %s in result %d\n", period, id, r);
                            found_diff = true;
                            diff_occurance d = {(ElementType) type, idx, period, r};
                            diffs.push_back(d);
                        }
                    }
                }
            }
        }
    }

    //printf("found %lu diffs\n", diffs.size());

    diffs.resize(10);

    for(int h_idx = 0; h_idx < num_inputs; ++h_idx) {
        for (diff_occurance &d: diffs) {
            const float *results = sbrr_get_element_results(handles[h_idx], d.period, d.index, d.type);
            printf("%f;", results[d.result_idx]);
        }
        printf("\n");
    }
}
