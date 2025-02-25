#include "StorageEngine.h"

StorageEngine::StorageEngine(const std::string& directory) : dataDirectory(directory) {}

void StorageEngine::createTable(const std::string& tableName,
    const std::vector<std::string>& columns,
    const std::vector<std::string>& types) {
    TableSchema tableSchema;
    tableSchema.tableName = tableName;
    tableSchema.columnNames = columns;
    tableSchema.columnTypes = types;

    tableSchema.saveSchemaToFile(this->dataDirectory);

    std::ofstream dataFile(this->dataDirectory + "/" + tableName + ".db");
    if (!dataFile) {
        std::cerr << "Failed to create DB file: " << tableName << std::endl;
        return;
    }
    dataFile.close();

    std::cout << "Table " << tableName << " created successfully!" << std::endl;
}

void StorageEngine::insertRecord(const std::string& tableName, const std::vector<std::string>& values) {
    if (openTables.find(tableName) == openTables.end()) {
        loadTable(tableName);
    }
    if (openTablesSchema.find(tableName) == openTablesSchema.end()) {
        std::cerr << "Error: Schema for table " << tableName << " is missing!" << std::endl;
        return;
    }

    TableSchema& schema = openTablesSchema[tableName];

    if (values.size() != schema.columnNames.size()) {
        std::cerr << "Error: Column count mismatch! Expected " << schema.columnNames.size()
            << ", but got " << values.size() << "." << std::endl;
        return;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        if (schema.columnTypes[i] == "INT") {
            for (char c : values[i]) {
                if (!std::isdigit(c)) {
                    std::cerr << "Error: Column '" << schema.columnNames[i]
                        << "' expects an INT but received '" << values[i] << "'" << std::endl;
                    return;
                }
            }
        }
    }

    std::fstream& file = openTables[tableName];

    if (!file.is_open()) {
        std::cerr << "Error: Failed to open table " << tableName << std::endl;
        return;
    }

    for (const auto& value : values) {
        file << value << " ";
    }
    file << std::endl;

    file.flush();

    std::cout << "Inserted record into " << tableName << " successfully!" << std::endl;
}

void StorageEngine::deleteRecord(const std::string& tableName, const std::string& columnName, const std::string& value) {
    std::string filePath = dataDirectory + "/" + tableName + ".db";
    std::string tempFilePath = dataDirectory + "/" + tableName + "_temp.db";

    std::ifstream inputFile(filePath);
    if (!inputFile) {
        std::cerr << "Error: Could not open table file " << filePath << std::endl;
        return;
    }

    std::ofstream tempFile(tempFilePath);
    if (!tempFile) {
        std::cerr << "Error: Could not create temp file " << tempFilePath << std::endl;
        return;
    }

    std::string line;
    bool deleted = false;

    // 첫 번째 컬럼이 `columnName`인지 확인 (테이블 스키마 로드 필요)
    std::getline(inputFile, line);
    std::vector<std::string> columnNames;
    std::istringstream headerStream(line);
    std::string col;
    while (headerStream >> col) {
        columnNames.push_back(col);
    }

    auto columnIt = std::find(columnNames.begin(), columnNames.end(), columnName);
    if (columnIt == columnNames.end()) {
        std::cerr << "Error: Column " << columnName << " not found in table " << tableName << std::endl;
        return;
    }

    int targetColumnIndex = std::distance(columnNames.begin(), columnIt);
    tempFile << line << "\n";  // 헤더 복사

    while (std::getline(inputFile, line)) {
        std::istringstream stream(line);
        std::vector<std::string> row;
        std::string word;
        while (stream >> word) {
            row.push_back(word);
        }

        if (row.size() > targetColumnIndex && row[targetColumnIndex] == value) {
            deleted = true;
            continue;  // ✅ 삭제된 레코드는 새 파일에 쓰지 않음
        }

        tempFile << line << "\n";  // ✅ 삭제되지 않은 레코드만 복사
    }

    inputFile.close();
    tempFile.close();

    if (deleted) {
        std::remove(filePath.c_str());
        std::rename(tempFilePath.c_str(), filePath.c_str());
        std::cout << "Record deleted successfully from " << tableName << std::endl;
    }
    else {
        std::cerr << "Error: No matching record found to delete in " << tableName << std::endl;
    }
}


void StorageEngine::selectRecords(const std::string& tableName) {
    if (openTablesSchema.find(tableName) == openTablesSchema.end()) {
        std::cerr << "Error: Schema for table " << tableName << " is missing!" << std::endl;
        return;
    }

    std::ifstream file(dataDirectory + "/" + tableName + ".db");
    if (!file) {
        std::cerr << "Error: Could not open table " << tableName << std::endl;
        return;
    }

    std::string line;
    std::cout << "=== Records in table " << tableName << " ===" << std::endl;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

void StorageEngine::dropTable(const std::string& tableName) {
    closeTable(tableName);

    std::string tableFilePath = dataDirectory + "/" + tableName + ".db";
    std::string schemaFilePath = dataDirectory + "/" + tableName + ".schema";

    try {
        if (std::filesystem::remove(tableFilePath) && std::filesystem::remove(schemaFilePath)) {
            std::cout << "Table " << tableName << " deleted successfully!" << std::endl;
        }
        else {
            std::cerr << "Error: Failed to delete table " << tableName << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

void StorageEngine::loadTable(const std::string& tableName) {
    std::string tableFilePath = dataDirectory + "/" + tableName + ".db";
    std::string schemaFilePath = dataDirectory + "/" + tableName + ".schema";

    std::fstream file(tableFilePath, std::ios::in | std::ios::out | std::ios::app);
    if (!file) {
        std::cerr << "Error: Could not open table " << tableName << std::endl;
        return;
    }

    TableSchema tableSchema = TableSchema::loadSchemaFromFile(this->dataDirectory, tableName);

    openTables[tableName] = std::move(file);
    openTablesSchema[tableName] = std::move(tableSchema);

    std::cout << "Loaded table " << tableName << " successfully!" << std::endl;
}

void StorageEngine::closeTable(const std::string& tableName) {
    if (openTables.find(tableName) != openTables.end()) {
        openTables[tableName].close();
        openTables.erase(tableName);
    }

    if (openTablesSchema.find(tableName) != openTablesSchema.end()) {
        openTablesSchema.erase(tableName);
    }

    std::cout << "Closed table " << tableName << std::endl;
}

void StorageEngine::closeAllTables() {
    for (auto& table : openTables) {
        table.second.close();
    }
    openTables.clear();
    openTablesSchema.clear();
    std::cout << "All tables closed." << std::endl;
}
