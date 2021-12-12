#include "Application.h"
#include "DllLoader.h"

#include <memory>


int main()
{
    auto app = std::make_unique<Application>();
    app->run();

    return 0;
}
