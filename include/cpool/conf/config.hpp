//
// Created by brian on 10/12/23.
//

#ifndef CONNECTION_POOL_CONFIG_HPP
#define CONNECTION_POOL_CONFIG_HPP

#include <fstream>
#include <iostream>
#include <string>

namespace xhl {
	struct conn_config {
/// 连接参数
		union _sink {
			std::string value;
			std::string servers;
			std::string db_file;

			_sink() : value() {};

			~_sink() {}
		} sink;

		union _vtb {
			std::string value;
			std::string topic;
			std::string table;

			_vtb() : value() {}

			~_vtb() {}
		} vtb;

		int32_t partition{0};
		int32_t max_idle{60};
		int32_t timeout_sec{20};

		operator std::string() const {
			return
					std::string("conn_config{.sink=")
							.append(this->sink.value)
							.append(", vtb=")
							.append(this->vtb.value)
							.append(", .partition=")
							.append(std::to_string(partition))
							.append(", .max_size=")
							.append(std::to_string(max_idle))
							.append(", .timeout_sec=")
							.append(std::to_string(timeout_sec))
							.append("}");
		}

		[[nodiscard]] std::string to_string() const {
			return this->operator std::string();
		}

		conn_config& operator=(const conn_config& other) {
			if (this == &other) {
				return *this; // 防止自赋值
			}
			partition = other.partition; // 复制数据
			max_idle = other.max_idle;
			timeout_sec = other.timeout_sec;
			sink.value = other.sink.value;
			vtb.value = other.vtb.value;
			return *this;
		}

	};

	struct pool_config {
		int32_t init_size{3};
		int32_t max_size{5};

		pool_config& operator=(const pool_config& other) {
			if (this == &other) {
				return *this; // 处理自赋值情况
			}
			init_size = other.init_size;
			max_size = other.max_size;
			return *this;
		}
	};

	struct config {
		conn_config cf;
		pool_config pf;

		config(conn_config& _cf, pool_config& _pf) {
			pf = _pf;
			cf = _cf;
		}

		config& operator=(const config& other) {
			if (this == &other) {
				return *this;
			}
			cf = other.cf;
			pf = other.pf;
			return *this;
		}
	};
} // namespace xhl



#endif //CONNECTION_POOL_CONFIG_HPP
