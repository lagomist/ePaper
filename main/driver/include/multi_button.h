#pragma once

#include "gpio_wrapper.h"
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

class MultiButton {
public:
    enum class Event : uint8_t {
        None = 0,
        PressDown,
        PressUp,
        PressRepeat,
        SingleClick,
        DoubleClick,
        LongPressStart,
        LongPressHold
    };

    using Callback = std::function<void(MultiButton &)>;

    MultiButton(Wrapper::GPIBase &gpio, uint8_t active_level = 0, uint16_t short_ticks = 30, uint16_t long_ticks = 100);
    ~MultiButton();

    void resume();
    void suspend();
    void poll();
    void reset();

    void addEvent(Event evt, Callback cb); // 注册事件
    void removeEvent(Event evt);            // 注销

    bool isPressed() const;
    bool isEnable() const;
    Event getState() const;

    // 多键管理静态接口
    static void pollAll();
    static void addToGlobal(MultiButton* btn);
    static void removeFromGlobal(MultiButton* btn);

private:
    enum class State : uint8_t {
        IDLE = 0,
        PRESSED,
        RELEASED,
        REPEAT,
        LONG_HOLD
    };
    constexpr static uint8_t DEBOUNCE_TICKS = 3;
    constexpr static uint8_t PRESS_REPEAT_MAX_NUM = 3;
    Wrapper::GPIBase &m_gpio;
    uint8_t m_active_level;
    uint16_t m_long_ticks;
    uint16_t m_short_ticks;

    State m_state;
    uint16_t m_ticks;
    uint16_t m_debounce_cnt;
    uint8_t m_button_level;
    uint8_t m_repeat;
    Event m_event;
    bool is_enable;
    std::map<Event, Callback> m_callbacks;

    // 多实例管理
    static std::vector<MultiButton*> s_btns;
    void handleEvent(Event ev);

    // 禁止复制
    MultiButton(const MultiButton&) = delete;
    MultiButton& operator=(const MultiButton&) = delete;
};

