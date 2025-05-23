#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sqlite3.h>

using ::testing::Eq;
using ::testing::Test;

const auto kPath = "/tmp/sqlitetest";
const auto kJournalPath = "/tmp/sqlitetest-journal";
const auto kIntegrityCheck = "pragma integrity_check;";
const auto kSchema = "create table if not exists t (i text unique);";
const auto kInsert = "insert into t (i) values ('abc');";
const auto kSelect = "select * from t;";
const auto kDelete = "delete from t;";

class ADbWithCorruptJournalFile : public Test {
protected:
    sqlite3* db;
    void SetUp() override
    {
        system((std::string("rm -rf ") + kPath).c_str());
        system((std::string("rm -rf ") + kJournalPath).c_str());
        system((std::string("echo trash > ") + kJournalPath).c_str());
    }
};

TEST_F(ADbWithCorruptJournalFile, WorksIfOpen)
{
    ASSERT_THAT(sqlite3_open(kPath, &db), Eq(SQLITE_OK));

    EXPECT_THAT(sqlite3_exec(db, kIntegrityCheck, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kSchema, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kInsert, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kSelect, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kDelete, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_close_v2(db), Eq(SQLITE_OK));
}

TEST_F(ADbWithCorruptJournalFile, WorksIfOpenEmpty)
{
    system((std::string("> ") + kPath).c_str());

    ASSERT_THAT(sqlite3_open(kPath, &db), Eq(SQLITE_OK));

    EXPECT_THAT(sqlite3_exec(db, kIntegrityCheck, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kSchema, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kInsert, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kSelect, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_exec(db, kDelete, 0, 0, 0), Eq(SQLITE_OK));
    EXPECT_THAT(sqlite3_close_v2(db), Eq(SQLITE_OK));
}
