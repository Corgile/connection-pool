//
// Created by brian on 10/12/23.
//

#ifndef CONNECTION_POOL_CONFIG_HPP
#define CONNECTION_POOL_CONFIG_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
//#include <dbg.h>

namespace xhl {
	struct conn_config {
		/// 连接参数
		std::string url;
		std::string tb;
		int32_t max_idle{60};
		int32_t timeout_sec{20};

		conn_config& operator=(const conn_config& other) {
			if (this == &other) {
				return *this; // 处理自赋值情况
			}
			url = other.url;
			tb = other.tb;
			max_idle = other.max_idle;
			timeout_sec = other.timeout_sec;
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

		config(conn_config& _cf, pool_config& _pf) : pf(_pf), cf(_cf) {}

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
