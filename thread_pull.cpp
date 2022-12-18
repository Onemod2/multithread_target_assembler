//
// Created by filip on 18.12.2022.
//

#include "thread_pull.h"

#include <optional>

thread_pull::thread_pull(size_t thread_number) {
  for (size_t i = 0; i < thread_number; ++i) {
    _workers.emplace_back(&thread_pull::work, this);
  }
}

void thread_pull::submit(task &&t) {
  {
    std::lock_guard guard(_mutex);
    _tasks.emplace_back(std::forward<task>(t));
  }
  _have_task.notify_one();
}

void thread_pull::work() {
  std::optional<task> t;
  while (true) {
    {
      std::unique_lock guard(_mutex);
      _have_task.wait(
        guard, [this]() -> bool { return _tasks.size(); });
      t = std::move(_tasks.front());
      _tasks.pop_front();
    }
    if (!t.value()) {
      return;
    }
    t.value()();
  }
}

void thread_pull::join() {
  { // empty tasks
    std::lock_guard guard(_mutex);
    for (size_t i = 0; i < _workers.size(); i++) {
      _tasks.emplace_back(task());
    }
  }
  _have_task.notify_all();

  for (auto& worker : _workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

thread_pull::~thread_pull() {
  join();
}
