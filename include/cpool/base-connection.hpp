//
// Created by brian on 10/12/23.
//

#ifndef CONNECTION_POOL_BASE_CONNECTION_HPP
#define CONNECTION_POOL_BASE_CONNECTION_HPP

#include <ctime>
#include "cpool/conf/conn-config.hpp"

namespace xhl {

	class base_connection {
	private:
		clock_t _aliveTime{};
	protected:
		xhl::conn_config c_conf;

	public:
		explicit base_connection(xhl::conn_config& conf);

		// 刷新连接的起始空闲时刻
		void refreshAliveTime();

		// 返回连接空闲的时长
		[[nodiscard]] clock_t getAliveTime() const;
	};

} // xhl

#endif //CONNECTION_POOL_BASE_CONNECTION_HPP
