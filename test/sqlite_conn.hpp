//
// Created by brian on 10/12/23.
//

#ifndef CONNECTION_POOL_SQLITE_CONN_HPP
#define CONNECTION_POOL_SQLITE_CONN_HPP


#include <string>
#include <sqlite3.h>
#include "dbg.h"
#include "student.hpp"
#include "cpool/base-connection.hpp"

namespace xhl {
	class sqlite_connection : public xhl::base {
	private:
		std::string db_path{"./temp.db"};
		std::string table_name{"test"};
		sqlite3* db_connection{};

	public:
		void createStudentTable() {
			const char* createTableSQL =
					"CREATE TABLE IF NOT EXISTS students ("
					"id INTEGER PRIMARY KEY AUTOINCREMENT,"
					"name TEXT NOT NULL,"
					"age INTEGER NOT NULL,"
					"grade REAL NOT NULL);";

			char* errMsg = nullptr;
			int rc = sqlite3_exec(db_connection, createTableSQL, nullptr, nullptr, &errMsg);
			if (rc not_eq SQLITE_OK) {
				dbg("SQL error: ", errMsg);
				sqlite3_free(errMsg);
			}
		}

		bool tableExists(const char* tableName) {
			const char* checkTableSQL = "SELECT name FROM sqlite_master WHERE type='table' AND name=?;";

			sqlite3_stmt* stmt = nullptr;
			int rc = sqlite3_prepare_v2(db_connection, checkTableSQL, -1, &stmt, nullptr);
			if (rc not_eq SQLITE_OK) {
				dbg("SQL error: ", sqlite3_errmsg(db_connection));
				return false;
			}
			sqlite3_bind_text(stmt, 1, tableName, -1, SQLITE_STATIC);

			rc = sqlite3_step(stmt);
			sqlite3_finalize(stmt);
			return rc == SQLITE_ROW;
		}

		// Function to retrieve and display student records
		void retrieveStudents() {
			std::string selectSQL = std::string("SELECT * FROM ").append(this->table_name).append(";");

			sqlite3_stmt* stmt = nullptr;
			int rc = sqlite3_prepare_v2(db_connection, selectSQL.c_str(), -1, &stmt, nullptr);
			if (rc not_eq SQLITE_OK) {
				dbg("SQL error: ", sqlite3_errmsg(db_connection));
				return;
			}

			while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
				int id = sqlite3_column_int(stmt, 0);
				const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
				int age = sqlite3_column_int(stmt, 2);
				double grade = sqlite3_column_double(stmt, 3);

				std::cout << "ID: " << id
									<< ", Name: " << name
									<< ", Age: " << age
									<< ", Grade: " << grade << std::endl;
			}

			sqlite3_finalize(stmt);
		}

	public:
		sqlite_connection(xhl::conn_config& conf) : base(conf) { // NOLINT(*-explicit-constructor)
			this->db_path = conf.url;
			this->table_name = conf.tb;

			int rc = sqlite3_open(db_path.c_str(), &db_connection);
			if (rc != SQLITE_OK) {
				dbg("无法打开数据库: ", sqlite3_errmsg(db_connection));
				sqlite3_close(db_connection);
			}
			if (not tableExists(this->table_name.c_str())) {
				createStudentTable();
			}
		}

		int insert(student& stu) {
			std::string insertSQL = std::string("INSERT INTO ")
					.append(this->table_name)
					.append(" (name, age, grade) VALUES (?, ?, ?);");

			sqlite3_stmt* stmt = nullptr;
			int rc = sqlite3_prepare_v2(db_connection, insertSQL.c_str(), -1, &stmt, nullptr);
			if (rc != SQLITE_OK) {
				dbg("SQL error: ", sqlite3_errmsg(db_connection));
				return -1;
			}

			sqlite3_bind_text(stmt, 1, stu.name.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_int(stmt, 2, stu.age);
			sqlite3_bind_double(stmt, 3, stu.grade);

			rc = sqlite3_step(stmt);
			if (rc != SQLITE_DONE) {
				dbg("SQL error: ", sqlite3_errmsg(db_connection));
			}

			sqlite3_finalize(stmt);

			return 0;
		}

		~sqlite_connection() {
			//sqlite3_finalize(stmt);
			sqlite3_close(db_connection);
		}

	};
}


#endif //CONNECTION_POOL_SQLITE_CONN_HPP
