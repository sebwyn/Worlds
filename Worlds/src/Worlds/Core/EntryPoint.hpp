#pragma once

#include "Worlds/Core/Application.hpp"
#include "Worlds/Core/Log.hpp"

extern Worlds::Application *Worlds::CreateApplication();

int main(int argc, char **argv) {
    Worlds::Log::init();

    auto app = Worlds::CreateApplication();
    app->run();
    delete app;

    return 0;
}
