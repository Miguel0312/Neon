#ifndef NEON_THREAD_POOL_H
#define NEON_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace Neon {
class ThreadPool final {
public:
  ThreadPool();

  ThreadPool(unsigned int threadCount);

  ~ThreadPool() = default;

  void queueJob(const std::function<void()> &job);

  void stop();

  bool busy();

private:
  void threadLoop();

  bool m_shouldTerminate = false;
  std::mutex m_queueMutex;
  std::condition_variable m_mutexCondition;
  std::vector<std::thread> m_threads;
  std::queue<std::function<void()>> m_jobs;
};
} // namespace Neon

#endif
