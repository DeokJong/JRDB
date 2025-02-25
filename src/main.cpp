#include <iostream>
#include "storage/StorageEngine.h"

int main() {
    StorageEngine se("./temp");

    se.createTable("users", {"id", "name", "age"}, {"INT", "VARCHAR", "INT"});

    se.insertRecord("users", {"1", "Alice", "24"});
    se.insertRecord("users", {"2", "Bob", "30"});

    std::cout << "=== table users data ===" << std::endl;
    se.selectRecords("users");
    se.insertRecord("users", { "3", "Jin", "99" });
    se.selectRecords("users");
    return 0;
}
