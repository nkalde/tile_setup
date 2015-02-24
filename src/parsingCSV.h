#ifndef CSV_Row_H_
#define CSV_ROW_H_
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <map>

class CSVRow
{
	public:
		float const& operator[](std::size_t index) const
		{
			return m_data[index];
		}

		std::size_t size() const
		{
			return m_data.size();
		}
		void readNextRow(std::istream& str)
		{
			std::string line;
			std::getline(str,line);

			std::stringstream lineStream(line);
			std::string cell;

			m_data.clear();
			while(std::getline(lineStream,cell,','))
			{m_data.push_back(atof(cell.c_str()));}
		}

	private:
		std::vector<float> m_data;
};

std::istream& operator>>(std::istream& str,CSVRow& data)
{
	data.readNextRow(str);
	return str;
}   

//lower left corner
std::vector<std::pair<float,float> > getTilesPose(std::string file){
	//read tile poses data from file
	std::vector<std::pair<float,float> > tiles_pose;

	std::cout << file << std::endl;
	std::ifstream file_s(file.c_str());
	CSVRow row;

	while(file_s >> row)
	{
		//std::cout << "(" << row[0] << "," << row[1] << ")\n";
		tiles_pose.push_back(std::make_pair(row[0],row[1]));
	}
	return tiles_pose;
}

//sensors
//24
//13
std::vector<std::pair<float,float> > getSensorsPose(std::string file){
	//read sensor poses data from file
	std::vector<std::pair<float,float> > sensors_pose;

	std::cout << file << std::endl;
	std::ifstream file_s(file.c_str());
	CSVRow row;	
	while(file_s >> row)
	{
		
		//std::cout << "(" << row[0] << "," << row[1] << ")\n";
		sensors_pose.push_back(std::make_pair(row[0],row[1]));
		sensors_pose.push_back(std::make_pair(row[2],row[3]));
		sensors_pose.push_back(std::make_pair(row[4],row[5]));
		sensors_pose.push_back(std::make_pair(row[6],row[7]));
	}
	return sensors_pose;
}


//sensors
//24
//13
std::map<int,std::vector<float> > getSensorsOffset(std::string file){
	//read sensor poses data from file
	std::map<int,std::vector<float> > sensors_offset;

	std::cout << file << std::endl;
	std::ifstream file_s(file.c_str());
	CSVRow row;	
	while(file_s >> row)
	{
		//std::cout << "(" << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << ")\n";
		sensors_offset[(int)row[0]].push_back(row[1]);
		sensors_offset[(int)row[0]].push_back(row[2]);
		sensors_offset[(int)row[0]].push_back(row[3]);
		sensors_offset[(int)row[0]].push_back(row[4]);
	}
	return sensors_offset;
}

#endif
