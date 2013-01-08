# 1 "../enums.h"
# 1 "<command-line>"
# 1 "../enums.h"







enum ConcentrationUnitTypes {

# 1 "../concentration_units.def" 1
MG,
UG,
COUNT,
# 11 "../enums.h" 2

};

const char* const ConcentrationUnitDesc[] = {

# 1 "../concentration_units.def" 1
"Milligrams / L",
"Micrograms / L",
"Counts / L",
# 17 "../enums.h" 2

};

enum FlowUnitsType {

# 1 "../flow_units.def" 1
CFS,
GPM,
MGD,
CMS,
LPS,
MLD,
# 23 "../enums.h" 2

};

const char* const FlowUnitsToDesc[] = {

# 1 "../flow_units.def" 1
"cubic feet per second",
"gallons per minute",
"million gallons per day",
"cubic meters per second",
"liters per second",
"million liters per day",
# 29 "../enums.h" 2

};


enum InputDataType {

# 1 "../input_data_type.def" 1
INPUT_TYPE_CODE,
INPUT_AREA,
INPUT_INVERT,
INPUT_MAX_DEPTH,
INPUT_OFFSET,
INPUT_LENGTH,
# 36 "../enums.h" 2

};


enum SubcatchResultType {

# 1 "../subcatch_result_type.def" 1
SUBCATCH_RAINFALL,
SUBCATCH_SNOWDEPTH,
SUBCATCH_LOSSES,
SUBCATCH_RUNOFF,
SUBCATCH_GW_FLOW,
SUBCATCH_GW_ELEV,
SUBCATCH_WASHOFF,
# 43 "../enums.h" 2

};

const char *const SubcatchResultTypeDesc[] = {

# 1 "../subcatch_result_type.def" 1
"rainfall intensity"
"snow depth"
"total losses (evap + infil)"
"runoff flow rate"
"groundwater flow rate to node"
"elevation of saturated gw table"
"pollutant washoff concentration"
# 49 "../enums.h" 2

};





enum NodeResultType {

# 1 "../node_result_type.def" 1
NODE_DEPTH,
NODE_HEAD,
NODE_VOLUME,
NODE_LATFLOW,
NODE_INFLOW,
NODE_OVERFLOW,
NODE_QUAL,
# 59 "../enums.h" 2

};

const char *const NodeResultTypeDesc[] = {

# 1 "../node_result_type.def" 1
"water depth above invert",
"hydraulic head",
"volume stored & ponded",
"lateral inflow rate",
"total inflow rate",
"overflow rate",
"concentration of each pollutant",
# 65 "../enums.h" 2

};

const char *const NodeResultTypeName[] = {

# 1 "../node_result_type.def" 1
"NODE_DEPTH",
"NODE_HEAD",
"NODE_VOLUME",
"NODE_LATFLOW",
"NODE_INFLOW",
"NODE_OVERFLOW",
"NODE_QUAL",
# 71 "../enums.h" 2

};





enum LinkResultType {

# 1 "../link_result_type.def" 1
LINK_FLOW,
LINK_DEPTH,
LINK_VELOCITY,
LINK_FROUDE,
LINK_CAPACITY,
LINK_QUAL,
# 81 "../enums.h" 2

};

const char *const LinkResultTypeDesc[] = {

# 1 "../link_result_type.def" 1
"flow rate",
"flow depth",
"flow velocity",
"Froude number",
"ratio of depth to full depth",
"concentration of each pollutant",
# 87 "../enums.h" 2

};

const char *const LinkResultTypeName[] = {

# 1 "../link_result_type.def" 1
"LINK_FLOW",
"LINK_DEPTH",
"LINK_VELOCITY",
"LINK_FROUDE",
"LINK_CAPACITY",
"LINK_QUAL",
# 93 "../enums.h" 2

};

typedef enum {

# 1 "../element_type.def" 1
SubCatchement,
Node,
Link,
Pollutant,
# 99 "../enums.h" 2

} ElementType;

const char* const ElementTypeDesc[] = {

# 1 "../element_type.def" 1
"SubCatchement",
"Node",
"Link",
"Pollutant",
# 105 "../enums.h" 2

};

const int ElementTypeMax[] = {

# 1 "../element_type.def" 1
7,
7,
6,
0,
# 111 "../enums.h" 2

};





enum SysFlowType {
    SYS_TEMPERATURE,
    SYS_RAINFALL,
    SYS_SNOWDEPTH,
    SYS_LOSSES,
    SYS_RUNOFF,
    SYS_DWFLOW,
    SYS_GWFLOW,
    SYS_IIFLOW,
    SYS_EXFLOW,
    SYS_INFLOW,
    SYS_FLOODING,
    SYS_OUTFLOW,
    SYS_STORAGE,
    SYS_EVAPORATION};
