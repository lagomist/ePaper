#include "nvs_wrapper.h"
#include "fs_wrapper.h"
#include "bsp.h"
#include "button_monitor.h"

// // --- 按键相关 GPIO 配置 ---
// #define BUTTON_GPIO 0 // 修改为你的真实引脚
// #define BUTTON_ACTIVE_LEVEL 0 // 低电平有效

// // --- 按键检测全局事件标志 ---
// static volatile bool g_short_press_evt = false;
// static volatile bool g_long_press_evt = false;

// --- 按键检测任务 ---
// void button_poll_task(void* arg) {
//     const TickType_t poll_delay = 20 / portTICK_PERIOD_MS;
//     TickType_t pressed_tick = 0;
//     bool pressed = false;
//     while (1) {
//         int val = gpio_get_level((gpio_num_t)BUTTON_GPIO);
//         if (val == BUTTON_ACTIVE_LEVEL) {
//             if (!pressed) {
//                 pressed = true;
//                 pressed_tick = xTaskGetTickCount();
//             }
//         } else {
//             if (pressed) {
//                 TickType_t now = xTaskGetTickCount();
//                 TickType_t dt = now - pressed_tick;
//                 if (dt >= 100 / portTICK_PERIOD_MS && dt < 1500 / portTICK_PERIOD_MS) {
//                     g_short_press_evt = true;
//                 }
//                 if (dt >= 1500 / portTICK_PERIOD_MS) {
//                     g_long_press_evt = true;
//                 }
//                 pressed = false;
//             }
//         }
//         vTaskDelay(poll_delay);
//     }
// }


extern "C" void app_main(void) {
    Wrapper::NVS::init("nvs");
    Wrapper::FileSystem::mount();

    bsp.init();
    buttonMonitor.init();

    // 2. 按键GPIO配置和任务启动
    // gpio_config_t io_conf = {.pin_bit_mask = 1ULL << BUTTON_GPIO, .mode = GPIO_MODE_INPUT, .pull_up_en = GPIO_PULLUP_ENABLE, .pull_down_en = GPIO_PULLDOWN_DISABLE, .intr_type = GPIO_INTR_DISABLE};
    // gpio_config(&io_conf);
    // xTaskCreatePinnedToCore(button_poll_task, "btnTask", 2048, NULL, 2, NULL, 1);

    // // 3. 动态创建所有 Widget，根据总数分页
    // std::vector<DashboardWidget*> widget_all;
    // widget_all.push_back(new SDCardWidget());
    // widget_all.push_back(new PowerWidget());
    // widget_all.push_back(new RTCWidget());
    // widget_all.push_back(new SHTC3Widget());
    // widget_all.push_back(new ButtonWidget());
    // widget_all.push_back(new LEDWidget());
    // widget_all.push_back(new MICWidget());
    // widget_all.push_back(new MCUWidget());

    // // 自动分页，每 8 个 Widget 一页
    // std::vector<DashboardPage*> pages;
    // int widgets_per_page = 8, total = widget_all.size();
    // for (int i = 0; i < total; i += widgets_per_page) {
    //     std::vector<DashboardWidget*> sub;
    //     for (int j = i; j < i + widgets_per_page && j < total; ++j) sub.push_back(widget_all[j]);
    //     pages.push_back(new DashboardPage(sub));
    // }
    // int page_idx = 0, num_pages = pages.size();

    // // 4. 启动时 render 首页
    // auto* page = pages[page_idx];
    // for (int i = 0; i < page->size(); ++i) {
    //     auto* w = page->getWidget(i);
    //     if (w) w->update();
    // }
    // page->render(0, 0, 88, 32);

    // // 5. 主循环：消费按键事件
    // while (1) {
    //     if (g_long_press_evt) {
    //         g_long_press_evt = false;
    //         power_off();
    //         break;
    //     }
    //     if (g_short_press_evt) {
    //         g_short_press_evt = false;
    //         page_idx = (page_idx + 1) % num_pages;
    //         auto* page = pages[page_idx];
    //         for (int i = 0; i < page->size(); ++i) {
    //             auto* w = page->getWidget(i);
    //             if (w) w->update();
    //         }
    //         page->render(0, 0, 88, 32);
    //     }
    //     vTaskDelay(20 / portTICK_PERIOD_MS);
    // }
    // for (auto* page : pages) delete page;
}
