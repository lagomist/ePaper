#include "lvgl_display.h"
#include "timer_wrapper.h"

#include "esp_heap_caps.h"
#include "esp_log.h"


constexpr static const char TAG[] = "lv_display";

Wrapper::OS::RecursiveMutex * LVDisplay::mutex = nullptr;
static Wrapper::Timer *lvTickTimer = nullptr;
static bool lvInited = false;

static void increase_lvgl_tick(void) {
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVDisplay::LV_TICK_PERIOD_MS);
}

/**
 * gui task periodic timer task
*/
void LVDisplay::gui_disp_task(void *pvParameter) {
	ESP_LOGI(TAG, "start lvgl task");
	uint32_t time_till_next_ms = 0;

	while (1) {
		/* Try to take the semaphore, call lvgl related function on success */
		mutex->lock();
		time_till_next_ms = lv_timer_handler();
		mutex->unlock();

		// in case of task watch dog timeout, set the minimal delay to 10ms
        if (time_till_next_ms < 10) {
            time_till_next_ms = 10;
        }

		Wrapper::OS::delay(time_till_next_ms);
	}
}

void LVDisplay::disp_flush_cb(lv_display_t * drv, const lv_area_t * area, uint8_t * px_map) {
	LVDisplay *disp = (LVDisplay *)lv_display_get_user_data(drv);
	if (disp->_flush_cb) {
		disp->_flush_cb(disp, area, px_map);
	}

	lv_display_flush_ready(drv);
}



/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void LVDisplay::lvgl_init() {
	if (lvInited) return;
	ESP_LOGI(TAG, "Initialize LVGL library");
	lv_init();
	Wrapper::OS::Task thread;
	mutex = new Wrapper::OS::RecursiveMutex();
	lvTickTimer = new Wrapper::Timer(increase_lvgl_tick, LV_TICK_PERIOD_MS, true);
	thread.create(gui_disp_task, nullptr, "lvgl", 10 * 1024, 4, Wrapper::OS::Task::Core::CORE_1);
	lvInited = true;
}

void LVDisplay::init(uint16_t hor_res, uint16_t ver_res, size_t buf_size, uint8_t num_fb) {
	/*------------------------------------
	 * Create a display and set a flush_cb
	 * -----------------------------------*/
	void *buf1 = nullptr;
	void *buf2 = nullptr;
	
	lvgl_init();
	mutex->lock();
	disp = lv_display_create(hor_res, ver_res);
	buf1 = (uint8_t *)heap_caps_malloc(buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
	assert(buf1);
	if (num_fb > 1) {
		buf2 = (uint8_t *)heap_caps_malloc(buf_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
		assert(buf2);
	}
	// initialize LVGL draw buffers
	lv_display_set_buffers(disp, buf1, buf2, buf_size, LV_DISPLAY_RENDER_MODE_DIRECT);

    lv_display_set_user_data(disp, this);
	lv_display_set_flush_cb(disp, disp_flush_cb);
	lvTickTimer->start();
	mutex->unlock();
}

