//
// Created by filip on 18.12.2022.
//

#include "BuildGraph.h"

BuildGraph::BuildGraph(std::vector<std::optional<Target>>&& targets)
    // add 1, for comfortable access to elements
  : _targets(std::move(targets))
{
}

const Target &BuildGraph::get_target(size_t id) const {
  return _targets.at(id).value();
}
