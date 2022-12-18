#include <chrono>
#include <iostream>
#include <optional>
#include <thread>
#include <vector>
using namespace std::chrono_literals;

#include "BuildGraph.h"
#include "Builder.h"

struct Edge {
  size_t from;
  size_t to;
};

using customer_info_target = std::pair<size_t, Target::task_t>;

BuildGraph create_graph(std::vector<customer_info_target> &targets_info,
                        std::vector<Edge> &edges) {
  std::vector<std::optional<Target>> targets(targets_info.size(), std::nullopt);
  for (auto [id, task]: targets_info) {
    targets[id].emplace(Target(id, task, {}, {}));
  }
  for (auto [from, to]: edges) {
    targets[from]->_my_depends.push_back(to);
    targets[to]->_depends_on_me.push_back(from);
  }
  return BuildGraph{std::move(targets)};
}

void test1() {
  auto sleep_function =
    [](size_t i) {
      std::this_thread::sleep_for(1s);
      std::cout << std::to_string(i) << " - task has finished" << std::endl;
    };
  std::vector<customer_info_target> targets_info;
  for (size_t i = 0; i < 6; ++i) {
    targets_info.emplace_back(i, std::bind(sleep_function, i));
  }
  std::vector<Edge> edges(
    {{1, 2}, {2, 3},
       {1, 4}, {4, 5}});
  BuildGraph graph = create_graph(targets_info, edges);
  Builder builder(8);
  builder.execute(graph, 1);
}

void test2() {
  auto sleep_function =
    [](size_t i) {
      std::this_thread::sleep_for(1s);
      std::cout << std::to_string(i) << " - task has finished" << std::endl;
    };
  std::vector<customer_info_target> targets_info;
  for (size_t i = 0; i < 5; ++i) {
    targets_info.emplace_back(i, std::bind(sleep_function, i));
  }
  std::vector<Edge> edges(
    {{1, 2}, {2, 3},
     {3, 4}, {4, 2}});
  BuildGraph graph = create_graph(targets_info, edges);
  Builder builder(8);
  builder.execute(graph, 1);
}

int main() {
  //test1();
  test2();

  return 0;
}
