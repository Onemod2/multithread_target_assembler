//
// Created by filip on 18.12.2022.
//

#ifndef TARGET_ASSEMBLER_TARGET_H
#define TARGET_ASSEMBLER_TARGET_H

#include <functional>
#include <vector>

struct Target {
  using vertex_numbers = std::vector<size_t>;
  using task_t = std::function<void()>;

  const task_t &get_task() const;
  const vertex_numbers &get_depends_on_me() const;
  const vertex_numbers &get_edges() const;
  inline size_t get_id() const {
    return _id;
  };
  Target(size_t id, task_t task,
         vertex_numbers &&depends, vertex_numbers &&depends_on_me);

  size_t _id;
  task_t _task;
  vertex_numbers _my_depends;
  vertex_numbers _depends_on_me;
};

#endif//TARGET_ASSEMBLER_TARGET_H
