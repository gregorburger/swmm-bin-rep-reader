#ifndef ENUMS_H
#define ENUMS_H

#ifdef __cplusplus
extern "C" {
#endif

enum ConcentrationUnitTypes {
#define X(name, desc) name,
#include "concentration_units.def"
#undef X
};

const char* const ConcentrationUnitDesc[] = {
#define X(name, desc) desc,
#include "concentration_units.def"
#undef X
};

enum FlowUnitsType {
#define X(name, desc) name,
#include "flow_units.def"
#undef X
};

const char* const FlowUnitsToDesc[] = {
#define X(name, desc) desc,
#include "flow_units.def"
#undef X
};


enum InputDataType {
#define X(name) name,
#include "input_data_type.def"
#undef X
};

#define SUBCATCH_RESULTS_MAX 7
enum SubcatchResultType {
#define X(name, desc) name,
#include "subcatch_result_type.def"
#undef X
};

const char *const SubcatchResultTypeDesc[] = {
#define X(name, desc) desc
#include "subcatch_result_type.def"
#undef X
};

//-------------------------------------
// Computed node quantities
//-------------------------------------
#define NODE_RESULTS_MAX 7
typedef enum {
#define X(name, desc) name,
#include "node_result_type.def"
#undef X
} NodeResultType;

const char *const NodeResultTypeDesc[] = {
#define X(name, desc) desc,
#include "node_result_type.def"
#undef X
};

const char *const NodeResultTypeName[] = {
#define X(name, desc) #name,
#include "node_result_type.def"
#undef X
};

//-------------------------------------
// Computed link quantities
//-------------------------------------
#define LINK_RESULTS_MAX 6
typedef enum {
#define X(name, desc) name,
#include "link_result_type.def"
#undef X
} LinkResultType;

const char *const LinkResultTypeDesc[] = {
#define X(name, desc) desc,
#include "link_result_type.def"
#undef X
};

const char *const LinkResultTypeName[] = {
#define X(name, desc) #name,
#include "link_result_type.def"
#undef X
};

typedef enum {
#define X(name, max) name,
#include "element_type.def"
#undef X
} ElementType;

const char* const ElementTypeDesc[] = {
#define X(name, max) #name,
#include "element_type.def"
#undef X
};

const int ElementTypeMax[] = {
#define X(name, max) max,
#include "element_type.def"
#undef X
};

//-------------------------------------
// System-wide flow quantities
//-------------------------------------
#define MAX_SYS_RESULTS 14                                                     //(5.0.016 - LR)
enum SysFlowType {
    SYS_TEMPERATURE,                  // air temperature
    SYS_RAINFALL,                     // rainfall intensity
    SYS_SNOWDEPTH,                    // snow depth
    SYS_LOSSES,                       // evap + infil
    SYS_RUNOFF,                       // runoff flow
    SYS_DWFLOW,                       // dry weather inflow
    SYS_GWFLOW,                       // ground water inflow
    SYS_IIFLOW,                       // RDII inflow
    SYS_EXFLOW,                       // external inflow
    SYS_INFLOW,                       // total lateral inflow
    SYS_FLOODING,                     // flooding outflow
    SYS_OUTFLOW,                      // outfall outflow
    SYS_STORAGE,                      // storage volume
    SYS_EVAPORATION};                 // evaporation                          //(5.0.016 - LR)

#ifdef __cplusplus
}
#endif

#endif // ENUMS_H
