#include "multi_button.h"
#include <algorithm>

std::vector<MultiButton *> MultiButton::s_btns;

MultiButton::MultiButton(Wrapper::GPIBase &gpio, uint8_t active_level, uint16_t short_ticks, uint16_t long_ticks)
    : m_gpio(gpio), m_active_level(active_level),
      m_long_ticks(long_ticks), m_short_ticks(short_ticks),
      m_state(State::IDLE), m_ticks(0), m_debounce_cnt(0), m_event(Event::None)
{
    is_enable = true;
    addToGlobal(this);
}

MultiButton::~MultiButton() {
    removeFromGlobal(this);
}

void MultiButton::resume() {
    is_enable = true;
    reset();
}
void MultiButton::suspend() {
    is_enable = false;
}

void MultiButton::reset() {
    m_state = State::IDLE;
    m_ticks = 0;
    m_debounce_cnt = 0;
    m_event = Event::None;
}

void MultiButton::addEvent(Event evt, Callback cb) {
    m_callbacks[evt] = std::move(cb);
}
void MultiButton::removeEvent(Event evt) {
    m_callbacks.erase(evt);
}

bool MultiButton::isPressed() const {
    Event val = getState();
    return val == Event::PressDown;
}

bool MultiButton::isEnable() const {
    return is_enable;
}

MultiButton::Event MultiButton::getState() const {
    return m_event;
}

void MultiButton::poll() {
	uint8_t read_gpio_level = m_gpio.get();

	// Increment ticks counter when not in idle state
	if (m_state > State::IDLE) {
		m_ticks++;
	}

	/*------------Button debounce handling---------------*/
	if (read_gpio_level != m_button_level) {
		// Continue reading same new level for debounce
		if (++(m_debounce_cnt) >= DEBOUNCE_TICKS) {
			m_button_level = read_gpio_level;
			m_debounce_cnt = 0;
		}
	} else {
		// Level not changed, reset counter
		m_debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (m_state) {
	case State::IDLE:
		if (m_button_level == m_active_level) {
			// Button press detected
            handleEvent(Event::PressDown);
			m_ticks = 0;
			m_repeat = 1;
			m_state = State::PRESSED;
		}
		break;

	case State::PRESSED:
		if (m_button_level != m_active_level) {
			// Button released
            handleEvent(Event::PressUp);
			m_ticks = 0;
			m_state = State::RELEASED;
		} else if (m_ticks > m_long_ticks) {
			// Long press detected
            handleEvent(Event::LongPressStart);
			m_state = State::LONG_HOLD;
		}
		break;

	case State::RELEASED:
		if (m_button_level == m_active_level) {
			// Button pressed again
            handleEvent(Event::PressDown);
			if (m_repeat < PRESS_REPEAT_MAX_NUM) {
				m_repeat++;
			}
            handleEvent(Event::PressRepeat);
			m_ticks = 0;
			m_state = State::REPEAT;
		} else if (m_ticks > m_short_ticks) {
			// Timeout reached, determine click type
			if (m_repeat == 1) {
                handleEvent(Event::SingleClick);
			} else if (m_repeat == 2) {
                handleEvent(Event::DoubleClick);
			}
			m_state = State::IDLE;
		}
		break;

	case State::REPEAT:
		if (m_button_level != m_active_level) {
			// Button released
            handleEvent(Event::PressUp);
			if (m_ticks < m_short_ticks) {
				m_ticks = 0;
				m_state = State::RELEASED;  // Continue waiting for more presses
			} else {
				m_state = State::IDLE;  // End of sequence
			}
		} else if (m_ticks > m_short_ticks) {
			// Held down too long, treat as normal press
			m_state = State::PRESSED;
		}
		break;

	case State::LONG_HOLD:
		if (m_button_level == m_active_level) {
			// Continue holding
            handleEvent(Event::LongPressHold);
		} else {
			// Released from long press
            handleEvent(Event::PressUp);
			m_state = State::IDLE;
		}
		break;

	default:
		// Invalid state, reset to idle
		m_state = State::IDLE;
		break;
	}
}

void MultiButton::handleEvent(Event ev) {
    m_event = ev;
    auto it = m_callbacks.find(ev);
    if (it != m_callbacks.end()) {
        it->second(*this);
    }
}

void MultiButton::pollAll() {
    for (auto *btn : s_btns) {
        if (btn->isEnable()) {
            btn->poll();
        }
    }
}
void MultiButton::addToGlobal(MultiButton *b) {
    if (std::find(s_btns.begin(), s_btns.end(), b) == s_btns.end()) {
        s_btns.push_back(b);
    }
}
void MultiButton::removeFromGlobal(MultiButton *b) {
    auto it = std::find(s_btns.begin(), s_btns.end(), b);
    if (it != s_btns.end()) {
        s_btns.erase(it);
    }
}
