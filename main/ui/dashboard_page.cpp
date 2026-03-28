#include "dashboard_page.h"
#include "epaper_icon.h"
#include <string.h>

// For display text. Adjust with your font/driver, row height, etc.
extern void EPD_DrawStringAt(int x, int y, const char* text, int font_size);

DashboardPage::DashboardPage(const std::vector<DashboardWidget*>& widgets)
    : m_widgets(widgets) {}

DashboardPage::~DashboardPage() {
    for (auto* w : m_widgets) {
        delete w;
    }
}

DashboardWidget* DashboardPage::getWidget(int idx) const {
    if (idx < 0 || idx >= (int)m_widgets.size()) return nullptr;
    return m_widgets[idx];
}

void DashboardPage::render(int x0, int y0, int cell_w, int cell_h) const {
    // 2 cols x 4 rows grid
    for (int i = 0; i < (int)m_widgets.size(); ++i) {
        int col = i % 2, row = i / 2;
        int icon_x = x0 + col * cell_w;
        int icon_y = y0 + row * cell_h;
        const uint8_t* icon = m_widgets[i]->getIcon();
        epaper_draw_icon16(icon_x, icon_y, icon);
        // Draw status text under/beside icon
        std::string status = m_widgets[i]->getStatusText();
        EPD_DrawStringAt(icon_x + 18, icon_y + 4, status.c_str(), 12); // 18px right of icon, Y+4 offset. Font size 12px.
    }
}
