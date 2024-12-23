
#include "m5gfx_lvgl.hpp"

SemaphoreHandle_t xGuiSemaphore;

LV_IMG_DECLARE(cursor_hand);

static void m5gfx_lvgl_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // 等待之前的 DMA 傳輸完成
    lv_draw_sw_rgb565_swap(px_map, w * h);
    M5.Display.startWrite();
    M5.Display.pushImageDMA<uint16_t>(area->x1, area->y1, w, h, (uint16_t *)px_map);
    M5.Display.waitDMA();
    M5.Display.endWrite();
    lv_display_flush_ready(disp);
}

static void m5gfx_lvgl_read(lv_indev_t * drv, lv_indev_data_t * data) {
    M5.update();
    auto t = M5.Touch.getDetail();
    
    if (t.isPressed()) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = t.x;
        data->point.y = t.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

static uint32_t my_tick_function() {
  return (esp_timer_get_time() / 1000LL);
}


#define LCD_BUFFER_SIZE (LCD_WIDTH * 80)

void m5gfx_lvgl_init(void) {
    
    lv_tick_set_cb(my_tick_function);

    
    
    //static lv_color_t *buf1 = (lv_color_t *)heap_caps_aligned_alloc(32, 
    //    LCD_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM |MALLOC_CAP_8BIT);
    //static lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(
    //    LCD_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM| MALLOC_CAP_DMA);

    //if(buf1 == nullptr ) {
    //    log_e("Failed to allocate memory for display buffers");
    //    return;
   // }

    static lv_color_t buf1[LCD_BUFFER_SIZE] __attribute__((aligned(4)));

    //static lv_color_t buf2[LCD_BUFFER_SIZE];

    // Create and configure the display
    lv_display_t* disp = lv_display_create(LCD_WIDTH, LCD_HEIGHT);
    if (!disp) {
        log_e("Failed to create display");
        return;
    }

    // Configure display properties
    lv_display_set_buffers(disp, buf1, nullptr , LCD_BUFFER_SIZE * sizeof(lv_color_t), LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_flush_cb(disp, m5gfx_lvgl_flush);
    //lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    //lv_display_set_antialiasing(disp, true);

    // Configure touch input
    lv_indev_t* touch_indev = lv_indev_create();
    if (!touch_indev) {
        log_e("Failed to create touch input device");
        return;
    }

    lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(touch_indev, m5gfx_lvgl_read);

    // Set up LVGL timer
    xGuiSemaphore                                     = xSemaphoreCreateMutex();
    /*
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lvgl_tick_task,
        .name = "lv_tick"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
    */

}