#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

class Joystick
{
public:
        Joystick();
        ~Joystick();
private:
        void joyCallback(const sensor_msgs::Joy::ConstPtr& msg);

        ros::NodeHandle nh;

        ros::Publisher velPub;
        ros::Subscriber joySub;

        // parameter
        int linear, angular;
        double lScale, aScale;

        geometry_msgs::Twist vel;
};

Joystick::Joystick()
{
        // publish
        velPub = nh.advertise<geometry_msgs::Twist>("/RosAria/cmd_vel",1);
        // subscribe
        joySub = nh.subscribe<sensor_msgs::Joy>("/joy", 10, &Joystick::joyCallback, this);

        nh.param("axis_linear", linear, linear);
        nh.param("axis_angular", angular, angular);
        nh.param("scale_linear", lScale, lScale);
        nh.param("scale_angular", aScale, aScale);
}

Joystick::~Joystick()
{
}

void Joystick::joyCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
        if(msg->buttons[3]==1 || msg->buttons[6]==1){
                vel.linear.x = lScale*msg->axes[linear];
                vel.angular.z = aScale*msg->axes[angular];
                velPub.publish(vel);
        }
}

int main(int argc, char** argv)
{
        ros::init(argc, argv, "joystick");
        Joystick joy;

        ros::spin();

        return 0;
}
