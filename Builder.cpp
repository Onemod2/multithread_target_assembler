//
// Created by filip on 18.12.2022.
//

#include <atomic>
#include <unordered_map>

#include "Builder.h"

Builder::Builder(size_t num_threads)
    : _executor(num_threads) {
}

void Builder::execute(const BuildGraph &graph, size_t target_id) {
  auto marks = std::make_shared<Marks>();
  auto counters = std::make_shared<Counter>();

  auto& target = graph.get_target(target_id);
  dfs(graph, target, marks, counters);

  std::unique_lock guard(_mutex);
  _build_done.wait(guard,
                   [counters, target_id]() {
                     return !counters->at(target_id);
                   }
 );
}

void Builder::dfs(const BuildGraph &graph,
                  const Target &t,
                  MarksPtr marks,
                  CounterPtr counters)
{
  size_t target_id = t.get_id();
  if (auto it = marks->find(target_id); it != marks->end()) {
    if (it->second == MarkColor::gray) {
      throw std::runtime_error("error. graph has a cycle");
    }
    return; // already black
  }
  auto &&depend_targets= t.get_edges();
  // every dependent target will have executed before main target
  counters->emplace(target_id, depend_targets.size());
  if (depend_targets.empty()) {
    auto wrapped_function = [this, &graph, &t, counters]() {
      task_wrapper(graph, t, counters);
    };
    _executor.submit(std::move(wrapped_function));
  }
  marks->emplace(target_id, MarkColor::gray);
  for (auto depend_target_id : depend_targets) {
    dfs(graph, graph.get_target(depend_target_id), marks, counters);
  }
  marks->at(target_id) = MarkColor::black;
}

void Builder::task_wrapper(const BuildGraph &graph,
                           const Target &target,
                           CounterPtr counters)
{
  target.get_task()();
  for (auto target_id : target.get_depends_on_me()) {
    // if last dependence has ended launch a main target
    if (size_t count = --(counters->at(target_id)); !count) {
      auto&& t = graph.get_target(target_id);
      auto wrapped_function = [this, &graph, &t, counters]() {
        task_wrapper(graph, t, counters);
      };
      _executor.submit(std::move(wrapped_function));
    }
  }
  _build_done.notify_one();
}

Builder::~Builder() {
  _executor.join();
}
