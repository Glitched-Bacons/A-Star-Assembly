#include <iostream>

#include "Structures/Node.h"
#include "Structures/Vector2.h"

int main()
{
    as::Node<as::Vector2i> node1({4, 2});
    as::Node<as::Vector2i> node2({2, 4});

    std::cout << node1.getCostTo(node2) << std::endl;

    std::cout << "Hello World!\n";
}
