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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "../../gcode.h"
#include "../../../feature/powerloss.h"
#include "../../../module/motion.h"
#include "Tronxy.h"
#if ENABLED(EXTENSIBLE_UI)
  #include "../../../lcd/extui/ui_api.h"
#endif

#define DEBUG_OUT ENABLED(DEBUG_POWER_LOSS_RECOVERY)
#include "../../../core/debug_out.h"

void menu_job_recovery();

inline void plr_error(PGM_P const prefix) {
  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    DEBUG_ECHO_START();
    serialprintPGM(prefix);
    DEBUG_ECHOLNPGM(" Job Recovery Data");
  #else
    UNUSED(prefix);
  #endif
}

#if HAS_LCD_MENU
  void lcd_power_loss_recovery_cancel();
#endif

/**
 * M1000: Resume from power-loss (undocumented)
 *   - With 'S' go to the Resume/Cancel menu
 *   - With no parameters, run recovery commands
 */
void GcodeSuite::M1000() {
  if (PrintJobRecovery::valid()) {
    if (parser.seen('S')) {
      screenPopIndex(POP_RECOVERY_PRINT);
    }
    else if (parser.seen('C')) {
      #if HAS_LCD_MENU
        lcd_power_loss_recovery_cancel();
      #else
        PrintJobRecovery::cancel();
      #endif
      TERN_(EXTENSIBLE_UI, ExtUI::onPrintTimerStopped());
    }
    else 
      PrintJobRecovery::resume();
  }
  else
    plr_error(PrintJobRecovery::info.valid_head ? PSTR("No") : PSTR("Invalid"));

}

#endif // POWER_LOSS_RECOVERY