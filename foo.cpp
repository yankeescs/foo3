#include <atomic>
#include <cstdint>
#include <cstring>
#include <thread>

constexpr auto kBufSize = 1024;

uint8_t gBuf[3][kBufSize];

std::atomic_int shared_buf_num{0};
int producer_buf_num{1};
int consumer_buf_num{2};

void producer_task() {
  while (1) {
    // produce
    uint8_t produced_data[kBufSize] = {};
    auto *buf_ptr = gBuf[producer_buf_num];
    memcpy(buf_ptr, produced_data, kBufSize);

    // swap buffers
    producer_buf_num = shared_buf_num.exchange(producer_buf_num);
  }
}

void consumer_task() {
  while (1) {
    // swap buffers
    consumer_buf_num = shared_buf_num.exchange(consumer_buf_num);

    // consumeq
    uint8_t consumed_data[kBufSize];
    auto *buf_ptr = gBuf[consumer_buf_num];
    memcpy(consumed_data, buf_ptr, kBufSize);
  }
}

int main() {
  std::thread producer{producer_task}, consumer{consumer_task};
  producer.join();
  consumer.join();
  return 0;
}
