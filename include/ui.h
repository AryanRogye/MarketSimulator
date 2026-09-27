#ifndef UI_H
#define UI_H

#include "MarketState.h"
#include <memory>
#include "GLFW/glfw3.h"

struct WindowSize {
    float x;
    float y;
    float width;
    float height;
};

class UI {
    public:
        UI(std::shared_ptr<MarketState> marketState);
        void begin();
    private:
        std::shared_ptr<MarketState> marketState;
        GLFWwindow* window;

        void drawStocks(WindowSize size);
        void drawUsers(WindowSize size);
};

#endif // UI_H
