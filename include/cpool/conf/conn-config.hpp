//
// Created by brian on 11/2/23.
//

#ifndef CONNECTION_POOL_CONN_CONFIG_HPP
#define CONNECTION_POOL_CONN_CONFIG_HPP

#include <string>
#include "config.hpp"

namespace xhl {
	struct conn_config : public config {
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
}
#endif //CONNECTION_POOL_CONN_CONFIG_HPP
