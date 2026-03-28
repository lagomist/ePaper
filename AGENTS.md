# AGENTS.md

**Repository Guidance for Automation Agents**

---

This file provides comprehensive preparation instructions, build and test workflows, and code style guidelines for any agents (e.g., Copilot, Cursor, or custom) operating in this repository.

---

## 1. BUILD, LINT, TEST, AND COVERAGE

### 1.1. Building the Firmware (ESP-IDF Platform)

- Build the whole project:
  ```sh
  idf.py build
  ```
- If using CMake directly (advanced):
  ```sh
  cmake .
  make
  ```

### 1.2. Testing LVGL Components

#### Requirements
- Requires Python 3, Ninja, CMake, and build-essential tools (see `scripts/install-prerequisites.sh`).
- For full reproducibility or to match CI, use provided Docker workflow below.

#### Basic Testing
- Install dependencies:
  ```sh
  scripts/install-prerequisites.sh
  ```
- Run all executable tests:
  ```sh
  ./components/lvgl/tests/main.py test
  ```
- Run both build and executable tests:
  ```sh
  ./components/lvgl/tests/main.py build test
  ```
- Run build-only tests (compile/link correctness):
  ```sh
  ./components/lvgl/tests/main.py build
  ```

#### Running a Single Test
- Each test maps to a file in `components/lvgl/tests/src/test_cases/` named `test_<name>.c`.
- To run a specific test or set of tests matching a regex (including single test), use:
  ```sh
  ./components/lvgl/tests/main.py --test-suite <TEST_REGEX> test
  ```
  Example:
  ```sh
  ./components/lvgl/tests/main.py --test-suite test_canvas test
  ```
  This will match and run the `test_canvas` executable.
- Alternatively, after building, you can run tests from the build directory using ctest:
  ```sh
  ctest -R <test_name>
  ```

#### Code Coverage (LVGL Library)
- Clean, rebuild, run tests, and generate an HTML+XML coverage report:
  ```sh
  ./components/lvgl/tests/main.py --clean --report build test
  # Output: components/lvgl/tests/report/index.html
  ```

#### Test Image Regeneration (Screenshot-based assertions)
- Requires pngquant and pypng:
  ```sh
  ./components/lvgl/tests/main.py --update-image test
  ```
- Note: Test image outputs vary slightly with pngquant version. CI uses pngquant 2.13.1-1.

#### Running in Docker (CI-reproducible)
- Build Docker image for the test environment:
  ```sh
  docker build . -f components/lvgl/tests/Dockerfile -t lvgl_test_env
  ```
- Run tests in Docker:
  ```sh
  docker run --rm -it -v $(pwd):/work lvgl_test_env "./components/lvgl/tests/main.py test"
  ```

#### Advanced/Performance/Benchmark Tests
- See tests/README.md for scripts: `perf.py`, `benchmark_emu.py`. E.g.,
  ```sh
  ./components/lvgl/tests/perf.py test
  ./components/lvgl/tests/benchmark_emu.py run
  ```

### 1.3. Linting
- No linter configuration or auto-formatting rules (e.g., .clang-format) found in this repo so far. Follow code style below.

---

## 2. CODE STYLE GUIDELINES

### 2.1. General
- Source files are C99 (preferred), C++14 permitted for test harnesses.
- **Follow the style conventions of the [LVGL project](https://docs.lvgl.io/latest/en/html/contributing/style.html).**
- For ESP-IDF integration, see the [Espressif coding guides](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/writing-style.html).

### 2.2. File and Test Naming
- Test source files: `test_<topic>.c` in `components/lvgl/tests/src/test_cases/`.
- New tests: copy from `_test_template.c` as a starting point.

### 2.3. Imports and Includes
- Use `#include "lvgl/lvgl.h"` for LVGL main API.
- Local includes: use quotes; system includes: use angle brackets.
- Group standard/system includes before project/local includes.

### 2.4. Formatting
- 4 spaces per indent, no tabs (unless project files otherwise dictate).
- Keep lines ≤80–100 chars where practical; wrap complex statements.
- One statement per line.
- Place opening braces on the same line as the function/statement.
- Space after keywords and before braces, e.g., `if (cond) {`.

### 2.5. Types and Declarations
- Prefer explicit types (never use typedef for structs unless idiomatic).
- Use clear, descriptive names: `lv_obj_t *label;`, `uint32_t counter;`.
- Pointer stars stick to variable: `char *ptr`, not `char* ptr`.

### 2.6. Naming Conventions
- Functions: `lowercase_with_underscores`, e.g., `lv_button_create`.
- Variables: `lowercase_with_underscores`.
- Types: `lv_<role>_t`, e.g., `lv_obj_t`, `lv_event_t`.
- Constants/macros: `LVGL_CONST_NAME`, enums: `LV_ENUM_ITEM`.

### 2.7. Error Handling
- Check the return value of all non-void functions.
- Use asserts thoughtfully (`LV_ASSERT`, `TEST_ASSERT_*`) for invariants.
- Express intent with custom macros for critical failure (`LV_ERROR(...)`).
- In tests, prefer Unity's assert macros. For LVGL-special asserts, use:
    - `TEST_ASSERT_EQUAL_SCREENSHOT("image.png")`
    - `TEST_ASSERT_EQUAL_COLOR(actual, expected)`

### 2.8. Warnings and Strictness
- All warnings elevated to errors (Werror) in most builds/tests.
- CMake for tests enables a large set of compiler warnings (see CMakeLists.txt for details).
- Address sanitizer (`-fsanitize=address`) and coverage (`--coverage`) enabled by default in non-Windows environments.

### 2.9. Contributions and Documentation
- Comment nontrivial blocks and functions with concise, descriptive doc comments.
- Use Doxygen comments (`/** ... */`) for public APIs if possible.
- Keep README.md and this file updated as code style and workflow evolve.

---

## 3. REFERENCES

- [LVGL Documentation](https://docs.lvgl.io/)
- [LVGL Contributing & Style Guide](https://docs.lvgl.io/latest/en/html/contributing/style.html)
- [Espressif ESP-IDF Style Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/writing-style.html)
- [Unity Assertion Reference](https://github.com/ThrowTheSwitch/Unity/blob/master/docs/UnityAssertionsReference.md)

---

## 4. WHEN IN DOUBT
- Mirror adjacent code and preferred style in each directory/component.
- Always prefer stricter compiler options, and ensure all tests pass before opening PRs or submitting contributions.

---

This AGENTS.md is maintained for the benefit of agentic coding tools and contributors. Edit as workflows or conventions change.
