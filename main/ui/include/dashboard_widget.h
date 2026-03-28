#ifndef DASHBOARD_WIDGET_H
#define DASHBOARD_WIDGET_H

#include <string>
#include <cstdint>

class DashboardWidget {
public:
    virtual ~DashboardWidget() {}
    // Returns 16x16 icon bitmap (pointer to icon[32])
    virtual const uint8_t* getIcon() const = 0;
    // Returns simple status string (max ~12-14 chars for e-ink)
    virtual std::string getStatusText() const = 0;
    // Poll device for latest status, update internal cache/state
    virtual void update() = 0;
};

// SD Card widget
class SDCardWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    bool present = false;
};

// Power widget
class PowerWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    float voltage = 0.0f;
};

// RTC widget
class RTCWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    bool running = false;
};

// SHTC3 (temp/humidity)
class SHTC3Widget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    float temp = 0, humidity = 0;
};

// Button widget
class ButtonWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    bool pressed = false;
};

// LED widget
class LEDWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    bool led_on = false;
};

// MIC widget
class MICWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    float level = 0;
};

// MCU widget (board self-test/heartbeat)
class MCUWidget : public DashboardWidget {
public:
    void update() override;
    const uint8_t* getIcon() const override;
    std::string getStatusText() const override;
private:
    bool ok = true;
};

#endif // DASHBOARD_WIDGET_H
