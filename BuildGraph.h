//
// Created by filip on 18.12.2022.
//

#ifndef TARGET_ASSEMBLER_BUILDGRAPH_H
#define TARGET_ASSEMBLER_BUILDGRAPH_H

#include <cstdlib>
#include <optional>
#include <vector>

#include "Target.h"


class BuildGraph {
public:
  explicit BuildGraph(std::vector<std::optional<Target>>&& targets);
  const Target& get_target(size_t id) const;

private:
  std::vector<std::optional<Target>> _targets;
};



#endif//TARGET_ASSEMBLER_BUILDGRAPH_H
