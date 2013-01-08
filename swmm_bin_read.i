%module pyswmm_bin_read
%{
#define SWIG_FILE_WITH_INIT
#include "reader.h"
%}

%include "numpy.i"

%init %{
import_array();
%}

%include "enums_pp.h"

%rename("%(strip:[sbrr_])s") "";
%include "reader.h"
void    sbrr_get_node_results(sbrr handle,
                              int index,
                              NodeResultType type,
                              float *INPLACE_ARRAY1,
                              int DIM1);

void    sbrr_get_link_results(sbrr handle,
                              int index,
                              LinkResultType type,
                              float *INPLACE_ARRAY1,
                              int DIM1);

