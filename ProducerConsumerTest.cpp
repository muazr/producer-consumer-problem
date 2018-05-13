#include "ConcurrentQueue.h"

#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <random>

#define BUFFER_SIZE 10
#define MIN_INTERVAL_MS 0
#define MAX_INTERVAL_MS 500
#define MAX_ITEM_COUNT 100

std::random_device rd;
std::mt19937 mt(rd());
auto dist = std::uniform_int_distribution<int>(MIN_INTERVAL_MS, MAX_INTERVAL_MS);

void producer(ConcurrentQueue<int> &buffer) {
	static std::atomic<int> prod_id{1};
	int cur_prod_id = prod_id++;

	while (true) {
		static std::atomic<int> item{0};
		int cur_item = item++;

		if (cur_item >= MAX_ITEM_COUNT) {
			return;
		}

		std::cout << "producer" << cur_prod_id << " pushing " << cur_item << "...\n";
		buffer.push(cur_item);
		std::cout << "producer" << cur_prod_id << " pushed " << cur_item << "!\n";

		//auto interval = std::chrono::milliseconds(dist(mt));
		//std::cout << __FUNCTION__ << " interval: " << interval.count() << "\n";
		//std::this_thread::sleep_for(interval);
	}
}

void consumer(ConcurrentQueue<int> &buffer) {
	static std::atomic<int> con_id{1};
	int cur_con_id = con_id++;

	while (true) {
		std::cout << "consumer" << cur_con_id << " popping...\n";
		int item = 0;
		buffer.pop(item);
		std::cout << "consumer" << cur_con_id << " popped " << item << "!\n";

		//auto interval = std::chrono::milliseconds(dist(mt));
		//std::cout << __FUNCTION__ << " interval: " << interval.count() << "\n";
		//std::this_thread::sleep_for(interval);
	}
}

int main() {
	ConcurrentQueue<int> buffer(BUFFER_SIZE);

	std::thread p1(producer, std::ref(buffer));
	std::thread p2(producer, std::ref(buffer));
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::thread c1(consumer, std::ref(buffer));
	std::thread c2(consumer, std::ref(buffer));
	std::thread c3(consumer, std::ref(buffer));

	p1.join();
	p2.join();
	c1.join();
	c2.join();
	c3.join();

	return EXIT_SUCCESS;
}
