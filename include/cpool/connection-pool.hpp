//
// Created by brian on 9/9/23.
//

#ifndef CONNECTION_POOL_POOL_HPP
#define CONNECTION_POOL_POOL_HPP

#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <queue>

#include "cpool/conf/config.hpp"
#include "base-connection.hpp"
#include "cpool/conf/pool-config.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>


namespace xhl {
	template<class Connection,
			typename = std::enable_if_t<std::is_base_of<xhl::base_connection, Connection>::value, int>>
	class connection_pool {
	public:
		static_assert(std::is_base_of_v<xhl::base_connection, Connection>,
									"\x1b[031;1m type name `Connection` must be a derived class of `xhl::base_connection`\x1b[0m");

		/// 获取连接池对象实例（懒汉式单例模式，在获取实例时才实例化对象）
		static connection_pool<Connection>* create(xhl::conn_config& _cf, xhl::pool_config& _pf) {
			static connection_pool<Connection>* instance;
			if (instance == nullptr) {
				instance = new connection_pool<Connection>(std::forward<xhl::conn_config&>(_cf),
																									 std::forward<xhl::pool_config&>(_pf));
			}
			return instance;
		}

		/**
		 * 给外部提供接口，从连接池中获取一个可用的空闲连接.
		 * 注意，这里不要直接返回指针，否则还需要定义一个（归还连接）的方法，还要自己去释放该指针。
		 * 这里直接返回一个智能指针，出作用域后自动析构，（只需重定义析构即可--不释放而是归还）
		 */
		std::shared_ptr<Connection> get_connection() {
			std::unique_lock<std::mutex> lock(_queueMutex);
			cv.wait_for(lock, std::chrono::seconds(c_config.timeout_sec),
									[&]() { return not _connectionQue.empty(); });
			if (_connectionQue.empty()) {
				std::printf("%s", "获取连接失败，等待空闲连接超时.\n");
				return nullptr;
			}
			/**
			 * shared_ptr智能指针析构时，会把connection资源直接delete掉，
			 * 相当于调用connection的析构函数，connection就被close掉了。
			 * 这里需要自定义shared_ptr的释放资源的方式，把connection直接归还到queue当中
			 */
			std::shared_ptr<Connection> any_connection(
					_connectionQue.front(),
					/// deleter
					[&](Connection* p_con) {
						/// 这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全操作
						std::unique_lock<std::mutex> lock(_queueMutex);
						/// 在归还回空闲连接队列之前要记录一下连接开始空闲的时刻
						p_con->refreshAliveTime();
						_connectionQue.push(p_con);
					});
			_connectionQue.pop();
			/// 消费者取出一个连接之后，通知生产者，生产者检查队列，如果为空则生产
			cv.notify_all();
			return any_connection;
		}

		~connection_pool() {
			while (not _connectionQue.empty()) {
				auto conn = _connectionQue.front();
				_connectionQue.pop();
				delete conn;
			}
		}

	private:
		/// 单例模式——构造函数私有化
		connection_pool(xhl::conn_config& cf, xhl::pool_config& pf) {
			/// 初始化kafka参数
			this->c_config = cf;
			this->p_config = pf;
			/// 创建初始数量的连接
			for (int i = 0; i < p_config.init_size; ++i) {
				_connectionQue.push(new Connection(std::forward<conn_config&>(this->c_config)));
				_connectionCnt++;
				/// 启动一个新的线程，作为连接的生产者 //守护线程
				std::thread(&connection_pool::produceConnectionTask, this).detach();
				/// 启动一个新的定时线程，扫描超过maxIdleTime时间的空闲连接，并对其进行回收
				std::thread(&connection_pool::scannerConnectionTask, this).detach();
			}
		}

		/// 运行在独立的线程中，专门负责生产新连接
		/// 非静态成员方法，其调用依赖对象，要把其设计为一个线程函数，需要绑定this指针。
		/// 把该线程函数写为类的成员方法，最大的好处是
		/// 非常方便访问当前对象的成员变量。（数据）
		[[noreturn]] void produceConnectionTask() {
			while (true) {
				std::unique_lock<std::mutex> lock(_queueMutex);
				while (not _connectionQue.empty()) {
					/// 队列非空时，此处生产线程进入等待状态,并在进入等待时释放锁，保证消费者线程正常运行
					cv.wait(lock);
				}
				/// 连接数量没有到达上限，继续创建新的连接
				if (_connectionCnt < p_config.max_size) {
					_connectionQue.push(new Connection(std::forward<conn_config&>(this->c_config)));
					_connectionCnt++;
				}
				/// 通知消费者线程，可以消费连接了
				cv.notify_all();
			}
		}

		/// 扫描超过maxIdleTime时间的空闲连接，进行对于连接的回收
		[[noreturn]] void scannerConnectionTask() {
			while (true) {
				// 通过sleep实现定时
				std::this_thread::sleep_for(std::chrono::seconds(c_config.max_idle));
				// 扫描整个队列，释放多余的连接
				std::unique_lock<std::mutex> lock(_queueMutex);
				while (_connectionCnt > p_config.init_size) {
					auto any_connection = _connectionQue.front();
					if (any_connection->getAliveTime() >= (c_config.max_idle * 1000)) {
						_connectionQue.pop();
						_connectionCnt--;
						delete any_connection; // 调用~Connection()释放连接
					} else {
						// 队头的连接没有超过_maxIdleTime，其它连接肯定也没有
						break;
					}
				}
			}
		}

		xhl::conn_config c_config;
		xhl::pool_config p_config;

		std::queue<Connection*> _connectionQue;
		std::mutex _queueMutex;

		/// 记录connection连接的总数量
		std::atomic_int _connectionCnt{};

		/// 设置条件变量，用于连接生产线程和连接消费线程的通信
		std::condition_variable cv;
	};

} // namespace xhl


#endif //CONNECTION_POOL_POOL_HPP
