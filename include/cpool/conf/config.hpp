//
// Created by brian on 10/12/23.
//

#ifndef CONNECTION_POOL_CONFIG_HPP
#define CONNECTION_POOL_CONFIG_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <any>

namespace xhl {

	class config {

	private:
		std::any callback;

	public:
		template<typename Signature>
		void setCallback(std::function<Signature> func) {
			callback = std::make_any<std::function<Signature>>(func);
		}

		template<typename... Args>
		auto call(Args... args) {
			if (callback.has_value()) {
				try {
					return std::any_cast<std::function<decltype(args)...>>(callback)(args...);
				} catch (const std::bad_any_cast& e) {
					std::cerr << "Error: " << e.what() << std::endl;
				}
			}
			// Return a default value or handle the case when no function is set.
			return std::declval<Args...>();
		}
	};
} // namespace xhl



#endif //CONNECTION_POOL_CONFIG_HPP
