#! /bin/bash

echo "<launch>" >> tf_static_floor_tiles.launch

#numbering lines
sed = tileToFloor.csv | sed 'N;s/\n/\t/' >> l_tileToFloor.csv

#replace lines for launch file
cat l_tileToFloor.csv | sed -e 's/\([0-9]*\)[\t]\([0-9\.-]*\),\([0-9\.-]*\)/<node pkg="tf" type="static_transform_publisher" name="base_tile_\1" args="\2 \3 0 0 0 0 base_floor base_tile_\1 100" \/> /g' >> tf_static_floor_tiles.launch

echo "</launch>" >> tf_static_floor_tiles.launch

#move to launch file
mv tf_static_floor_tiles.launch ../launch/
rm l_tileToFloor.csv

#numbering lines
echo "<launch>" >> tf_static_tile_sensors.launch

#numbering lines
sed = sensorToTile.csv | sed 'N;s/\n/\t/' >> l_sensorToTile.csv

#replace lines for launch file
cat l_sensorToTile.csv | sed -e 's/\([0-9]*\)[\t]\([0-9\.-]*\),\([0-9\.-]*\), \([0-9\.-]*\),\([0-9\.-]*\), \([0-9\.-]*\),\([0-9\.-]*\), \([0-9\.-]*\),\([0-9\.-]*\)/<node pkg="tf" type="static_transform_publisher" name="base_sensor_\1_1" args="\2 \3 0 0 0 0 base_tile_\1 base_sensor_\1_1 100" \/>\n <node pkg="tf" type="static_transform_publisher" name="base_sensor_\1_2" args="\2 \3 0 0 0 0 base_tile_\1 base_sensor_\1_2 100" \/>\n <node pkg="tf" type="static_transform_publisher" name="base_sensor_\1_3" args="\2 \3 0 0 0 0 base_tile_\1 base_sensor_\1_3 100" \/>\n <node pkg="tf" type="static_transform_publisher" name="base_sensor_\1_4" args="\2 \3 0 0 0 0 base_tile_\1 base_sensor_\1_4 100" \/> /g' >> tf_static_tile_sensors.launch

echo "</launch>" >> tf_static_tile_sensors.launch

#move to launch file
mv tf_static_tile_sensors.launch ../launch/
rm l_sensorToTile.csv
