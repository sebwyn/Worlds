#pragma once

#include "Worlds/Core/Application.hpp"
#include "Worlds/Core/Log.hpp"

extern Worlds::Application *Worlds::CreateApplication(int argc, char** argv);

int main(int argc, char **argv) {

    auto app = Worlds::CreateApplication(argc, argv);
    app->Run();
    delete app;

    return 0;
}
