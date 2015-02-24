#include "ros/ros.h"
#include "sensor_msgs/PointCloud.h"
#include "std_msgs/String.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include "parsingCSV.h"
#include <math.h>

std::vector<std::vector<sensor_msgs::PointCloud> > tile_pointclouds(104);
std::vector<sensor_msgs::PointCloud> tile_pointclouds2(104);
sensor_msgs::PointCloud pointcloud;
int tile_id;
float weight_slope;

std::vector<std::pair<float,float> > tileToFloorVector;
std::vector<std::pair<float,float> > sensorToTileVector;
std::vector<std::pair<int,float> > sensorOffsets;

void init_pointclouds(){
	tile_id = -1;
	for (int i=0;i<104;i++){
		std::stringstream sstm_tile;
		sstm_tile << "base_tile_" << i+1;
		std::string result_tile = sstm_tile.str();
		std::vector<sensor_msgs::PointCloud> tile_pointcloud(4);
		tile_pointclouds[i] = tile_pointcloud; 
		for (int j=0;j<4;j++){
			std::stringstream sstm_sensor;
			sstm_sensor << "base_sensor_" << i+1 << "_" << j+1;
			std::string result_sensor = sstm_sensor.str();
			tile_pointclouds[i][j].header.stamp = ros::Time::now();
			tile_pointclouds[i][j].header.frame_id = result_sensor;	
			tile_pointclouds[i][j].points.resize(1);
			tile_pointclouds[i][j].channels.resize(0);
			tile_pointclouds[i][j].points[0].x = 0;
			tile_pointclouds[i][j].points[0].y = 0;
			tile_pointclouds[i][j].points[0].z = 0;
		}
	}
}

void init_pointclouds(std::string filepath_sensors){
	sensorToTileVector = getSensorsPose(filepath_sensors);
	tile_id = -1;
	for (int i=0;i<104;i++){
		std::stringstream sstm_tile;
		sstm_tile << "base_tile_" << i+1;
		std::string result_tile = sstm_tile.str();
		tile_pointclouds2[i].header.stamp = ros::Time::now();
		tile_pointclouds2[i].header.frame_id = result_tile;	
		tile_pointclouds2[i].points.resize(4);
		tile_pointclouds2[i].channels.resize(0);
		for (int j=0;j<4;j++){
			float x = sensorToTileVector[i*4+j].first;
			float y = sensorToTileVector[i*4+j].second;
			tile_pointclouds2[i].points[j].x = x;
			tile_pointclouds2[i].points[j].y = y;
			tile_pointclouds2[i].points[j].z = 0;
		}
	}
}


void init_pointclouds(std::string filepath_tiles, std::string filepath_sensors){
	tileToFloorVector = getTilesPose(filepath_tiles);
	sensorToTileVector = getSensorsPose(filepath_sensors);
	
	//std::cout << "size tileFloor" << tileToFloorVector.size();
	//std::cout << "size sensorTile" << sensorToTileVector.size();
	tile_id = -1;
	std::string result_tile = "base_floor";
	pointcloud.header.stamp = ros::Time::now();
	pointcloud.header.frame_id = result_tile;	
	pointcloud.points.resize(104*4);
	pointcloud.channels.resize(1);	
	pointcloud.channels[0].name = "load";
	pointcloud.channels[0].values.resize(104*4);
	
	for (int i=0;i<104;i++){
		float x_tile = tileToFloorVector[i].first;
		float y_tile = tileToFloorVector[i].second;
		//std::cout << "i (x,y)" << i << " (" << x_tile << "," << y_tile << ")";
		
		for (int j=0;j<4;j++){
			int index = i*4+j;
			float x_sensor = sensorToTileVector[index].first;
			float y_sensor = sensorToTileVector[index].second;
			//std::cout << "j (x,y)" << j << " (" << x_sensor << "," << y_sensor << ")";
			pointcloud.points[index].x = x_tile+x_sensor;
			pointcloud.points[index].y = y_tile+y_sensor;
			pointcloud.points[index].z = 0;
			pointcloud.channels[0].values[index] = 0;
			//init offset
			sensorOffsets.push_back(std::make_pair(0,0));
		}
	}
}

void callback(const std_msgs::String::ConstPtr& msg)
{
	//	ROS_INFO("I heard: [%s]", msg->data.c_str());
	std::istringstream iss(msg->data.c_str());
	std::vector<std::string> tokens;
	copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),back_inserter(tokens));	

	std::string tile_ip = tokens[1];
	std::stringstream iss_ip(tile_ip.c_str());
	int a, b, c, d;
	char ch;
	iss_ip >> a >> ch >> b >> ch >> c >> ch >> d;

	tile_id = d-1;//start at 1
	std::vector<int> gauges;
	gauges.push_back(atoi(tokens[4].c_str()));
	gauges.push_back(atoi(tokens[5].c_str()));
	gauges.push_back(atoi(tokens[6].c_str()));
	gauges.push_back(atoi(tokens[7].c_str()));

	//offset update	
	//std::cout << tile_id << " " << gauges[0] << " " << gauges[1] << " " << gauges[2] << " " << gauges[3] << std::endl;
	/*
	   for (int i=0;i<4;i++){
	   tile_pointclouds[tile_id][i].header.stamp = ros::Time::now();//update time stamp
	   tile_pointclouds[tile_id][i].points[0].z = gauges[i]/1000.0;//update gauge value
	   }
	 */

	/*for (int i=0;i<4;i++){
	  tile_pointclouds2[tile_id].header.stamp = ros::Time::now();//update time stamp
	  tile_pointclouds2[tile_id].points[i].z = gauges[i]/3000.0;//update gauge value
	  }
	 */


	for (int i=0;i<4;i++){
		int index = tile_id*4+i;
		//sensor offset update
		if (sensorOffsets[index].first<50){
			sensorOffsets[index].first = sensorOffsets[index].first + 1;
			sensorOffsets[index].second = sensorOffsets[index].second + gauges[i];
		}
		else{
			if (sensorOffsets[index].first == 50){
				sensorOffsets[index].second = sensorOffsets[index].second/sensorOffsets[index].first;
				sensorOffsets[index].first = 51;
			}
		}

		//raw data update 		
		if (sensorOffsets[index].second != 0)
		{
			float weight = (gauges[i]-sensorOffsets[index].second)/weight_slope;//update gauge value
			//std::cerr << "gauge" << gauges[i] << "soffsets" << sensorOffsets[index].second << "weight" << weight << std::endl;
			pointcloud.channels[0].values[index] = weight;//update gauge value
		}
	}
}

int main(int argc, char** argv){
	ros::init(argc, argv, "tile_pointcloud_publisher");

	//subscriber to rawdata
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/smarttiles/raw_gauges",104,callback);

	ros::NodeHandle n;
	ros::Publisher cloud_pub = n.advertise<sensor_msgs::PointCloud>("/smarttiles/point_clouds", 1);

	std::string fileTileToFloor = argv[1];
	std::string fileSensorToTile = argv[2];
	weight_slope = atof(argv[3]);
	init_pointclouds(fileTileToFloor,fileSensorToTile);
	ros::Rate r(50);
	while(n.ok()){
		if (tile_id != -1){
			/*
			   for (int i=0;i<104;i++){
			//for (int j=0;j<4;j++){
			//	cloud_pub.publish(tile_pointclouds[i][j]);//v1
			//}
			//cloud_pub.publish(tile_pointclouds2[i]);//v2
			}
			 */
			pointcloud.header.stamp = ros::Time::now();//update time stamp
			cloud_pub.publish(pointcloud);
		}
		ros::spinOnce();
		r.sleep();

	}
	return 0;
}
