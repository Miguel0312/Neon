#include <iostream>
#include <mutex>
#include <utils/threadPool.h>

namespace Neon {
ThreadPool::ThreadPool() {
  const unsigned int threadCount = std::thread::hardware_concurrency();
  std::cout << "Creating a pool with " << threadCount << " threads\n";
  m_threads.reserve(threadCount);
  for (unsigned int i = 0; i < threadCount; i++) {
    m_threads.emplace_back(std::thread(&ThreadPool::threadLoop, this));
  }
}

void ThreadPool::queueJob(const std::function<void()> &job) {
  {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_jobs.push(job);
  }
  m_mutexCondition.notify_one();
}

bool ThreadPool::busy() {
  bool poolBusy;
  {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    poolBusy = !m_jobs.empty();
  }
  return poolBusy;
}

void ThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_shouldTerminate = true;
  }

  m_mutexCondition.notify_all();
  for (auto &thread : m_threads) {
    thread.join();
  }
  m_threads.clear();
}

void ThreadPool::threadLoop() {
  while (true) {
    std::function<void()> job;
    {
      std::unique_lock<std::mutex> lock(m_queueMutex);
      m_mutexCondition.wait(
          lock, [this] { return !m_jobs.empty() || m_shouldTerminate; });
      if (m_shouldTerminate) {
        return;
      }
      job = m_jobs.front();
      m_jobs.pop();
    }
    job();
  }
}

} // namespace Neon
