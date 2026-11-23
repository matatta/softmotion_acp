#include <atomic>
#include <cstring>

constexpr size_t RING_CAPACITY = 1024;
struct Message {
	uint64_t id;
	uint32_t value;
	char    text[32];
};

struct RingQueue {
	alignas(64) std::atomic<size_t> wr_idx;
	alignas(64) std::atomic<size_t> rd_idx;
	Message buf[RING_CAPACITY];
};

inline void ring_enqueue(RingQueue* queue, const Message& msg) {
	size_t wr = queue->wr_idx.load(std::memory_order_relaxed);
	size_t rd = queue->rd_idx.load(std::memory_order_acquire);

	queue->buf[wr % RING_CAPACITY] = msg;

	size_t new_wr = wr + 1;	
	while (new_wr - rd > RING_CAPACITY) {
		rd++;
	}

	if (rd != queue->rd_idx.load(std::memory_order_relaxed)) {
		queue->rd_idx.store(rd, std::memory_order_release);
	}

	queue->wr_idx.store(new_wr, std::memory_order_release);
}

inline bool ring_dequeue(RingQueue* queue, Message& msg) {
	size_t r = queue->rd_idx.load(std::memory_order_relaxed);
	size_t w = queue->wr_idx.load(std::memory_order_acquire);

	if (w != r) {
		msg = queue->buf[r % RING_CAPACITY];
		queue->rd_idx.store(r + 1, std::memory_order_release);
	}

	return w != r;
}

//#include <sys/mman.h>
//#include <fcntl.h>
//#include <unistd.h>
//#include <iostream>
//
//// 打开或创建共享内存
//RingQueue* create_shm_ring() {
//	const char* shm_name = "/my_ring_queue";
//	int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
//	ftruncate(fd, sizeof(RingQueue));
//
//	void* ptr = mmap(nullptr, sizeof(RingQueue),
//		PROT_READ | PROT_WRITE,
//		MAP_SHARED, fd, 0);
//	close(fd);
//
//	// 首次使用需要初始化（只做一次）
//	RingQueue* q = static_cast<RingQueue*>(ptr);
//	if (queue->wr_idx.load() == 0 && queue->rd_idx.load() == 0) {
//		queue->wr_idx.store(0, std::memory_order_relaxed);
//		queue->rd_idx.store(0, std::memory_order_relaxed);
//	}
//	return q;
//}
//
//// 使用示例
//int main() {
//	RingQueue* q = create_shm_ring();
//
//	// 生产者
//	Message m{};
//	m.id = 123;
//	ring_enqueue(q, m);
//
//	// 消费者
//	Message out{};
//	if (ring_dequeue(q, out)) {
//		std::cout << out.id << std::endl;
//	}
//
//	return 0;
//}