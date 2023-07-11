#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TwistStamped.h>
#include <geometry_msgs/Twist.h>
#include <tf2/LinearMath/Quaternion.h>
float pose[13] = {};
float covariance[36] = {};
int i = 0;
int j = 0;
void poseCallback(const nav_msgs::Odometry::ConstPtr& msg){
 static tf2_ros::TransformBroadcaster br;
 geometry_msgs::TransformStamped transformStamped;
 transformStamped.header.stamp = ros::Time::now();
 transformStamped.header.frame_id = "odom";
 transformStamped.child_frame_id = "base_footprint";
 transformStamped.transform.translation.x = msg->pose.pose.position.x;
 transformStamped.transform.translation.y = msg->pose.pose.position.y;
 transformStamped.transform.translation.z = msg->pose.pose.position.z;
 transformStamped.transform.rotation.x = msg->pose.pose.orientation.x;
 transformStamped.transform.rotation.y = msg->pose.pose.orientation.y;
 transformStamped.transform.rotation.z = msg->pose.pose.orientation.z;
 transformStamped.transform.rotation.w = msg->pose.pose.orientation.w;

 br.sendTransform(transformStamped);

 ROS_INFO("TFsend");

 pose[0] = msg->pose.pose.position.x;
 pose[1] = msg->pose.pose.position.y;
 pose[2] = msg->pose.pose.position.z;
 pose[3] = msg->pose.pose.orientation.x;
 pose[4] = msg->pose.pose.orientation.y;
 pose[5] = msg->pose.pose.orientation.z;
 pose[6] = msg->pose.pose.orientation.w;
 pose[7] = msg->twist.twist.linear.x;      //pose[7] = msg->pose.twist.linear.x;
 pose[8] = msg->twist.twist.linear.y;      //pose[8] = msg->pose.twist.linear.y;
 pose[9] = msg->twist.twist.linear.z;      //pose[9] = msg->pose.twist.linear.z;
 pose[10] = msg->twist.twist.angular.x;    //pose[10] = msg->pose.twist.angural.x;
 pose[11] = msg->twist.twist.angular.y;    //pose[11] = msg->pose.twist.angural.y;
 pose[12] = msg->twist.twist.angular.z;    //pose[12] = msg->pose.twist.angural.z;

 ROS_INFO("Saveodomparam");


while(true){
if(j > 35){
j = 0;
break;
}
else{
covariance[j] = msg->twist.covariance[j];
ROS_INFO("j : %d",j);
j = j+1;

}
}
  //odom_pub.publish(odom);
}
int main(int argc, char** argv){
 ros::init(argc, argv, "tf_lis_bro_node");
 ros::NodeHandle node;
 ros::Subscriber sub = node.subscribe("/integrated_to_init", 1000, poseCallback);
 ros::Publisher odom_pub = node.advertise<nav_msgs::Odometry>("odom_convert", 1000);
 ros::Rate loop_rate(10);
 nav_msgs::Odometry odom;
while (ros::ok()){
i = 0;

        odom.header.stamp = ros::Time::now();
        odom.header.frame_id = "odom";
        odom.child_frame_id = "base_footprint";
        odom.pose.pose.position.x = pose[0];
        odom.pose.pose.position.y = pose[1];
        odom.pose.pose.position.z = pose[2];
        odom.pose.pose.orientation.x = pose[3];
        odom.pose.pose.orientation.y = pose[4];
        odom.pose.pose.orientation.z = pose[5];
        odom.pose.pose.orientation.w = pose[6];
        odom.twist.twist.linear.x =pose[7];            //odom.pose.twist.linear.x =pose[7];
        odom.twist.twist.linear.y = pose[8];           //odom.pose.twist.linear.y =pose[8];
        odom.twist.twist.linear.z = pose[9];           //odom.pose.twist.linear.z =pose[9];
        odom.twist.twist.angular.x = pose[10];         //odom.pose.twist.angural.x = pose[10];
        odom.twist.twist.angular.y = pose[11];         //odom.pose.twist.angural.y = pose[11];
        odom.twist.twist.angular.z = pose[12];         //odom.pose.twist.angural.z = pose[12];

while(true){
if(i > 35){
break;
}
else{
odom.twist.covariance[i] = covariance[i];
ROS_INFO("i : %d",i);
i = i+1;

}
}

  odom_pub.publish(odom);
  ROS_INFO("Send_odom_topic");
 ros::spinOnce();
 loop_rate.sleep();
}
 return 0;
}
