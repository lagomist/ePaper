#ifndef DASHBOARD_PAGE_H
#define DASHBOARD_PAGE_H

#include <vector>
#include "dashboard_widget.h"

class DashboardPage {
public:
    // Takes ownership of widget pointers
    DashboardPage(const std::vector<DashboardWidget*>& widgets);
    ~DashboardPage();
    // Renders this page: 2x4 grid of icon+status text
    void render(int x0, int y0, int cell_w, int cell_h) const;
    int size() const { return m_widgets.size(); }
    DashboardWidget* getWidget(int idx) const;
private:
    std::vector<DashboardWidget*> m_widgets;
};

#endif // DASHBOARD_PAGE_H
