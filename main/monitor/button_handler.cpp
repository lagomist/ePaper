#include "button_handler.h"
#include "bsp.h"
#include "led_heartbeat.h"
#include "esp_log.h"

constexpr static const char *TAG = "ButtonMonitor";

ButtonMonitor buttonMonitor;

void ButtonMonitor::bootBtnHandler(MultiButton &btn) {
    MultiButton::Event state = btn.getState();
    switch (state) {
    case MultiButton::Event::SingleClick :
        _event->set(BOOT_SINGLE_CLICK);
        break;
    case MultiButton::Event::DoubleClick :
        _event->set(BOOT_DOUBLE_CLICK);
        break;
    case MultiButton::Event::LongPressStart :
        _event->set(BOOT_LONG_PRESS);
        break;
    default:
        break;
    }
}

void ButtonMonitor::powerBtnHandler(MultiButton &btn) {
    MultiButton::Event state = btn.getState();
    switch (state) {
    case MultiButton::Event::SingleClick :
        _event->set(POWER_SINGLE_CLICK);
        break;
    case MultiButton::Event::DoubleClick :
        _event->set(POWER_DOUBLE_CLICK);
        break;
    case MultiButton::Event::LongPressStart :
        _event->set(POWER_LONG_PRESS);
        break;
    default:
        break;
    }
}

void ButtonMonitor::btnEventHandler(void *arg) {
    ButtonMonitor *btnMonitor = (ButtonMonitor *)arg;
    while (1) {
        int evt = btnMonitor->_event->wait_any(0xFF, true);
        if (evt & ButtonMonitor::BOOT_SINGLE_CLICK) {
            ESP_LOGI(TAG, "Boot button single click");
        }
        if (evt & ButtonMonitor::BOOT_DOUBLE_CLICK) {
            ESP_LOGI(TAG, "Boot button double click");
        }
        if (evt & ButtonMonitor::BOOT_LONG_PRESS) {
            ESP_LOGI(TAG, "Boot button long press");
        }
        if (evt & ButtonMonitor::POWER_SINGLE_CLICK) {
            ESP_LOGI(TAG, "Power button single click");
        }
        if (evt & ButtonMonitor::POWER_DOUBLE_CLICK) {
            ESP_LOGI(TAG, "Power button double click");
        }
        if (evt & ButtonMonitor::POWER_LONG_PRESS) {
            ESP_LOGI(TAG, "Power button long press");
            Wrapper::OS::delay(500);
            ledHeartbeat.stop();
            bsp.power->epdPowerOff();
            bsp.power->audioPowerOff();
            bsp.power->vbatPowerOff();
        }
        Wrapper::OS::delay(100);
    }
    
}


void ButtonMonitor::init() {
    ESP_LOGI(TAG, "init");
    bsp.powerButton->addEvent(MultiButton::Event::SingleClick, [this](MultiButton &btn) {
        this->powerBtnHandler(btn);
    });
    bsp.powerButton->addEvent(MultiButton::Event::DoubleClick, [this](MultiButton &btn) {
        this->powerBtnHandler(btn);
    });
    bsp.powerButton->addEvent(MultiButton::Event::LongPressStart, [this](MultiButton &btn) {
        this->powerBtnHandler(btn);
    });
    bsp.bootButton->addEvent(MultiButton::Event::SingleClick, [this](MultiButton &btn) {
        this->bootBtnHandler(btn);
    });
    bsp.bootButton->addEvent(MultiButton::Event::DoubleClick, [this](MultiButton &btn) {
        this->bootBtnHandler(btn);
    });
    bsp.bootButton->addEvent(MultiButton::Event::LongPressStart, [this](MultiButton &btn) {
        this->bootBtnHandler(btn);
    });

    _event = new Wrapper::OS::EventGroup();
    _task = new Wrapper::OS::Task();
    _task->create(btnEventHandler, this, "ButtonMonitor", 8 * 1024, 5);
    _timer = new Wrapper::OS::Timer([](){
        MultiButton::pollAll();
    }, 10, true, "ButtonMonitor", nullptr);
    _timer->start();
}