# AGENTS.md

**本仓库自动化Agent与开发者专用指南**

---

本文件为代理（如Copilot、Cursor，以及后续的自动化Agent）与人工开发者，提供权威的准备流程、构建和测试指令、C++14+ESP-IDF+LVGL项目专用代码风格，并详细描述实际工程结构与模块划分。

本项目结构专为现代化嵌入式（特别是ESP32-S3/ePaper/LVGL应用）+ 面向对象C++设计，已适配多工具链Agent和自动测试/集成规约。

---

## 1. 构建、测试、Lint

### 1.1. 构建固件（ESP-IDF平台）

- **激活 IDF 环境**（每次构建前须执行）：
  ```sh
  IDF
  ```
- 构建整个项目（带缓存及 size 输出）：
  ```sh
  idf.py --ccache build size
  ```
- 高级/自定义（CMake直接调用）：
  ```sh
  cmake .
  make
  ```

### 1.2. 测试建议（硬件/应用模块）

- 推荐为每个主要驱动、BSP、功能模块添加对应 `test/` 或 `test_apps/` 子目录，并实现独立 main 入口的基础自测例程，便于开发与CI识别。
- 测试例程通常直接以 ESP-IDF 固件构建运行，或通过增加测试目标模块到主构建树统一管理。
- 测试示例：
  ```sh
  idf.py -DTEST_COMPONENT=bsp test
  ```
- 所有模块测试应与主用例和业务逻辑解耦，优先打印日志/对比结果，必要时用 ESP_LOG 系统断言。
- 非必须测试 LVGL 官方自带测试环境和 Docker 容器。

### 1.3. Lint检查
- 本仓库尚无自动linter配置，格外**请严格遵守下方代码风格**。

---

## 2. 目录结构说明

- `main/`
    - `bsp/`, `driver/`, `monitor/`, `ui/` ：模块主源码，**每个子目录均有 include/ 放头文件**
    - 所有接口头文件均在`include/`子目录下维护
    - `main.cpp`：入口文件（初始化及主循环）
- `components/` ：外部submodule（如LVGL及第三方库）
- `scripts/` ：工具脚本
- `doc/` ：文档
- `build/` ：构建产物

---

## 3. 代码风格 C++14/ESP-IDF/LVGL

### 3.1. 基本原则
- 所有新模块、测试、平台相关交互**统一使用C++14**（低层驱动支持extern "C"接口但推荐纯C++）。
- 强制严格构建（见CMakeLists.txt），所有**警告视为错误**，无警告通过为底线。
- 每次PR/合入前必须保证完整构建和所有主要测试通过。
- 保证良好的面向对象结构、RAII、所有权管理，尽量避免原始指针和裸资源泄露。
- 没有官方自动格式化配置，**人工风格一致性最重要**！相邻、同目录代码须保持风格一致。

### 3.2. 文件与命名
- 目录/文件命名小写、下划线分隔：`epaper_driver.h`/`epaper_driver.cpp`
- 头文件强制后缀`.h`，源文件`.cpp`。
- 新增测试以`test_*.c`或C++测试文件，放入对应`test/`/`test_apps/`。
- 类成员变量：`m_`前缀（如`int m_value;`），静态成员`s_`。
- 类、结构体/枚举用 PascalCase：`class ButtonMonitor`、`enum class GpioType : uint8_t { ... }`。
- 函数、变量：小驼峰`camelCase`。
- 常量、枚举值全部大写下划线`LVGL_CONST_NAME`。

### 3.3. import与包含顺序
- 先标准库/系统头文件（<...>），再项目/本地头文件（"..."），同类分组。
    ```cpp
    #include <vector>
    #include <driver/gpio.h>
    #include "bsp/include/epaper_driver.h"
    #include "ui/include/my_widget.h"
    ```

### 3.4. 缩进与格式
- 4空格，**禁用Tab**。
- 行宽建议80-100字符以内。
- 每行仅一句。
- 所有大括号同一行，如`void foo() { ... }`。
- 关键字、括号与语句之间空格，如`if (cond) {`。

### 3.5. 类型、声明与OOP惯例
- 强制用显式类型，不用`auto`推导易错场合。
- 默认禁止拷贝构造（`= delete`），推荐只允许move。
- 指针强制用`std::unique_ptr`，绝不随便裸指针。
- 成员变量全部`m_`前缀，静态成员`s_`。
- 用enum class代替原生enum。
- 回调统一用`std::function<>`和binder。
- 对于硬件资源类（如I2C/SPI设备），要RAII管理打开/关闭。

### 3.6. 错误处理
- 全部非void函数须检查返回值，不能忽略。
- 用断言/ESP_LOG/异常机制组织错误分级处理。
- 测试用例建议用 `ESP_LOG`、断言和最小测试宏。
- 其他层优先返回错误码或用ESP-IDF自带错误机制。

### 3.7. 注释与文档
- 非直观代码、模块/复杂函数，需用Doxygen注释（`/** ... */`）。
- 新增/变更API与重要结构在头文件补充注释。
- 关键路径中文注释简明扼要，尽量用英文。

---

## 4. 测试与CI

- 建议所有主要组件配套 test/ 或 test_apps/ 目录及最小可构建、单元自测例程。
- 全局集成测试、CI建议通过 ESP-IDF 平台直接调用测试固件/日志断言。
- 严禁引入 LVGL 官方库内部的 main.py、ctest、或 Docker 相关环境与脚本。
- 如需覆盖率、回归或接口监控，可用 ESP_LOG 日志处理关联脚本，避免侵入第三方依赖体系。

---

## 5. 相关资料（直接随用）

- [LVGL官方文档](https://docs.lvgl.io/)
- [LVGL贡献与风格](https://docs.lvgl.io/latest/en/html/contributing/style.html)
- [ESP-IDF官方编程风格](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/writing-style.html)

---

## 6. 补充说明

- 所有目录及文件请遵循本文件规约。Agent工具需优先参照本文件执行各类CI、构建和风格修正任务。
- 开发流程若有非常规需求（如独立环境/脚本/Agent），请更新本文件并在变更PR内提示reviewer重点关注。
- 若发现风格、构建方式与实际工程演变不符，请优先修正AGENTS.md。