#include <iostream>

#include "util/serializeManager/SerializeManager.h"

int main() {
    SerializeManager::loadFile("temp", "test");
    return 0;
}
