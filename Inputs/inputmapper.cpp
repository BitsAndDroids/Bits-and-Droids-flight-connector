#include "Inputs/InputMapper.h"

#include <Inputs/inputenum.h>
#include <headers/SimConnect.h>
#include <qsettings.h>
#include <qstandardpaths.h>
#include <tchar.h>
#include <windows.h>

InputEnum definitionsMap;

InputMapper::InputMapper() {}
void InputMapper::mapEvents(HANDLE mapConnect) {
  SimConnect_MapClientEventToSimEvent(mapConnect,
                                      definitionsMap.DATA_EX_THROTTLE_1_AXIS,
                                      "THROTTLE1_AXIS_SET_EX1");
  SimConnect_MapClientEventToSimEvent(mapConnect,
                                      definitionsMap.DATA_EX_THROTTLE_2_AXIS,
                                      "THROTTLE2_AXIS_SET_EX1");
  SimConnect_MapClientEventToSimEvent(mapConnect,
                                      definitionsMap.DATA_EX_THROTTLE_3_AXIS,
                                      "THROTTLE3_AXIS_SET_EX1");
  SimConnect_MapClientEventToSimEvent(mapConnect,
                                      definitionsMap.DATA_EX_THROTTLE_4_AXIS,
                                      "THROTTLE4_AXIS_SET_EX1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_1_AXIS, "AXIS_THROTTLE1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_2_AXIS, "AXIS_THROTTLE2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_3_AXIS, "AXIS_THROTTLE3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_4_AXIS, "AXIS_THROTTLE4_SET");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_1,
      "GENERAL ENG MIXTURE LEVER POSITION:1", "percent");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_2,
      "GENERAL ENG MIXTURE LEVER POSITION:2", "percent");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_LEVER_AXIS_1,
      "AXIS_MIXTURE1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_LEVER_AXIS_2,
      "AXIS_MIXTURE2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_LEVER_AXIS_3,
      "AXIS_MIXTURE3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MIXTURE_LEVER_AXIS_4,
      "AXIS_MIXTURE4_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_2_AXIS, "AXIS_THROTTLE2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_3_AXIS, "AXIS_THROTTLE3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DATA_THROTTLE_4_AXIS, "AXIS_THROTTLE4_SET");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_PROPELLER_LEVER_POSITION_1,
      "GENERAL ENG PROPELLER LEVER POSITION:1", "percent");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_PROPELLER_LEVER_POSITION_2,
      "GENERAL ENG PROPELLER LEVER POSITION:2", "percent");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PROP_LEVER_AXIS_1,
      "AXIS_PROPELLER1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PROP_LEVER_AXIS_2,
      "AXIS_PROPELLER2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PROP_LEVER_AXIS_3,
      "AXIS_PROPELLER3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PROP_LEVER_AXIS_4,
      "AXIS_PROPELLER4_SET");

  // Avionics
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AVIONICS1_MASTER_ON,
      "AVIONICS_MASTER_1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AVIONICS1_MASTER_OFF,
      "AVIONICS_MASTER_1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AVIONICS2_MASTER_ON,
      "AVIONICS_MASTER_2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AVIONICS2_MASTER_OFF,
      "AVIONICS_MASTER_2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_MASTER_ALTERNATOR,
      "TOGGLE_MASTER_ALTERNATOR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PARKING_BRAKE, "PARKING BRAKE");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PITOT_HEAT_TOGGLE,
      "PITOT_HEAT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PITOT_HEAT_ON, "PITOT_HEAT_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PITOT_HEAT_OFF, "PITOT_HEAT_OFF");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO, "MAGNETO");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_DECR, "MAGNETO_DECR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_INCR, "MAGNETO_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_OFF, "MAGNETO1_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_RIGHT, "MAGNETO1_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_LEFT, "MAGNETO1_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_BOTH, "MAGNETO1_BOTH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_START, "MAGNETO1_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_OFF, "MAGNETO2_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_RIGHT, "MAGNETO2_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_LEFT, "MAGNETO2_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_BOTH, "MAGNETO2_BOTH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_START, "MAGNETO2_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_OFF, "MAGNETO3_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_RIGHT, "MAGNETO3_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_LEFT, "MAGNETO3_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_BOTH, "MAGNETO3_BOTH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_START, "MAGNETO3_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_OFF, "MAGNETO4_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_RIGHT, "MAGNETO4_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_LEFT, "MAGNETO4_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_BOTH, "MAGNETO4_BOTH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_START, "MAGNETO4_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_OFF, "MAGNETO_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_RIGHT, "MAGNETO_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_LEFT, "MAGNETO_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_BOTH, "MAGNETO_BOTH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_START, "MAGNETO_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_DECR, "MAGNETO1_DECR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_INCR, "MAGNETO1_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_DECR, "MAGNETO2_DECR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_INCR, "MAGNETO2_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_DECR, "MAGNETO3_DECR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_INCR, "MAGNETO3_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_DECR, "MAGNETO4_DECR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_INCR, "MAGNETO4_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO_SET, "MAGNETO_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO1_SET, "MAGNETO1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO2_SET, "MAGNETO2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO3_SET, "MAGNETO3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_MAGNETO4_SET, "MAGNETO4_SET");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_ON, "ANTI_ICE_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_OFF, "ANTI_ICE_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_SET, "ANTI_ICE_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_TOGGLE, "ANTI_ICE_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_TOGGLE_ENG1,
      "ANTI_ICE_TOGGLE_ENG1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_TOGGLE_ENG2,
      "ANTI_ICE_TOGGLE_ENG2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_TOGGLE_ENG3,
      "ANTI_ICE_TOGGLE_ENG3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_TOGGLE_ENG4,
      "ANTI_ICE_TOGGLE_ENG4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_SET_ENG1,
      "ANTI_ICE_SET_ENG1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_SET_ENG2,
      "ANTI_ICE_SET_ENG2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_SET_ENG3,
      "ANTI_ICE_SET_ENG3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ANTI_ICE_SET_ENG4,
      "ANTI_ICE_SET_ENG4");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ALL,
      "TOGGLE_FUEL_VALVE_ALL");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ENG1,
      "TOGGLE_FUEL_VALVE_ENG1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ENG2,
      "TOGGLE_FUEL_VALVE_ENG2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ENG3,
      "TOGGLE_FUEL_VALVE_ENG3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ENG4,
      "definitionsMap.DEFINITION_TOGGLE_FUEL_VALVE_ENG4");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_OFF,
      "FUEL_SELECTOR_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_ALL,
      "FUEL_SELECTOR_ALL");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_LEFT,
      "FUEL_SELECTOR_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_RIGHT,
      "FUEL_SELECTOR_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_LEFT_AUX,
      "FUEL_SELECTOR_LEFT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_RIGHT_AUX,
      "FUEL_SELECTOR_RIGHT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_CENTER,
      "FUEL_SELECTOR_CENTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_SET,
      "FUEL_SELECTOR_SET");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_OFF,
      "FUEL_SELECTOR_2_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_ALL,
      "FUEL_SELECTOR_2_ALL");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_LEFT,
      "FUEL_SELECTOR_2_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_RIGHT,
      "FUEL_SELECTOR_2_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_LEFT_AUX,
      "FUEL_SELECTOR_2_LEFT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_RIGHT_AUX,
      "FUEL_SELECTOR_2_RIGHT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_CENTER,
      "FUEL_SELECTOR_2_CENTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_SET,
      "FUEL_SELECTOR_2_SET");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_OFF,
      "FUEL_SELECTOR_3_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_ALL,
      "FUEL_SELECTOR_3_ALL");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_LEFT,
      "FUEL_SELECTOR_3_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_RIGHT,
      "FUEL_SELECTOR_3_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_LEFT_AUX,
      "FUEL_SELECTOR_3_LEFT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_RIGHT_AUX,
      "FUEL_SELECTOR_3_RIGHT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_CENTER,
      "FUEL_SELECTOR_3_CENTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_SET,
      "FUEL_SELECTOR_3_SET");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_OFF,
      "FUEL_SELECTOR_4_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_ALL,
      "FUEL_SELECTOR_4_ALL");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_LEFT,
      "FUEL_SELECTOR_4_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_RIGHT,
      "FUEL_SELECTOR_4_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_LEFT_AUX,
      "FUEL_SELECTOR_4_LEFT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_RIGHT_AUX,
      "FUEL_SELECTOR_4_RIGHT_AUX");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_CENTER,
      "FUEL_SELECTOR_4_CENTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_SET,
      "FUEL_SELECTOR_4_SET");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_CROSS_FEED_OPEN, "CROSS_FEED_OPEN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_CROSS_FEED_TOGGLE,
      "CROSS_FEED_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_CROSS_FEED_OFF, "CROSS_FEED_OFF");

  // Battery
  SimConnect_AddToDataDefinition(mapConnect, definitionsMap.DEFINITION_BAT_1_ON,
                                 "ELECTRICAL MASTER BATTERY:1", "Bool",
                                 SIMCONNECT_DATATYPE_INT32);
  SimConnect_AddToDataDefinition(mapConnect, definitionsMap.DEFINITION_BAT_2_ON,
                                 "ELECTRICAL MASTER BATTERY:2", "Bool");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_MASTER_BATTERY,
      "TOGGLE_MASTER_BATTERY");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_MASTER_ALTERNATOR,
      "TOGGLE_MASTER_BATTERY_ALTERNATOR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_EXTERNAL_POWER,
      "TOGGLE_EXTERNAL_POWER");
  // Engines
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_THROTTLE_1,
      "GENERAL ENG THROTTLE LEVER POSITION:1", "percent");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_THROTTLE_2,
      "GENERAL ENG THROTTLE LEVER POSITION:2", "percent");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_THROTTLE_3,
      "GENERAL ENG THROTTLE LEVER POSITION:3", "percent");
  SimConnect_AddToDataDefinition(
      mapConnect, definitionsMap.DEFINITION_THROTTLE_4,
      "GENERAL ENG THROTTLE LEVER POSITION:4", "percent");
  // AP
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MASTER, "AP_MASTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_HEADING_HOLD,
      "AP_PANEL_HEADING_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_ALTITUDE_HOLD,
      "AP_PANEL_ALTITUDE_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ATT_HOLD_ON, "AP_ATT_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_LOC_HOLD_ON, "AP_LOC_HOLD_ON");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_APR_HOLD_ON, "AP_APR_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_HDG_HOLD_ON, "AP_HDG_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_HOLD_ON, "AP_ALT_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_WING_LEVELER_ON,
      "AP_WING_LEVELER_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_BC_HOLD_ON, "AP_BC_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_NAV1_HOLD_ON, "AP_NAV1_HOLD_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ATT_HOLD_OFF, "AP_ATT_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_LOC_HOLD_OFF, "AP_LOC_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_APR_HOLD_OFF, "AP_APR_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_HDG_HOLD_OFF, "AP_HDG_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_HOLD_OFF, "AP_ALT_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_WING_LEVELER_OFF,
      "AP_WING_LEVELER_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_BC_HOLD_OFF, "AP_BC_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_NAV1_HOLD_OFF,
      "AP_NAV1_HOLD_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_AIRSPEED_HOLD,
      "AP_AIRSPEED_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_SPEED_HOLD,
      "AP_PANEL_SPEED_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_VAR_INC, "AP_ALT_VAR_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_VAR_DEC, "AP_ALT_VAR_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_VS_VAR_INC, "AP_VS_VAR_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_VS_VAR_DEC, "AP_VS_VAR_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_SPD_VAR_INC, "AP_SPD_VAR_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_SPD_VAR_DEC, "AP_SPD_VAR_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_MACH_HOLD,
      "AP_PANEL_MACH_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_VAR_INC, "AP_MACH_VAR_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_VAR_DEC, "AP_MACH_VAR_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_HOLD, "AP_MACH_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_VAR_SET_METRIC,
      "AP_ALT_VAR_SET_METRIC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_VS_VAR_SET_ENGLISH,
      "AP_VS_VAR_SET_ENGLISH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_SPD_VAR_SET, "AP_SPD_VAR_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_VAR_SET, "AP_MACH_VAR_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_AIRSPEED_ON, "AP_AIRSPEED_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_AIRSPEED_OFF, "AP_AIRSPEED_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_AIRSPEED_SET, "AP_AIRSPEED_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_ON, "AP_MACH_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_OFF, "AP_MACH_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MACH_SET, "AP_MACH_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_ALTITUDE_ON,
      "AP_PANEL_ALTITUDE_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_ALTITUDE_OFF,
      "AP_PANEL_ALTITUDE_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_ALTITUDE_SET, "");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_HEADING_ON,
      "AP_PANEL_HEADING_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_HEADING_OFF,
      "AP_PANEL_HEADING_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_HEADING_SET,
      "AP_PANEL_HEADING_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_MACH_ON,
      "AP_PANEL_MACH_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_MACH_OFF,
      "AP_PANEL_MACH_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_MACH_SET,
      "AP_PANEL_MACH_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_SPEED_ON,
      "AP_PANEL_SPEED_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_SPEED_OFF,
      "AP_PANEL_SPEED_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_SPEED_SET,
      "AP_PANEL_SPEED_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_VAR_SET_ENGLISH,
      "AP_ALT_VAR_SET_ENGLISH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_VS_VAR_SET_METRIC,
      "AP_VS_VAR_SET_METRIC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_SPEED_HOLD_TOGGLE,
      "AP_PANEL_SPEED_HOLD_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PANEL_MACH_HOLD_TOGGLE,
      "AP_PANEL_MACH_HOLD_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_NAV_SELECT_SET,
      "AP_NAV_SELECT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PITCH_REF_INC_UP,
      "AP_PITCH_REF_INC_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PITCH_REF_INC_DN,
      "AP_PITCH_REF_INC_DN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_PITCH_REF_SELECT,
      "AP_PITCH_REF_SELECT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ATT_HOLD, "AP_ATT_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_LOC_HOLD, "AP_LOC_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_APR_HOLD, "AP_APR_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_HDG_HOLD, "AP_HDG_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_ALT_HOLD, "AP_ALT_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_WING_LEVELER, "AP_WING_LEVELER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_BC_HOLD, "AP_BC_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_NAV1_HOLD, "AP_NAV1_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_HEADING_BUG_INC, "HEADING_BUG_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_HEADING_BUG_DEC, "HEADING_BUG_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FLIGHT_DIRECTOR,
      "TOGGLE_FLIGHT_DIRECTOR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_VS_HOLD, "AP_VS_HOLD");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_KOHLSMAN_INC, "KOHLSMAN_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_KOHLSMAN_DEC, "KOHLSMAN_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AUTO_THROTTLE_ARM,
      "AUTO_THROTTLE_ARM");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AUTO_THROTTLE_TO_GA,
      "AUTO_THROTTLE_TO_GA");

  // Coms
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_1_SET, "COM_STBY_RADIO_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_2_SET, "COM_STBY_RADIO_SET_2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM1_RADIO_WHOLE_DEC,
      "COM_RADIO_WHOLE_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM1_RADIO_WHOLE_INC,
      "COM_RADIO_WHOLE_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_WHOLE_DEC,
      "COM2_RADIO_WHOLE_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_WHOLE_INC,
      "COM2_RADIO_WHOLE_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_RADIO_FRACT_DEC,
      "COM_RADIO_FRACT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_RADIO_FRACT_INC,
      "COM_RADIO_FRACT_INC");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_FRACT_DEC,
      "COM2_RADIO_FRACT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_FRACT_INC,
      "COM2_RADIO_FRACT_INC");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_RADIO_FRACT_DEC_CARRY,
      "COM_RADIO_FRACT_DEC_CARRY");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_RADIO_FRACT_INC_CARRY,
      "COM_RADIO_FRACT_INC_CARRY");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_FRACT_DEC_CARRY,
      "COM2_RADIO_FRACT_DEC_CARRY");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM2_RADIO_FRACT_INC_CARRY,
      "COM2_RADIO_FRACT_INC_CARRY");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1, "NAV1_RADIO_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2, "NAV2_RADIO_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1_RADIO_FRACT_INC,
      "NAV1_RADIO_FRACT_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1_RADIO_FRACT_DEC,
      "NAV1_RADIO_FRACT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2_RADIO_FRACT_INC,
      "NAV2_RADIO_FRACT_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2_RADIO_FRACT_DEC,
      "NAV2_RADIO_FRACT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1_RADIO_WHOLE_INC,
      "NAV1_RADIO_WHOLE_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1_RADIO_WHOLE_DEC,
      "NAV1_RADIO_WHOLE_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2_RADIO_WHOLE_INC,
      "NAV2_RADIO_WHOLE_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2_RADIO_WHOLE_DEC,
      "NAV2_RADIO_WHOLE_DEC");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_STANDBY_SWAP,
      "COM1_RADIO_SWAP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COM_2_STANDBY_SWAP,
      "COM2_RADIO_SWAP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_1_STANDBY_SWAP,
      "NAV1_RADIO_SWAP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_NAV_2_STANDBY_SWAP,
      "NAV2_RADIO_SWAP");
  // G1000
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_ZOOMIN_BUTTON,
      "G1000_PFD_ZOOMIN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_ZOOMOUT_BUTTON,
      "G1000_PFD_ZOOMOUT_BUTTON");

  // DME
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME1_IDENT_ENABLE,
      "RADIO_SELECTED_DME1_IDENT_ENABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME1_IDENT_DISABLE,
      "RADIO_SELECTED_DME1_IDENT_DISABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME1_IDENT_SET,
      "RADIO_SELECTED_DME1_IDENT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DM1E_IDENT_TOGGLE,
      "RADIO_SELECTED_DM1E_IDENT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME2_IDENT_ENABLE,
      "RADIO_SELECTED_DME2_IDENT_ENABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME2_IDENT_DISABLE,
      "RADIO_SELECTED_DME2_IDENT_DISABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME2_IDENT_SET,
      "RADIO_SELECTED_DME2_IDENT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_DME2_IDENT_TOGGLE,
      "RADIO_SELECTED_DME2_IDENT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_DME, "TOGGLE_DME");

  // VOR
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR1_IDENT_ENABLE,
      "RADIO_SELECTED_VOR1_IDENT_ENABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR1_IDENT_DISABLE,
      "RADIO_SELECTED_VOR1_IDENT_DISABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR1_IDENT_SET,
      "RADIO_SELECTED_VOR1_IDENT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VORE_IDENT_TOGGLE,
      "RADIO_SELECTED_VORE_IDENT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR2_IDENT_ENABLE,
      "RADIO_SELECTED_VOR2_IDENT_ENABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR2_IDENT_DISABLE,
      "RADIO_SELECTED_VOR2_IDENT_DISABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR2_IDENT_SET,
      "RADIO_SELECTED_VOR2_IDENT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_VOR2_IDENT_TOGGLE,
      "RADIO_SELECTED_VOR2_IDENT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_VOR, "TOGGLE_VOR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_VOR1_OBI_DEC, "VOR1_OBI_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_VOR1_OBI_INC, "VOR1_OBI_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_VOR2_OBI_DEC, "VOR2_OBI_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_VOR2_OBI_INC, "VOR2_OBI_INC");

  // ADF
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_ADF_IDENT_ENABLE,
      "RADIO_SELECTED_ADF_IDENT_ENABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_ADF_IDENT_DISABLE,
      "RADIO_SELECTED_ADF_IDENT_DISABLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_ADF_IDENT_SET,
      "RADIO_SELECTED_ADF_IDENT_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_SELECTED_ADF_IDENT_TOGGLE,
      "RADIO_SELECTED_ADF_IDENT_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_CARD_INC, "ADF_CARD_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_CARD_DEC, "ADF_CARD_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_CARD_SET, "ADF_CARD_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_100_INC, "ADF_100_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_10_INC, "ADF_10_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_1_INC, "ADF_1_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_100_DEC, "ADF_100_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_10_DEC, "ADF_10_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_1_DEC, "ADF_1_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF_COMPLETE_SET,
      "ADF_COMPLETE_SET ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF1_WHOLE_INC, "ADF1_WHOLE_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF1_WHOLE_DEC, "ADF1_WHOLE_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_100_INC, "ADF2_100_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_10_INC, "ADF2_10_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_1_INC, "ADF2_1_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_RADIO_TENTHS_INC,
      "ADF2_RADIO_TENTHS_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_100_DEC, "ADF2_100_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_10_DEC, "ADF2_10_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_1_DEC, "ADF2_1_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_RADIO_TENTHS_DEC,
      "ADF2_RADIO_TENTHS_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_WHOLE_INC, "ADF2_WHOLE_INC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_WHOLE_DEC, "ADF2_WHOLE_DEC ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_FRACT_DEC_CARRY,
      "ADF2_FRACT_DEC_CARRY ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_FRACT_INC_CARRY,
      "ADF2_FRACT_INC_CARRY ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ADF2_COMPLETE_SET,
      "ADF2_COMPLETE_SET ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_ADF2_IDENT_DISABLE,
      "RADIO_ADF2_IDENT_DISABLE ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_ADF2_IDENT_ENABLE,
      "RADIO_ADF2_IDENT_ENABLE ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_ADF2_IDENT_TOGGLE,
      "RADIO_ADF2_IDENT_TOGGLE ");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RADIO_ADF2_IDENT_SET,
      "RADIO_ADF2_IDENT_SET ");

  // XPNDR
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_1000_INC, "XPNDR_1000_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_100_INC, "XPNDR_100_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_10_INC, "XPNDR_10_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_1_INC, "XPNDR_1_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_1000_DEC, "XPNDR_1000_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_100_DEC, "XPNDR_100_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_10_DEC, "XPNDR_10_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_1_DEC, "XPNDR_1_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_XPNDR_SET, "XPNDR_SET");

  // Flaps
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_FLAPS_SET, "AXIS_FLAPS_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_UP, "FLAPS_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_1, "FLAPS_1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_2, "FLAPS_2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_3, "FLAPS_3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_DOWN, "FLAPS_DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_INCR, "FLAPS_INCR");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FLAPS_DECR, "FLAPS_DECR");

  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ELEV_TRIM_DN, "ELEV_TRIM_DN");
  SimConnect_MapClientEventToSimEvent(mapConnect,
                                      definitionsMap.DEFINITION_DOWN, "DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ELEV_TRIM_UP, "ELEV_TRIM_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ELEV_UP, "ELEV_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_ARM_TOGGLE,
      "SPOILERS_ARM_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_ON, "SPOILERS_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_OFF, "SPOILERS_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_ARM_ON, "SPOILERS_ARM_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_ARM_OFF,
      "SPOILERS_ARM_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AILERON_TRIM_LEFT,
      "AILERON_TRIM_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AILERON_TRIM_RIGHT,
      "AILERON_TRIM_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RUDDER_TRIM_LEFT,
      "RUDDER_TRIM_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_RUDDER_TRIM_RIGHT,
      "RUDDER_TRIM_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COWLFLAP1_SET, "COWLFLAP1_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COWLFLAP2_SET, "COWLFLAP2_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COWLFLAP3_SET, "COWLFLAP3_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_COWLFLAP4_SET, "COWLFLAP4_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INC_COWL_FLAPS, "INC_COWL_FLAPS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DEC_COWL_FLAPS, "DEC_COWL_FLAPS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INC_COWL_FLAPS1, "INC_COWL_FLAPS1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DEC_COWL_FLAPS1, "DEC_COWL_FLAPS1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INC_COWL_FLAPS2, "INC_COWL_FLAPS2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DEC_COWL_FLAPS2, "DEC_COWL_FLAPS2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INC_COWL_FLAPS3, "INC_COWL_FLAPS3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DEC_COWL_FLAPS3, "DEC_COWL_FLAPS3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INC_COWL_FLAPS4, "INC_COWL_FLAPS4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DEC_COWL_FLAPS4, "DEC_COWL_FLAPS4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_PUMP, "FUEL_PUMP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ELECT_FUEL_PUMP,
      "TOGGLE_ELECT_FUEL_PUMP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ELECT_FUEL_PUMP1,
      "TOGGLE_ELECT_FUEL_PUMP1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ELECT_FUEL_PUMP2,
      "TOGGLE_ELECT_FUEL_PUMP2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ELECT_FUEL_PUMP3,
      "TOGGLE_ELECT_FUEL_PUMP3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ELECT_FUEL_PUMP4,
      "TOGGLE_ELECT_FUEL_PUMP4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ENGINE_PRIMER, "ENGINE_PRIMER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PRIMER, "TOGGLE_PRIMER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PRIMER1, "TOGGLE_PRIMER1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PRIMER2, "TOGGLE_PRIMER2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PRIMER3, "TOGGLE_PRIMER3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PRIMER4, "TOGGLE_PRIMER4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FEATHER_SWITCHES,
      "TOGGLE_FEATHER_SWITCHES");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FEATHER_SWITCH_1,
      "TOGGLE_FEATHER_SWITCH_1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FEATHER_SWITCH_2,
      "TOGGLE_FEATHER_SWITCH_2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FEATHER_SWITCH_3,
      "TOGGLE_FEATHER_SWITCH_3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_FEATHER_SWITCH_4,
      "TOGGLE_FEATHER_SWITCH_4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PROPELLER_SYNC,
      "TOGGLE_PROPELLER_SYNC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AUTOFEATHER_ARM,
      "TOGGLE_AUTOFEATHER_ARM");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AFTERBURNER,
      "TOGGLE_AFTERBURNER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AFTERBURNER1,
      "TOGGLE_AFTERBURNER1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AFTERBURNER2,
      "TOGGLE_AFTERBURNER2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AFTERBURNER3,
      "TOGGLE_AFTERBURNER3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_AFTERBURNER4,
      "TOGGLE_AFTERBURNER4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ENGINE, "ENGINE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_SPOILERS_TOGGLE, "SPOILERS_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_STARTER1, "TOGGLE_STARTER1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_STARTER2, "TOGGLE_STARTER2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_STARTER3, "TOGGLE_STARTER3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_STARTER4, "TOGGLE_STARTER4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ALL_STARTERS,
      "TOGGLE_ALL_STARTERS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ENGINE_AUTO_START,
      "ENGINE_AUTO_START");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ENGINE_AUTO_SHUTDOWN,
      "ENGINE_AUTO_SHUTDOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_LEFT_MAIN,
      "FUEL_SELECTOR_LEFT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_LEFT_MAIN,
      "FUEL_SELECTOR_2_LEFT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_LEFT_MAIN,
      "FUEL_SELECTOR_3_LEFT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_LEFT_MAIN,
      "FUEL_SELECTOR_4_LEFT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_RIGHT_MAIN,
      "FUEL_SELECTOR_RIGHT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_2_RIGHT_MAIN,
      "FUEL_SELECTOR_2_RIGHT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_3_RIGHT_MAIN,
      "FUEL_SELECTOR_3_RIGHT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_FUEL_SELECTOR_4_RIGHT_MAIN,
      "FUEL_SELECTOR_4_RIGHT_MAIN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_APU_STARTER, "APU_STARTER");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_APU_OFF_SWITCH, "APU_OFF_SWITCH");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_APU_GENERATOR_SWITCH_TOGGLE,
      "APU_GENERATOR_SWITCH_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect,
      definitionsMap.DEFINITION_CABIN_NO_SMOKING_ALERT_SWITCH_TOGGLE,
      "CABIN_NO_SMOKING_ALERT_SWITCH_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE,
      "CABIN_SEATBELTS_ALERT_SWITCH_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_STROBES_TOGGLE, "STROBES_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ALL_LIGHTS_TOGGLE,
      "ALL_LIGHTS_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PANEL_LIGHTS_TOGGLE,
      "PANEL_LIGHTS_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHTS_TOGGLE,
      "LANDING_LIGHTS_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHT_UP,
      "LANDING_LIGHT_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHT_DOWN,
      "LANDING_LIGHT_DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHT_LEFT,
      "LANDING_LIGHT_LEFT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHT_RIGHT,
      "LANDING_LIGHT_RIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHT_HOME,
      "LANDING_LIGHT_HOME");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_STROBES_ON, "STROBES_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_STROBES_OFF, "STROBES_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PANEL_LIGHTS_ON, "PANEL_LIGHTS_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_PANEL_LIGHTS_OFF,
      "PANEL_LIGHTS_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHTS_ON,
      "LANDING_LIGHTS_ON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_LANDING_LIGHTS_OFF,
      "LANDING_LIGHTS_OFF");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_BEACON_LIGHTS,
      "TOGGLE_BEACON_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_TAXI_LIGHTS,
      "TOGGLE_TAXI_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_LOGO_LIGHTS,
      "TOGGLE_LOGO_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_RECOGNITION_LIGHTS,
      "TOGGLE_RECOGNITION_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_WING_LIGHTS,
      "TOGGLE_WING_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_NAV_LIGHTS,
      "TOGGLE_NAV_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_CABIN_LIGHTS,
      "TOGGLE_CABIN_LIGHTS");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GEAR_UP, "GEAR_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GEAR_DOWN, "GEAR_DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_STRUCTURAL_DEICE,
      "TOGGLE_STRUCTURAL_DEICE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_PROPELLER_DEICE,
      "TOGGLE_PROPELLER_DEICE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ALTERNATOR1,
      "TOGGLE_ALTERNATOR1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ALTERNATOR2,
      "TOGGLE_ALTERNATOR2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ALTERNATOR3,
      "TOGGLE_ALTERNATOR3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_ALTERNATOR4,
      "TOGGLE_ALTERNATOR4");
  SimConnect_MapClientEventToSimEvent(mapConnect, definitionsMap.DEFINITION_ATC,
                                      "ATC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_1, "ATC_MENU_1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_2, "ATC_MENU_2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_3, "ATC_MENU_3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_4, "ATC_MENU_4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_5, "ATC_MENU_5");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_6, "ATC_MENU_6");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_7, "ATC_MENU_7");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_8, "ATC_MENU_8");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_9, "ATC_MENU_9");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ATC_MENU_0, "ATC_MENU_0");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT1_INC, "EGT1_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT1_DEC, "EGT1_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT2_INC, "EGT2_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT2_DEC, "EGT2_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT3_INC, "EGT3_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT3_DEC, "EGT3_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT4_INC, "EGT4_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT4_DEC, "EGT4_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT_INC, "EGT_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_EGT_DEC, "EGT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TOGGLE_GPS_DRIVES_NAV1,
      "TOGGLE_GPS_DRIVES_NAV1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_POWER_BUTTON,
      "GPS_POWER_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_NEAREST_BUTTON,
      "GPS_NEAREST_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_OBS_BUTTON, "GPS_OBS_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_MSG_BUTTON, "GPS_MSG_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_MSG_BUTTON_DOWN,
      "GPS_MSG_BUTTON_DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_MSG_BUTTON_UP,
      "GPS_MSG_BUTTON_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_FLIGHTPLAN_BUTTON,
      "GPS_FLIGHTPLAN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_TERRAIN_BUTTON,
      "GPS_TERRAIN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_PROCEDURE_BUTTON,
      "GPS_PROCEDURE_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_ZOOMIN_BUTTON,
      "GPS_ZOOMIN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_ZOOMOUT_BUTTON,
      "GPS_ZOOMOUT_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_DIRECTTO_BUTTON,
      "GPS_DIRECTTO_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_MENU_BUTTON, "GPS_MENU_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_CLEAR_BUTTON,
      "GPS_CLEAR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_CLEAR_ALL_BUTTON,
      "GPS_CLEAR_ALL_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_CLEAR_BUTTON_DOWN,
      "GPS_CLEAR_BUTTON_DOWN");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_CLEAR_BUTTON_UP,
      "GPS_CLEAR_BUTTON_UP");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_ENTER_BUTTON,
      "GPS_ENTER_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_CURSOR_BUTTON,
      "GPS_CURSOR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_GROUP_KNOB_INC,
      "GPS_GROUP_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_GROUP_KNOB_DEC,
      "GPS_GROUP_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_PAGE_KNOB_INC,
      "GPS_PAGE_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GPS_PAGE_KNOB_DEC,
      "GPS_PAGE_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_YAW_DAMPER_TOGGLE,
      "YAW_DAMPER_TOGGLE");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_FLIGHTPLAN_BUTTON,
      "G1000_PFD_FLIGHTPLAN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_PROCEDURE_BUTTON,
      "G1000_PFD_PROCEDURE_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_ZOOMIN_BUTTON,
      "G1000_PFD_ZOOMIN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_ZOOMOUT_BUTTON,
      "G1000_PFD_ZOOMOUT_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_DIRECTTO_BUTTON,
      "G1000_PFD_DIRECTTO_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_MENU_BUTTON,
      "G1000_PFD_MENU_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_CLEAR_BUTTON,
      "G1000_PFD_CLEAR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_ENTER_BUTTON,
      "G1000_PFD_ENTER_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_CURSOR_BUTTON,
      "G1000_PFD_CURSOR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_GROUP_KNOB_INC,
      "G1000_PFD_GROUP_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_GROUP_KNOB_DEC,
      "G1000_PFD_GROUP_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_PAGE_KNOB_INC,
      "G1000_PFD_PAGE_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_PAGE_KNOB_DEC,
      "G1000_PFD_PAGE_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY1,
      "G1000_PFD_SOFTKEY1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY2,
      "G1000_PFD_SOFTKEY2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY3,
      "G1000_PFD_SOFTKEY3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY4,
      "G1000_PFD_SOFTKEY4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY5,
      "G1000_PFD_SOFTKEY5");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY6,
      "G1000_PFD_SOFTKEY6");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY7,
      "G1000_PFD_SOFTKEY7");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY8,
      "G1000_PFD_SOFTKEY8");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY9,
      "G1000_PFD_SOFTKEY9");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY10,
      "G1000_PFD_SOFTKEY10");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY11,
      "G1000_PFD_SOFTKEY11");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_PFD_SOFTKEY12,
      "G1000_PFD_SOFTKEY12");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_FLIGHTPLAN_BUTTON,
      "G1000_MFD_FLIGHTPLAN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_PROCEDURE_BUTTON,
      "G1000_MFD_PROCEDURE_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_ZOOMIN_BUTTON,
      "G1000_MFD_ZOOMIN_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_ZOOMOUT_BUTTON,
      "G1000_MFD_ZOOMOUT_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_DIRECTTO_BUTTON,
      "G1000_MFD_DIRECTTO_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_MENU_BUTTON,
      "G1000_MFD_MENU_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_CLEAR_BUTTON,
      "G1000_MFD_CLEAR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_ENTER_BUTTON,
      "G1000_MFD_ENTER_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_CURSOR_BUTTON,
      "G1000_MFD_CURSOR_BUTTON");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_GROUP_KNOB_INC,
      "G1000_MFD_GROUP_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_GROUP_KNOB_DEC,
      "G1000_MFD_GROUP_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_PAGE_KNOB_INC,
      "G1000_MFD_PAGE_KNOB_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_PAGE_KNOB_DEC,
      "G1000_MFD_PAGE_KNOB_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY1,
      "G1000_MFD_SOFTKEY1");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY2,
      "G1000_MFD_SOFTKEY2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY3,
      "G1000_MFD_SOFTKEY3");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY4,
      "G1000_MFD_SOFTKEY4");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY5,
      "G1000_MFD_SOFTKEY5");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY6,
      "G1000_MFD_SOFTKEY6");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY7,
      "G1000_MFD_SOFTKEY7");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY8,
      "G1000_MFD_SOFTKEY8");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY9,
      "G1000_MFD_SOFTKEY9");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY10,
      "G1000_MFD_SOFTKEY10");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY11,
      "G1000_MFD_SOFTKEY11");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_G1000_MFD_SOFTKEY2,
      "G1000_MFD_SOFTKEY2");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MAX_BANK_INC, "AP_MAX_BANK_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AP_MAX_BANK_DEC, "AP_MAX_BANK_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GYRO_DRIFT_INC, "GYRO_DRIFT_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_GYRO_DRIFT_DEC, "GYRO_DRIFT_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TRUE_AIRSPEED_CAL_INC,
      "TRUE_AIRSPEED_CAL_INC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_TRUE_AIRSPEED_CAL_DEC,
      "TRUE_AIRSPEED_CAL_DEC");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_DECREASE_DECISION_HEIGHT,
      "DECREASE_DECISION_HEIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_INCREASE_DECISION_HEIGHT,
      "INCREASE_DECISION_HEIGHT");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_ELEVATOR_TRIM_SET,
      "ELEVATOR_TRIM_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_RUDDER_SET, "AXIS_RUDDER_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_LEFT_BRAKE_SET,
      "AXIS_LEFT_BRAKE_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_RIGHT_BRAKE_SET,
      "AXIS_RIGHT_BRAKE_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_ELEVATOR_SET,
      "AXIS_ELEVATOR_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_AILERONS_SET,
      "AXIS_AILERONS_SET");
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_CRJ_SMOKE_ON, "LVAR_ACCESS.EFIS");
}
