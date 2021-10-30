/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * feature/pause.cpp - Pause feature support functions
 * This may be combined with related G-codes if features are consolidated.
 */

#include "../inc/MarlinConfigPre.h"

#if ENABLED(ADVANCED_PAUSE_FEATURE)

//#define DEBUG_PAUSE_RESUME

#include "../MarlinCore.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../module/stepper.h"
#include "../module/printcounter.h"
#include "../module/temperature.h"

#if ENABLED(FWRETRACT)
  #include "fwretract.h"
#endif

#if HAS_FILAMENT_SENSOR
  #include "runout.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "host_actions.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

#include "Tronxy.h"

#if HAS_BUZZER
  #include "../libs/buzzer.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "powerloss.h"
#endif

#include "../libs/nozzle.h"
#include "pause.h"

#define DEBUG_OUT ENABLED(DEBUG_PAUSE_RESUME)
#include "../core/debug_out.h"

// private:
static xyze_pos_t resume_position;
static uint32_t resume_sdpos;

#if HAS_LCD_MENU || PROJ_MKS_ROBIN_PRO
  PauseMenuResponse pause_menu_response;
  PauseMode pause_mode = PAUSE_MODE_PAUSE_PRINT;
#endif

fil_change_settings_t fc_settings[EXTRUDERS];

#if ENABLED(SDSUPPORT)
  #include "../sd/cardreader.h"
#endif

#if ENABLED(EMERGENCY_PARSER)
  #define _PMSG(L) L##_M108
#else
  #define _PMSG(L) L##_LCD
#endif

#if HAS_BUZZER
  static void impatient_beep(const int8_t max_beep_count, const bool restart=false) {

    if (TERN0(HAS_LCD_MENU, pause_mode == PAUSE_MODE_PAUSE_PRINT)) return;

    static millis_t next_buzz = 0;
    static int8_t runout_beep = 0;

    if (restart) next_buzz = runout_beep = 0;

    const bool always = max_beep_count < 0;

    const millis_t ms = millis();
    if (ELAPSED(ms, next_buzz)) {
      if (always || runout_beep < max_beep_count + 5) { // Only beep as long as we're supposed to
        next_buzz = ms + ((always || runout_beep < max_beep_count) ? 1000 : 500);
        BUZZ(50, 880 - (runout_beep & 1) * 220);
        runout_beep++;
      }
    }
  }
  inline void first_impatient_beep(const int8_t max_beep_count) { impatient_beep(max_beep_count, true); }
#else
  inline void impatient_beep(const int8_t, const bool=false) {}
  inline void first_impatient_beep(const int8_t) {}
#endif

/**
 * Ensure a safe temperature for extrusion
 *
 * - Fail if the TARGET temperature is too low
 * - Display LCD placard with temperature status
 * - Return when heating is done or aborted
 *
 * Returns 'true' if heating was completed, 'false' for abort
 */
static bool ensure_safe_temperature(const bool wait=true, const PauseMode mode=PAUSE_MODE_SAME) {
  DEBUG_SECTION(est, "ensure_safe_temperature", true);
  DEBUG_ECHOLNPAIR("... wait:", int(wait), " mode:", int(mode));

  if(globalStop)return false;

  if (!DEBUGGING(DRYRUN) && thermalManager.targetTooColdToExtrude(active_extruder))
    thermalManager.setTargetHotend(thermalManager.extrude_min_temp, active_extruder);

  if(ABS(thermalManager.degHotend(active_extruder) - thermalManager.degTargetHotend(active_extruder)) <= TEMP_WINDOW)return true;
  screenPopIndex(POP_WAITNOZZLE_HEAT);
  UNUSED(mode);

  if (wait)
    return thermalManager.wait_for_hotend(active_extruder);

  wait_for_heatup = true; // Allow interruption by Emergency Parser M108
  while (wait_for_heatup && ABS(thermalManager.degHotend(active_extruder) - thermalManager.degTargetHotend(active_extruder)) > TEMP_WINDOW && globalStop == 0)
    idle();
  wait_for_heatup = false;

  if(messageIndexNow == POP_WAITNOZZLE_HEAT)screenCloseMessageBox();
  if(globalStop)return false;

  #if ENABLED(PREVENT_COLD_EXTRUSION)
    // A user can cancel wait-for-heating with M108
    if (!DEBUGGING(DRYRUN) && thermalManager.targetTooColdToExtrude(active_extruder)) {
      SERIAL_ECHO_MSG(STR_ERR_HOTEND_TOO_COLD);
      return false;
    }
  #endif

  return true;
}

/**
 * Load filament into the hotend
 *
 * - Fail if the a safe temperature was not reached
 * - If pausing for confirmation, wait for a click or M108
 * - Show "wait for load" placard
 * - Load and purge filament
 * - Show "Purge more" / "Continue" menu
 * - Return when "Continue" is selected
 *
 * Returns 'true' if load was completed, 'false' for abort
 */
bool load_filament(const float &slow_load_length/*=0*/, const float &fast_load_length/*=0*/, const float &purge_length/*=0*/, const int8_t max_beep_count/*=0*/,
                   const bool show_lcd/*=false*/, const bool pause_for_user/*=false*/,
                   const PauseMode mode/*=PAUSE_MODE_PAUSE_PRINT*/
                   DXC_ARGS
) {
  DEBUG_SECTION(lf, "load_filament", true);
  DEBUG_ECHOLNPAIR("... slowlen:", slow_load_length, " fastlen:", fast_load_length, " purgelen:", purge_length, " maxbeep:", int(max_beep_count), " showlcd:", int(show_lcd), " pauseforuser:", int(pause_for_user), " pausemode:", int(mode) DXC_SAY);

  UNUSED(show_lcd);

  if (!ensure_safe_temperature(false, mode)) {
    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_STATUS, mode);
    #endif
    return false;
  }

  if (pause_for_user) {
    #if HAS_LCD_MENU
    if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_INSERT, mode);
    #endif
    SERIAL_ECHO_MSG(_PMSG(STR_FILAMENT_CHANGE_INSERT));

    first_impatient_beep(max_beep_count);

    KEEPALIVE_STATE(PAUSED_FOR_USER);
    #if ENABLED(HOST_PROMPT_SUPPORT)
      const char tool = '0'
        #if NUM_RUNOUT_SENSORS > 1
          + active_extruder
        #endif
      ;
      host_action_prompt_begin(PROMPT_USER_CONTINUE, PSTR("Load Filament T"), tool);
      host_action_prompt_button(CONTINUE_STR);
      host_action_prompt_show();
    #endif
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Load Filament")));
    while (wait_for_user && globalStop == 0) {
      impatient_beep(max_beep_count);
      idle_no_sleep();
    }
    wait_for_user = false;//补充,以防wait_for_user一直为true
  }

  #if HAS_LCD_MENU
    if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_LOAD, mode);
  #endif

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    active_extruder = DXC_ext;
    extruder_duplication_enabled = false;
  #endif

  // Slow Load filament
  if (slow_load_length) unscaled_e_move(slow_load_length, FILAMENT_CHANGE_SLOW_LOAD_FEEDRATE);

  // Fast Load Filament
  if (fast_load_length) {
    #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
      const float saved_acceleration = planner.settings.retract_acceleration;
      planner.settings.retract_acceleration = FILAMENT_CHANGE_FAST_LOAD_ACCEL;
    #endif

    unscaled_e_move(fast_load_length, FILAMENT_CHANGE_FAST_LOAD_FEEDRATE);

    #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
      planner.settings.retract_acceleration = saved_acceleration;
    #endif
  }

  #if ENABLED(DUAL_X_CARRIAGE)      // Tie the two extruders movement back together.
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif

  #if ENABLED(ADVANCED_PAUSE_CONTINUOUS_PURGE)

    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_PURGE);
    #endif

    TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, PSTR("Filament Purging..."), CONTINUE_STR));
    TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(PSTR("Filament Purging...")));
    wait_for_user = true; // A click or M108 breaks the purge_length loop
    for (float purge_count = purge_length; purge_count > 0 && wait_for_user; --purge_count)
      unscaled_e_move(1, ADVANCED_PAUSE_PURGE_FEEDRATE);
    wait_for_user = false;

  #else

    do {
      if (purge_length > 0) {
        // "Wait for filament purge"
        #if HAS_LCD_MENU
        if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_PURGE);
        #endif
        // Extrude filament to get into hotend
        unscaled_e_move(purge_length, ADVANCED_PAUSE_PURGE_FEEDRATE);
      }

      TERN_(HOST_PROMPT_SUPPORT, filament_load_host_prompt()); // Initiate another host prompt. (NOTE: host_response_handler may also do this!)
      if (show_lcd) {
        // Show "Purge More" / "Resume" menu and wait for reply
        KEEPALIVE_STATE(PAUSED_FOR_USER);
        wait_for_user = false;
        #if HAS_LCD_MENU
        lcd_pause_show_message(PAUSE_MESSAGE_OPTION);
        while (pause_menu_response == PAUSE_RESPONSE_WAIT_FOR) idle_no_sleep();
        #endif
      }

      // Keep looping if "Purge More" was selected
    } while (TERN0(HAS_LCD_MENU, show_lcd && pause_menu_response == PAUSE_RESPONSE_EXTRUDE_MORE));

  #endif
  TERN_(HOST_PROMPT_SUPPORT, host_action_prompt_end());

  return true;
}

/**
 * Unload filament from the hotend
 *
 * - Fail if the a safe temperature was not reached
 * - Show "wait for unload" placard
 * - Retract, pause, then unload filament
 * - Disable E stepper (on most machines)
 *
 * Returns 'true' if unload was completed, 'false' for abort
 */
bool unload_filament(const float &unload_length, const bool show_lcd/*=false*/,
                     const PauseMode mode/*=PAUSE_MODE_PAUSE_PRINT*/
                     #if BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
                       , const float &mix_multiplier/*=1.0*/
                     #endif
) {
  DEBUG_SECTION(uf, "unload_filament", true);
  DEBUG_ECHOLNPAIR("... unloadlen:", unload_length, " showlcd:", int(show_lcd), " mode:", int(mode)
    #if BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
      , " mixmult:", mix_multiplier
    #endif
  );

  UNUSED(show_lcd);

  #if !BOTH(FILAMENT_UNLOAD_ALL_EXTRUDERS, MIXING_EXTRUDER)
    constexpr float mix_multiplier = 1.0;
  #endif

  if (!ensure_safe_temperature(false, mode)) {
    #if HAS_LCD_MENU
      if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_STATUS);
    #endif
    return false;
  }

  #if HAS_LCD_MENU
    if (show_lcd) lcd_pause_show_message(PAUSE_MESSAGE_UNLOAD, mode);
  #endif

  // Retract filament
  unscaled_e_move(-(FILAMENT_UNLOAD_PURGE_RETRACT) * mix_multiplier, (PAUSE_PARK_RETRACT_FEEDRATE) * mix_multiplier);

  // Wait for filament to cool
  safe_delay(FILAMENT_UNLOAD_PURGE_DELAY);

  // Quickly purge
  unscaled_e_move((FILAMENT_UNLOAD_PURGE_RETRACT + FILAMENT_UNLOAD_PURGE_LENGTH) * mix_multiplier,
                  (FILAMENT_UNLOAD_PURGE_FEEDRATE) * mix_multiplier);

  // Unload filament
  #if FILAMENT_CHANGE_UNLOAD_ACCEL > 0
    const float saved_acceleration = planner.settings.retract_acceleration;
    planner.settings.retract_acceleration = FILAMENT_CHANGE_UNLOAD_ACCEL;
  #endif

  unscaled_e_move(unload_length * mix_multiplier, (FILAMENT_CHANGE_UNLOAD_FEEDRATE) * mix_multiplier);

  #if FILAMENT_CHANGE_FAST_LOAD_ACCEL > 0
    planner.settings.retract_acceleration = saved_acceleration;
  #endif

  // Disable E steppers for manual change
  #if HAS_E_STEPPER_ENABLE
    disable_e_stepper(active_extruder);
    safe_delay(100);
  #endif

  return true;
}

// public:

/**
 * Pause procedure
 *
 * - Abort if already paused
 * - Send host action for pause, if configured
 * - Abort if TARGET temperature is too low
 * - Display "wait for start of filament change" (if a length was specified)
 * - Initial retract, if current temperature is hot enough
 * - Park the nozzle at the given position
 * - Call unload_filament (if a length was specified)
 *
 * Return 'true' if pause was completed, 'false' for abort
 */
uint8_t did_pause_print = 0;

bool pause_print(const float &retract, const xyz_pos_t &park_point, const float &unload_length/*=0*/, const bool show_lcd/*=false*/ DXC_ARGS) {
  DEBUG_SECTION(pp, "pause_print", true);
  DEBUG_ECHOLNPAIR("... park.x:", park_point.x, " y:", park_point.y, " z:", park_point.z, " unloadlen:", unload_length, " showlcd:", int(show_lcd) DXC_SAY);
  
  UNUSED(show_lcd);
  if (did_pause_print) return false; // already paused
  #if ENABLED(HOST_ACTION_COMMANDS)
    #ifdef ACTION_ON_PAUSED
      host_action_paused();
    #elif defined(ACTION_ON_PAUSE)
      host_action_pause();
    #endif
  #endif

  TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("Pause"), DISMISS_STR));

  // Indicate that the printer is paused
  ++did_pause_print;
  #if TRONXY_PRINTING_NOWAIT
  printing_no_wait = false;
  #endif

  // Pause the print job and timer
  #if ENABLED(SDSUPPORT)
    if (IS_SD_PRINTING()) {
      card.pauseSDPrint();
      ++did_pause_print; // Indicate SD pause also
    }
  #endif

  print_job_timer.pause();
  resume_sdpos = PrintJobRecovery::info.sdpos;
  quickstop_stepper();
  resume_position = current_position;
  TERN_(POWER_LOSS_RECOVERY, PrintJobRecovery::save(true,resume_position.z + 10));//保存当前位置,以及高度

  
  #if ENABLED(ADVANCED_PAUSE_FANS_PAUSE) && HAS_FAN
    thermalManager.set_fans_paused(true);
  #endif

  if(thermalManager.hotEnoughToExtrude(active_extruder))
  unscaled_e_move(-(PAUSE_PARK_RETRACT_LENGTH),feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    extruder_duplication_enabled = false;
  #endif
  (void)unload_length;
  do_blocking_move_to_z(PrintJobRecovery::info.zraise);
  gcode.process_subcommands_now_P(PSTR("G28R0 XY"));//归0后,Z值会变0
  #if ENABLED(DUAL_X_CARRIAGE)
  if(dual_x_carriage_mode == DXC_AUTO_PARK_MODE)active_extruder_parked = false;//因为G28后,双头这里会变成true,移回XY后,会执行park动作,暂停不需要这个动作
  #endif
  current_position.z = PrintJobRecovery::info.zraise;
  sync_plan_position();

  #if ENABLED(DUAL_X_CARRIAGE)
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif
  return true;
}

/**
 * For Paused Print:
 * - Show "Press button (or M108) to resume"
 *
 * For Filament Change:
 * - Show "Insert filament and press button to continue"
 *
 * - Wait for a click before returning
 * - Heaters can time out and must reheat before continuing
 *
 * Used by M125 and M600
 */

void show_continue_prompt(const bool is_reload) {
  DEBUG_SECTION(scp, "pause_print", true);
  DEBUG_ECHOLNPAIR("... is_reload:", int(is_reload));
  if(is_reload)screenPopIndex(POP_FILAMENT_INSERT);
  SERIAL_ECHO_START();
  serialprintPGM(is_reload ? PSTR(_PMSG(STR_FILAMENT_CHANGE_INSERT) "\n") : PSTR(_PMSG(STR_FILAMENT_CHANGE_WAIT) "\n"));
}

void wait_for_confirmation(const bool is_reload/*=false*/, const int8_t max_beep_count/*=0*/ DXC_ARGS) {
  DEBUG_SECTION(wfc, "wait_for_confirmation", true);
  DEBUG_ECHOLNPAIR("... is_reload:", is_reload, " maxbeep:", int(max_beep_count) DXC_SAY);

  show_continue_prompt(is_reload);

  first_impatient_beep(max_beep_count);

  // Start the heater idle timers
  const millis_t nozzle_timeout = SEC_TO_MS(PAUSE_PARK_NOZZLE_TIMEOUT);

  HOTEND_LOOP() thermalManager.heater_idle[e].start(nozzle_timeout);

  #if ENABLED(DUAL_X_CARRIAGE)
    const int8_t saved_ext        = active_extruder;
    const bool saved_ext_dup_mode = extruder_duplication_enabled;
    active_extruder = DXC_ext;
    extruder_duplication_enabled = false;
  #endif

  // Wait for filament insert by user and press button
  KEEPALIVE_STATE(PAUSED_FOR_USER);
  TERN_(HOST_PROMPT_SUPPORT, host_prompt_do(PROMPT_USER_CONTINUE, GET_TEXT(MSG_NOZZLE_PARKED), CONTINUE_STR));
  TERN_(EXTENSIBLE_UI, ExtUI::onUserConfirmRequired_P(GET_TEXT(MSG_NOZZLE_PARKED)));
  wait_for_user = true;    // LCD click or M108 will clear this
  while (wait_for_user && globalStop == 0) {
    impatient_beep(max_beep_count);
    idle_no_sleep();
  }
  wait_for_user = false;//补充,以防wait_for_user一直为true
  #if ENABLED(DUAL_X_CARRIAGE)
    active_extruder = saved_ext;
    extruder_duplication_enabled = saved_ext_dup_mode;
    stepper.set_directions();
  #endif
}

/**
 * Resume or Start print procedure
 *
 * - If not paused, do nothing and return
 * - Reset heater idle timers
 * - Load filament if specified, but only if:
 *   - a nozzle timed out, or
 *   - the nozzle is already heated.
 * - Display "wait for print to resume"
 * - Retract to prevent oozing
 * - Move the nozzle back to resume_position
 * - Unretract
 * - Re-prime the nozzle...
 *   -  FWRETRACT: Recover/prime from the prior G10.
 *   - !FWRETRACT: Retract by resume_position.e, if negative.
 *                 Not sure how this logic comes into use.
 * - Sync the planner E to resume_position.e
 * - Send host action for resume, if configured
 * - Resume the current SD print job, if any
 */
void resume_print(const float &slow_load_length/*=0*/, const float &fast_load_length/*=0*/, const float &purge_length/*=ADVANCED_PAUSE_PURGE_LENGTH*/, const int8_t max_beep_count/*=0*/, int16_t targetTemp/*=0*/ DXC_ARGS) {
  DEBUG_SECTION(rp, "resume_print", true);
  DEBUG_ECHOLNPAIR("... slowlen:", slow_load_length, " fastlen:", fast_load_length, " purgelen:", purge_length, " maxbeep:", int(max_beep_count), " targetTemp:", targetTemp DXC_SAY);

  /*
  SERIAL_ECHOLNPAIR(
    "start of resume_print()\ndual_x_carriage_mode:", dual_x_carriage_mode,
    "\nextruder_duplication_enabled:", extruder_duplication_enabled,
    "\nactive_extruder:", active_extruder,
    "\n"
  );
  //*/

  if (!did_pause_print) return;
  if(globalStop > 0){did_pause_print = 0; return;}

  HOTEND_LOOP()thermalManager.reset_hotend_idle_timer(e);

  if (targetTemp > thermalManager.degTargetHotend(active_extruder)) {
    thermalManager.setTargetHotend(targetTemp, active_extruder);
  }

  if(!ensure_safe_temperature(false))return;
  (void)slow_load_length;
  (void)fast_load_length;
  (void)purge_length;
  (void)max_beep_count;

  if (targetTemp > 0) {
    thermalManager.setTargetHotend(targetTemp, active_extruder);
    thermalManager.wait_for_hotend(active_extruder, false);
  }

  TERN_(HAS_LCD_MENU, lcd_pause_show_message(PAUSE_MESSAGE_RESUME));
  // Check Temperature before moving hotend
  if(!ensure_safe_temperature(false))return;

  // Retract to prevent oozing
  if (!axes_should_home()) {
    char cmd[MAX_CMD_SIZE+16], str_1[16], str_2[16];
    // Move XY to starting position, then Z
    sprintf_P(cmd, PSTR("G1 X%s Y%s F4800"),
      dtostrf(resume_position.x, 1, 3, str_1),
      dtostrf(resume_position.y, 1, 3, str_2)
    );
    #if ENABLED(DUAL_X_CARRIAGE)
    extruder_duplication_enabled = false;//这里必须置false,否则复制模式在执行dual_x_carriage_unpark()时,头一也会动
    #endif
    gcode.process_subcommands_now(cmd);
    // Move Z_AXIS to saved position
    do_blocking_move_to_z(resume_position.z, feedRate_t(NOZZLE_PARK_Z_FEEDRATE));
  }

  // Unretract
  unscaled_e_move(PAUSE_PARK_RETRACT_LENGTH, feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));
  // Intelligent resuming
  #if ENABLED(FWRETRACT)
    // If retracted before goto pause
    if (fwretract.retracted[active_extruder])
      unscaled_e_move(-fwretract.settings.retract_length, fwretract.settings.retract_feedrate_mm_s);
  #endif

  // If resume_position is negative
  if (resume_position.e < 0)unscaled_e_move(resume_position.e, feedRate_t(PAUSE_PARK_RETRACT_FEEDRATE));
  #if ADVANCED_PAUSE_RESUME_PRIME != 0
    unscaled_e_move(ADVANCED_PAUSE_RESUME_PRIME, feedRate_t(ADVANCED_PAUSE_PURGE_FEEDRATE));
  #endif

  // Now all extrusion positions are resumed and ready to be confirmed
  // Set extruder to saved position
  planner.set_e_position_mm((destination.e = current_position.e = resume_position.e));

  // Write PLR now to update the z axis value
  card.setIndex(resume_sdpos);
  my_print_status = PRINT_RUNNING;
  feedrate_mm_s = PrintJobRecovery::info.feedrate;
  queue.clear();

  TERN_(HAS_LCD_MENU, lcd_pause_show_message(PAUSE_MESSAGE_STATUS));

  #ifdef ACTION_ON_RESUMED
    host_action_resumed();
  #elif defined(ACTION_ON_RESUME)
    host_action_resume();
  #endif

  --did_pause_print;

  TERN_(HOST_PROMPT_SUPPORT, host_prompt_open(PROMPT_INFO, PSTR("Resuming"), DISMISS_STR));

  #if ENABLED(SDSUPPORT)
    if (did_pause_print) { card.startFileprint(); --did_pause_print; }
  #endif

  #if ENABLED(ADVANCED_PAUSE_FANS_PAUSE) && HAS_FAN
    thermalManager.set_fans_paused(false);
  #endif

  TERN_(HAS_FILAMENT_SENSOR, runout.reset());

  // Resume the print job timer if it was running
  if (print_job_timer.isPaused()) print_job_timer.start();
  #if TRONXY_PRINTING_NOWAIT
  if(my_print_status == PRINT_RUNNING){
    printing_no_wait = true;
  }
  #endif

  TERN_(HAS_DISPLAY, ui.reset_status());
  TERN_(HAS_LCD_MENU, ui.return_to_status());
}

#endif // ADVANCED_PAUSE_FEATURE
