#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// General
//#define SERVER_HOST (char*)"10.10.245.63"
#define SERVER_HOST (char*)"localhost"
#define SERVER_PORT 6665
#define BLOW_ROBOT_FACTOR 2.5
#define MAP_ROTATION -30

// Robot
#define WAYPOINTS_SKIP_NUM 3
#define WAYPOINT_RANGE 2
#define MIN_ANGLE_TO_REQUIRE_ROTATION 2
#define ROTATION_SPEED 0.5
#define DRIVE_SPEED 0.5
#define ROBOT_SIZE 20
#define RESOLUTION_SIZE 5
#define WAYPOINT_TOLERENCE 15
#define ROBOT_START_X 470
#define ROBOT_START_Y 470
#define GOAL_X 650
#define GOAL_Y 500
#define LIDAR_SCOPE 240
#define LIDAR_COUNT 666

// Laser
//#define LASER_SAMPLES_NUM 666
//#define LASER_FOV 240
//#define MIDDLE_SAMPLE_INDEX LASER_SAMPLES_NUM / 2
//#define LASER_MAXIMUM_RANGE 2

// Particle
#define STARTING_PARTICLES 99
#define BELIEF_NORMALIZATION 1.5
#define DISTANCE_THRESHOLD 1
#define YAW_THRESHOLD_DEGREES 120
#define MIN_BELIEF 0.3
#define BELIEF_BIRTH_THRESHOLD 0.8
#define MIN_PARTICLE_NUM 1
#define MAX_PARTICLE_NUM 300
#define PARTICLES_REMOVE 10
#define PARTICLE_BREED_NUM 3
#define PARTICLE_BREED_SPREAD 50
#define PARTICLE_YAW_BREED_SPREAD 10
#define NEAR_OBSTACLE_RADIUS 2

// Debug
#define DEBUG_BLOWN_MAP true
#define DEBUG_ASTAR_MAP false
#define DEBUG_WAYPOINTS_MAP false
#define DEBUG_RESIZED_PATH_MAP true
#define DEBUG_OBSTACLES_MAP false
#define DEBUG_PARTICLE_MAP false
#define DEBUG_RUN_MAP true

#define BLOWN_IMAGE_DEBUG_MAP "blownDebugMap.png"
#define ASTAR_DEBUG_IMAGE_NAME "aStarDebugMap.png"
#define WAYPOINTS_DEBUG_IMAGE_NAME "wayPointsDebugMap.png"
#define RESIZED_PATH_DEBUG_IMAGE_NAME "resizedPathDebugMap.png"
#define PARTICLES_OBSTACLES_DEBUG_IMAGE_NAME "particlesObstaclesDebugMap.png"
#define PARTICLES_DEBUG_IMAGE_NAME "particlesDebugMap.png"
#define RUN_DEBUG_MAP "runDebugMap.png"

#define RUN_DEBUG_INTERVAL 50

#endif // CONSTANTS_H_

