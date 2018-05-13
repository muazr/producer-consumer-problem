#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>

template<typename T>
class ConcurrentQueue {
	private:
		mutable std::mutex m_mutex;
		std::queue<T> m_queue;
		std::condition_variable m_cond_var;
		const unsigned long m_max_size;

	public:
		ConcurrentQueue(unsigned long size)
				: m_max_size(size) {}

		ConcurrentQueue(const ConcurrentQueue<T> &other)
				: m_max_size(other.m_max_size) {
			std::lock_guard<std::mutex> lock(other.m_mutex);
			m_queue = other.m_queue;
		}

		ConcurrentQueue(const std::queue<T> &non_thread_safe_queue, unsigned long size)
				: m_queue(non_thread_safe_queue),
				  m_max_size(size) {}

		ConcurrentQueue& operator=(const ConcurrentQueue &thread_safe_queue) = delete;

		~ConcurrentQueue() = default;

		void push(T new_value) {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cond_var.wait(lock, [this]{ return m_queue.size() < m_max_size; });
			m_queue.push(new_value);
			m_cond_var.notify_one();
		}

		void pop(T &value) {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cond_var.wait(lock, [this]{ return !m_queue.empty(); });
			value = m_queue.front();
			m_queue.pop();
			m_cond_var.notify_one();
		}

		std::shared_ptr<T> pop() {
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cond_var.wait(lock, [this]{ return !m_queue.empty(); });
			auto result = std::make_shared<T>(m_queue.front());
			m_queue.pop();
			m_cond_var.notify_one();
			return result;
		}

		bool empty() const {
			std::lock_guard<std::mutex> lock(m_mutex);
			return m_queue.empty();
		}
};
#endif
