//
// Created by filip on 18.12.2022.
//

#include "Target.h"

const Target::task_t &Target::get_task() const {
  return _task;
}

const Target::vertex_numbers &Target::get_edges() const {
  return _my_depends;
}

const Target::vertex_numbers &Target::get_depends_on_me() const {
  return _depends_on_me;
}

Target::Target(size_t id, Target::task_t task,
               Target::vertex_numbers &&depends,
               Target::vertex_numbers &&depends_on_me)
    : _id(id),
      _task(task),
      _my_depends(std::move(depends)),
      _depends_on_me(std::move(depends_on_me)) {
}
