// Copyright (c) 2020, OUXT-Polaris
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef BEHAVIOR_TREE_ACTION_BUILDER__ACTION_NODE_WITH_ACTION_HPP_
#define BEHAVIOR_TREE_ACTION_BUILDER__ACTION_NODE_WITH_ACTION_HPP_

#include <string>
#include <memory>

#include "behaviortree_cpp_v3/action_node.h"
#include "behaviortree_cpp_v3/bt_factory.h"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

namespace behavior_tree_action_builder
{
template<class ActionT>
class ActionNodeWithAction : public BT::SyncActionNode, public rclcpp::Node
{
public:
  using GoalHandle = rclcpp_action::ClientGoalHandle<ActionT>;
  ActionNodeWithAction(
    const std::string & name,
    const BT::NodeConfiguration & config,
    const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  :  BT::SyncActionNode(name, config), rclcpp::Node(name, options)
  {
    name_ = name;
    setRegistrationID(name);
    action_client_ = rclcpp_action::create_client<ActionT>(
      get_node_base_interface(),
      get_node_graph_interface(),
      get_node_logging_interface(),
      get_node_waitables_interface(),
      name_);
  }

protected:
  std::string name_;
  typename rclcpp_action::Client<ActionT>::SharedPtr action_client_;
  virtual void callbackGoalResponce(std::shared_future<typename GoalHandle::SharedPtr>) = 0;
  virtual void callbackFeedback(
    typename GoalHandle::SharedPtr,
    const std::shared_ptr<typename ActionT::Feedback>) = 0;
  virtual void callbackResult(const typename GoalHandle::WrappedResult &) = 0;
};
}  // namespace behavior_tree_action_builder

#endif  // BEHAVIOR_TREE_ACTION_BUILDER__ACTION_NODE_WITH_ACTION_HPP_
