#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <vector>
#include <string>
#include "parsingCSV.h"

using namespace std;

int main(int argc, char** argv){
	std::string fileTileToFloor = argv[1];
	std::string fileSensorToTile = argv[2];
	std::vector<std::pair<float,float> > tileToFloorVector = getTilesPose(fileTileToFloor);
	//std::vector<std::pair<float,float> > sensorToTileVector = getSensorsPose(fileSensorToTile);

	ros::init(argc, argv, "tile_tf_publisher");
	ros::NodeHandle n;

	ros::Rate r(100);

	tf::TransformBroadcaster broadcaster;

	while (n.ok()){
		std::string result_i;//tile
		//tile i to floor transforms
		for (int i=0; i<tileToFloorVector.size();i++){
			std::stringstream sstm_i;
			sstm_i << "base_tile_" << i+1;
			result_i = sstm_i.str();
			broadcaster.sendTransform(
					tf::StampedTransform(
						tf::Transform(tf::Quaternion(0,0,0,1), tf::Vector3(tileToFloorVector[i].first,tileToFloorVector[i].second,0)),
						ros::Time::now(),"base_floor",result_i));

			//overhead
			/*
			std::string result_j;//sensor
			for (int j=0;j<4;j++){
				std::stringstream sstm_j;
				sstm_j << "base_sensor_" << i+1 << "_" << j+1;
				result_j = sstm_j.str();
				broadcaster.sendTransform(
						tf::StampedTransform(
							tf::Transform(tf::Quaternion(0,0,0,1), tf::Vector3(sensorToTileVector[i*4+j].first,sensorToTileVector[i*4+j].second,0)),
							ros::Time::now(),result_i, result_j));
		
			}
			*/
		}
		//sensor j to tile i transforms
		r.sleep();


	}
}
