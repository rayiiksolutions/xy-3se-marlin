

Import("env")

env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "ihex", "-R", ".eeprom",
        "$BUILD_DIR/${PROGNAME}.elf", "../5download/Tronxy/fmw_tronxy_stm32f446.hex"
    ]), "Building ../5download/Tronxy/fmw_tronxy_stm32f446.hex")
)
