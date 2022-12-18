//
// Created by filip on 18.12.2022.
//

#ifndef TARGET_ASSEMBLER_BUILDER_H
#define TARGET_ASSEMBLER_BUILDER_H

#include <cstdlib>
#include <map>
#include <atomic>

#include "BuildGraph.h"
#include "thread_pull.h"

class Builder {
public:
  explicit Builder(size_t num_threads);
  ~Builder();

  void execute(const BuildGraph &build_graph, size_t target_id);

private:
  // helper enum for find a cycle in graph
  enum class MarkColor: uint8_t {
    gray = 0u,
    black
  };
  using Counter = std::map<size_t, std::atomic_size_t>;
  using CounterPtr = std::shared_ptr<Counter>;
  using Marks = std::unordered_map<size_t, MarkColor>;
  using MarksPtr = std::shared_ptr<Marks>;

  // std::map instead std::unordered_map because of invalidation of iterator
  void task_wrapper(const BuildGraph &graph,
                    const Target &target,
                    CounterPtr counters);

  void dfs(const BuildGraph &build_graph,
           const Target &t,
           MarksPtr marks,
           CounterPtr counters);

  thread_pull _executor;
  std::condition_variable _build_done;
  std::mutex _mutex;
};

#endif//TARGET_ASSEMBLER_BUILDER_H
