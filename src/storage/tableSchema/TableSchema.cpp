#include "TableSchema.h"

void TableSchema::saveSchemaToFile(const std::string &directory)
{
    std::string filePath = directory + "/" + tableName + ".schema";
    if (!std::filesystem::exists(directory))
    {
        std::filesystem::create_directories(directory);
    }

    std::ofstream file(filePath);
    if (!file)
    {
        std::cerr << "Error: Could not create schema file " << filePath << std::endl;
        return;
    }

    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        file << columnNames[i] << " " << columnTypes[i] << "\n";
    }

    file.close();
    std::cout << "Schema file saved: " << filePath << std::endl;
}

TableSchema TableSchema::loadSchemaFromFile(const std::string &directory, const std::string &tableName)
{
    TableSchema schema;
    schema.tableName = tableName;

    std::ifstream file(directory + "/" + tableName + ".schema");
    if (file)
    {
        std::string columnName, columnType;
        while (file >> columnName >> columnType)
        {
            schema.columnNames.push_back(columnName);
            schema.columnTypes.push_back(columnType);
        }
    }
    else
    {
        std::cerr << tableName << ".schema load fail!!" << std::endl;
    }
    std::cout << tableName << ".schema load success!!" << std::endl;
    return schema;
}
