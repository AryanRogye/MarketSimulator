#include "ui.h"
#include "GLFW/glfw3.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <iostream>
#include <unordered_map>

static std::string selected_symbol = "";

UI::UI(std::shared_ptr<MarketState> marketState) : marketState(marketState) {

    /// Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->window = glfwCreateWindow(
        1280,
        720,
        "MarketSim",
        nullptr,
        nullptr
    );

    if (!this->window) {
        std::cerr << "Failed to create GLFW window\n";
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

enum UIWindow {
    STOCK_SCREEN,
    USERS_SCREEN,
};
std::unordered_map<UIWindow, WindowSize> getWindowSizes(int width, int height) {

    float usersScreenWidth = 200.0f;
    float stockScreenWidth = static_cast<float>(width) - usersScreenWidth;

    float stockScreenHeight = static_cast<float>(height);
    float usersScreenHeight = static_cast<float>(height);
    
    std::unordered_map<UIWindow, WindowSize> sizes;
    sizes[STOCK_SCREEN] = {0.0f, 0.0f, stockScreenWidth, stockScreenHeight};
    sizes[USERS_SCREEN] = {stockScreenWidth, 0.0f, usersScreenWidth, usersScreenHeight};
    return sizes;
}

void UI::begin() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int fbW, fbH;
        glfwGetWindowSize(window, &fbW, &fbH);

        std::unordered_map<UIWindow, WindowSize> windowSizes = getWindowSizes(fbW, fbH);

        drawStocks(windowSizes[STOCK_SCREEN]);
        drawUsers(windowSizes[USERS_SCREEN]);
        
        ImGui::Render();
        
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
        
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
}

void UI::drawStocks(WindowSize size) {
    ImGui::SetNextWindowPos(ImVec2(size.x, size.y));
    ImGui::SetNextWindowSize(ImVec2(size.width, size.height));
    ImGui::Begin(
        "Stocks",
        nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
    );
    
    if (ImGui::BeginChild("Stocks", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border)) {
        ImGuiTableFlags flags =
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_ScrollY;
        
        if (ImGui::BeginTable("StockListTable", 1, flags, ImVec2(0.0f, ImGui::GetContentRegionAvail().y))) {
            for (const auto& stock: this->marketState->getStocks()) {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_SpanFullWidth;

                /// if is selected set the selected flag
                if (selected_symbol == stock->getSymbol()) {
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                }

                if (ImGui::TreeNodeEx(stock->getSymbol().c_str(), node_flags)) {
                    ImGui::Indent();

                    ImGui::Text("Price: %.2f", stock->getPrice());
                    // ImGui::Dummy(ImVec2(0, 500));
                    if (ImPlot::BeginPlot("Price")) {
                        
                        std::vector<double> prices = stock->getPrices();
                        std::vector<double> times = stock->getTimes();

                        double currentTime = times[times.size() - 1];
                        double lowestPrice = stock->getLowestPrice() - 10;
                        double highestPrice = stock->getHighestPrice() + 10;
                        
                        ImPlot::SetupAxesLimits(
                            0.0, 
                            currentTime + 2.0, 
                            lowestPrice, 
                            highestPrice, 
                            ImPlotCond_Always
                        );

                        ImPlot::PlotLine(
                            "FAKE",
                            times.data(),
                            prices.data(),
                            static_cast<int>(prices.size())
                        );
                        ImPlot::EndPlot();
                    }

                    ImGui::Unindent();
                    ImGui::TreePop();
                }

                if (ImGui::IsItemClicked()) {
                    selected_symbol = stock->getSymbol();
                }
            }
            ImGui::EndTable();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void UI::drawUsers(WindowSize size) {
    ImGui::SetNextWindowPos(ImVec2(size.x, size.y));
    ImGui::SetNextWindowSize(ImVec2(size.width, size.height));
    ImGui::Begin(
        "Users",
        nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
    );
    
    ImGui::End();
}
