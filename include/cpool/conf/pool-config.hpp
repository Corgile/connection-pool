//
// Created by brian on 11/2/23.
//

#ifndef CONNECTION_POOL_POOL_CONFIG_HPP
#define CONNECTION_POOL_POOL_CONFIG_HPP

#include "config.hpp"

namespace xhl {
	struct pool_config : public config {
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
}

#endif //CONNECTION_POOL_POOL_CONFIG_HPP
