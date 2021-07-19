#pragma once

#include "Worlds/Core/Application.hpp"

extern Worlds::Application *Worlds::CreateApplication();

int main(int argc, char **argv) {
    auto app = Worlds::CreateApplication();
    app->run();
    delete app;

    return 0;
}
