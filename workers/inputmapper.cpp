#include "InputMapper.h"

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

  // Flaps
  SimConnect_MapClientEventToSimEvent(
      mapConnect, definitionsMap.DEFINITION_AXIS_FLAPS_SET, "AXIS_FLAPS_SET");

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
    SimConnect_MapClientEventToSimEvent(mapConnect, definitionsMap.DEFINITION_ELEVATOR_TRIM_SET, "ELEVATOR_TRIM_SET");

}
