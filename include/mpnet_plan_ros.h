/** 
 * Defines the class for mpnet local planner
 */
#include <memory>
#include <torch/script.h>

#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>

#include <tf2_ros/buffer.h>

#include <costmap_2d/costmap_2d_ros.h>
#include <costmap_2d/costmap_2d.h>

#include <base_local_planner/world_model.h>
#include <base_local_planner/costmap_model.h>

#include <ompl/base/ScopedState.h>
#include <ompl/base/spaces/DubinsStateSpace.h>
#include <ompl/geometric/SimpleSetup.h>

namespace ob = ompl::base;
namespace og = ompl::geometric;

namespace mpnet_local_planner{

    class MpnetLocalPlanner{
        /**
         * @class MpnetLocalPlanner
         * @brief Provides an interface for the mpnet local planner, with a model precitive controller
         */

        public:
            tf2_ros::Buffer* tf_;
            costmap_2d::Costmap2DROS* navigation_costmap_ros_;
            costmap_2d::Costmap2D* costmap_;
            base_local_planner::WorldModel* world_model_;
            bool initialized_;

            /**
             * @brief Compute the velocity commmand to send to the base
             */
            bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel);

            /**
             * @brief Check if the goal pose has been achieved by the local planner
             */
            bool isGoalReach();
            
            /**
             * @brief Set the plan that the local planner is following
             * @param plan The plan to pass to the local planner
             * @return True if the plan was updated successfully, false otherwise
             */
            bool setPlan(const std::vector<geometry_msgs::PoseStamped>& plan);

            /**
             * @brief Constructs the local planner
             * @param name The name given to this instance of the local planner
             * @param tf A pointer to a transform listener
             * @param costmap_ros The costmap map to use for assigning costs to local plans
             */
            void initialize(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros);
            

            /**
             * @brief Bundle the current pose and target pose to a normalized torch tensor
             * @param start The current position of the robot
             * @param goal The goal to be achieved in the local costmap
             * @param origin_x The x co-ordinate of the origin of the local costmap
             * @param origin_y The y co-ordinate of the origin of the local costmap
             */
            torch::Tensor getInputTensor(const ob::ScopedState<> &start, const ob::ScopedState<> &goal, double origin_x, double origin_y);

            /**
             * Destructor for the interface
             */
            ~MpnetLocalPlanner();

        private:
            MpnetLocalPlanner();
        
    };
}