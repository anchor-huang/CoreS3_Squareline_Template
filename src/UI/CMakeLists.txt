SET(SOURCES screens/ui_StartUpPage.c
    screens/ui_WifiConfigPage.c
    screens/ui_Main.c
    ui_theme_manager.c
    ui_themes.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_core_text_png.c
    images/ui_img_s3_text_png.c
    images/ui_img_1024934270.c)

add_library(ui ${SOURCES})
