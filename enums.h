#ifndef ENUMS_H
#define ENUMS_H

enum ConcentrationUnitTypes {
#define X(name, desc) name,
#include "concentration_units.def"
#undef X
};

const char* const ConcentrationUnitToDesc[] = {
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

#define MAX_SUBCATCH_RESULTS 7
enum SubcatchResultType {
     SUBCATCH_RAINFALL,               // rainfall intensity
     SUBCATCH_SNOWDEPTH,              // snow depth
     SUBCATCH_LOSSES,                 // total losses (evap + infil)
     SUBCATCH_RUNOFF,                 // runoff flow rate
     SUBCATCH_GW_FLOW,                // groundwater flow rate to node
     SUBCATCH_GW_ELEV,                // elevation of saturated gw table
     SUBCATCH_WASHOFF};               // pollutant washoff concentration

//-------------------------------------
// Computed node quantities
//-------------------------------------
#define MAX_NODE_RESULTS 7
enum NodeResultType {
     NODE_DEPTH,                      // water depth above invert
     NODE_HEAD,                       // hydraulic head
     NODE_VOLUME,                     // volume stored & ponded
     NODE_LATFLOW,                    // lateral inflow rate
     NODE_INFLOW,                     // total inflow rate
     NODE_OVERFLOW,                   // overflow rate
     NODE_QUAL};                      // concentration of each pollutant

//-------------------------------------
// Computed link quantities
//-------------------------------------
#define MAX_LINK_RESULTS 6
enum LinkResultType {
     LINK_FLOW,                       // flow rate
     LINK_DEPTH,                      // flow depth
     LINK_VELOCITY,                   // flow velocity
     LINK_FROUDE,                     // Froude number
     LINK_CAPACITY,                   // ratio of depth to full depth
     LINK_QUAL};                      // concentration of each pollutant

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


#endif // ENUMS_H
