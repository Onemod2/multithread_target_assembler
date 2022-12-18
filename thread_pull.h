//
// Created by filip on 18.12.2022.
//

#ifndef TARGET_ASSEMBLER_THREAD_PULL_H
#define TARGET_ASSEMBLER_THREAD_PULL_H

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class thread_pull {
public:
  using task = std::function<void()>;

public:
  explicit thread_pull(size_t thread_number);
  ~thread_pull();
  void join();
  void submit(task &&t);

private:
  thread_pull(const thread_pull &) = delete;
  thread_pull(thread_pull &&) = delete;
  thread_pull &operator=(thread_pull &&) = delete;
  thread_pull &operator=(const thread_pull &) = delete;

  void work();
private:
  std::deque<task> _tasks;
  std::vector<std::thread> _workers;
  std::mutex _mutex;
  std::condition_variable _have_task;
};


#endif//TARGET_ASSEMBLER_THREAD_PULL_H
