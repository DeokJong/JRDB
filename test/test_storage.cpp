#include <gtest/gtest.h>
#include "../src/storage/StorageEngine.h"
#include <vector>
#include <random>

// 테스트용 StorageEngine 생성
class StorageEngineTest : public ::testing::Test {
protected:
    void SetUp() override {
        se = new StorageEngine("./temp");
    }

    void TearDown() override {
        delete se;
    }

    StorageEngine* se;
};

// ✅ 1. 테이블 생성 테스트 (다양한 컬럼 수 & 데이터 타입)
struct TableTestCase {
  std::string tableName;
  std::vector<std::string> columns;
  std::vector<std::string> types;
};

TEST_F(StorageEngineTest, CreateTableTests) {
  std::vector<TableTestCase> testCases = {
      { "users", {"id", "name", "age"}, {"INT", "VARCHAR", "INT"} },
      { "products", {"product_id", "price"}, {"INT", "FLOAT"} },
      { "logs", {"timestamp", "event"}, {"VARCHAR", "TEXT"} },
      { "employees", {"emp_id", "name", "department", "salary"}, {"INT", "VARCHAR", "VARCHAR", "FLOAT"} }
  };

  for (const auto& testCase : testCases) {
      se->createTable(testCase.tableName, testCase.columns, testCase.types);
  }
}


// ✅ 2. 레코드 삽입 테스트 (정상적인 입력 & 잘못된 입력)
TEST_F(StorageEngineTest, InsertRecordTests) {
    se->createTable("users", {"id", "name", "age"}, {"INT", "VARCHAR", "INT"});

    std::vector<std::vector<std::string>> validRecords = {
        {"1", "Alice", "25"},
        {"2", "Bob", "30"},
        {"3", "Charlie", "22"},
    };

    for (const auto& record : validRecords) {
        EXPECT_NO_THROW(se->insertRecord("users", record));
    }

    std::vector<std::vector<std::string>> invalidRecords = {
        {"Alice", "25"},     // 컬럼 개수 불일치
        {"4", "David", "xyz"},  // 잘못된 데이터 타입
        {"5"} // 너무 적은 데이터 개수
    };

    for (const auto& record : invalidRecords) {
        EXPECT_ANY_THROW(se->insertRecord("users", record));
    }
}

// ✅ 3. 레코드 조회 테스트 (랜덤 데이터 삽입 후 조회)
TEST_F(StorageEngineTest, SelectRecordsTests) {
    se->createTable("logs", {"timestamp", "event"}, {"VARCHAR", "TEXT"});

    std::vector<std::vector<std::string>> logs = {
        {"2024-02-25 12:00:00", "User Login"},
        {"2024-02-25 12:10:00", "File Uploaded"},
        {"2024-02-25 12:15:00", "User Logout"},
    };

    for (const auto& log : logs) {
        se->insertRecord("logs", log);
    }

    testing::internal::CaptureStdout();
    se->selectRecords("logs");
    std::string output = testing::internal::GetCapturedStdout();

    for (const auto& log : logs) {
        std::string expectedOutput = log[0] + " " + log[1];
        EXPECT_NE(output.find(expectedOutput), std::string::npos);
    }
}

// ✅ 4. 레코드 삭제 테스트 (존재하는 데이터 & 없는 데이터 삭제)
TEST_F(StorageEngineTest, DeleteRecordTests) {
  se->createTable("tasks", {"task_id", "task_name"}, {"INT", "VARCHAR"});
  se->insertRecord("tasks", {"1", "Write Code"});
  se->insertRecord("tasks", {"2", "Debug Issues"});

  // ✅ 존재하는 데이터 삭제 (예외 발생 X)
  EXPECT_NO_THROW(se->deleteRecord("tasks", "task_id", "1"));

  // ✅ 존재하지 않는 데이터 삭제 (예외 발생 예상)
  EXPECT_ANY_THROW(se->deleteRecord("tasks", "task_id", "100"));

  // ✅ 삭제 후 다시 조회하여 데이터가 남아있는지 확인
  testing::internal::CaptureStdout();
  se->selectRecords("tasks");
  std::string output = testing::internal::GetCapturedStdout();

  EXPECT_EQ(output.find("1 Write Code"), std::string::npos);  // 삭제된 데이터가 출력되지 않아야 함
}


// ✅ 5. 예외 처리 테스트 (잘못된 삽입, 존재하지 않는 테이블 접근)
TEST_F(StorageEngineTest, ExceptionHandlingTests) {
    // 존재하지 않는 테이블 조회
    EXPECT_ANY_THROW(se->selectRecords("non_existent_table"));

    // 존재하지 않는 테이블에 삽입
    EXPECT_ANY_THROW(se->insertRecord("non_existent_table", {"1", "Alice"}));
}

// ✅ 6. 랜덤 데이터 100개 삽입 & 조회 테스트
TEST_F(StorageEngineTest, BulkInsertAndSelectTests) {
    se->createTable("random_users", {"id", "name", "age"}, {"INT", "VARCHAR", "INT"});

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ageDist(18, 65);

    std::vector<std::vector<std::string>> randomRecords;
    for (int i = 0; i < 100; ++i) {
        randomRecords.push_back({std::to_string(i), "User_" + std::to_string(i), std::to_string(ageDist(gen))});
    }

    for (const auto& record : randomRecords) {
        se->insertRecord("random_users", record);
    }

    testing::internal::CaptureStdout();
    se->selectRecords("random_users");
    std::string output = testing::internal::GetCapturedStdout();

    for (const auto& record : randomRecords) {
        std::string expectedOutput = record[0] + " " + record[1] + " " + record[2];
        EXPECT_NE(output.find(expectedOutput), std::string::npos);
    }
}
