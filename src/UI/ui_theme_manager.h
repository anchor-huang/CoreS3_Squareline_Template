// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2

#ifndef _UI_THEME_MANAGER_H
#define _UI_THEME_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#define UI_THEME_ACTIVE 1

#ifndef LV_SQUARELINE_THEME__IMPLICIT_GARBAGE_COLLECTOR
 #ifndef LV_SQUARELINE_THEME__EXPLICIT_GARBAGE_COLLECTOR_PERIOD  //for slow hardware, tells how frequently the style-properties inside this style should be garbage-collected (recommended value: 10..100, 0:never)
  #define LV_SQUARELINE_THEME__EXPLICIT_GARBAGE_COLLECTOR_PERIOD 10  //a sensible default value that already gives good results on slow hardware, can be overridden from outside
 #endif
#endif


enum { UI_VARIABLE_STYLES_MODE_INIT = 1, UI_VARIABLE_STYLES_MODE_FOLLOW = 0 };


typedef int64_t ui_style_variable_t;
typedef ui_style_variable_t ui_theme_variable_t; //A 'theme' variable array is an array of 'style' variables for corresponding themes.

typedef struct {
    lv_obj_t            * object_p;
    lv_style_selector_t   selector;
    lv_style_prop_t       property;
    #ifdef LV_SQUARELINE_THEME__OBJECT_VALIDITY_CACHE  //for slow hardware, adding this and setting to 5..10 in lv_conf.h might make it faster (less lv_obj_is_valid calls)
     uint16_t              validcheck_counter; //used for garbage collection (finding empty slots) not to call lv_obj_is_valid() every time
    #endif
    void                * next_p;
} _ui_local_style_property_setting_t;

typedef struct {
    bool                  is_themeable; //scalar or vector?
    ui_style_variable_t * style_variable_p;
    ui_style_variable_t * previous_pointer;
    ui_style_variable_t   previous_value;
    uint32_t              style_property_setting_count;
    _ui_local_style_property_setting_t * style_property_settings;
    #ifdef LV_SQUARELINE_THEME__EXPLICIT_GARBAGE_COLLECTOR_PERIOD
     uint16_t              garbage_collector_couter;
    #endif
} _ui_local_style_t;


extern _ui_local_style_t * _ui_local_styles;
extern uint32_t            _ui_local_style_count;


void ui_object_set_local_style_property
(lv_obj_t* object_p, lv_style_selector_t selector, lv_style_prop_t property, ui_style_variable_t value );

void ui_object_set_themeable_style_property
(lv_obj_t* object_p, lv_style_selector_t selector, lv_style_prop_t property, const ui_theme_variable_t* theme_variable_p);

void _ui_theme_set_variable_styles (uint8_t mode);

lv_style_value_t    _ui_style_value_convert (lv_style_prop_t property, ui_style_variable_t value);
ui_style_variable_t ui_get_theme_value      (const ui_theme_variable_t *var);

_ui_local_style_t * _ui_local_style_create  (const ui_style_variable_t * style_variable_p, bool is_themeable);

_ui_local_style_property_setting_t * _ui_local_style_property_setting_create
(_ui_local_style_t * local_style, lv_obj_t * object_p, lv_style_selector_t selector, lv_style_prop_t property);


#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
