#include "base_classes.h"
#include "HashMap.h"
#include <iostream>
#include <fstream>
#include <sstream>

class GeoDatabase : public GeoDatabaseBase
{
public:
	GeoDatabase();
	virtual ~GeoDatabase();
	virtual bool load(const std::string& map_data_file);
	virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
	virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
	virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
private:
	HashMap<GeoPoint> m_mapPOI;
	HashMap<std::string> m_mapPathToName;
	HashMap<std::vector<GeoPoint>> m_mapGeoPointConnections;

	bool processAddressName(std::string& address,  std::string line) {
		address = line;
		return true;
	};

	bool processCoordinates(std::ifstream& infile, GeoPoint &G1, GeoPoint &G2) {
		std::string line;
		if (!getline(infile, line)) {
			std::cerr << "ERROR IN FILE ENCODING";
			return false;
		}
		std::istringstream coordinateStream(line);
		std::string lat1, long1, lat2, long2;
		if (!(coordinateStream >> lat1 >> long1 >> lat2 >> long2)) {
			std::cerr << "ERROR IN FILE ENCODING";
			return false;
		}
		GeoPoint tempG1(lat1, long1);
		G1 = tempG1;
		GeoPoint tempG2(lat2, long2);
		G2 = tempG2;
		return true;
	};

	bool processPoiAmt(std::ifstream &infile, int &amt) {
		std::string line;
		if (!getline(infile, line)) {
			std::cerr << "ERROR IN FILE ENCODING";
			return false;
		}		std::istringstream amtOfPoiStream(line);
		if (!(amtOfPoiStream >> amt)) {
			std::cerr << "ERROR IN FILE ENCODING";
			return false;
		}
		return true;
	};

	bool processPoi(std::ifstream& infile, int amt, std::vector<GeoPoint> &vectorPOI) {
		std::string line;
		for (int i = 0; i < amt; i++) {
			if (!getline(infile, line, '|')) {
				std::cerr << "ERROR IN FILE ENCODING";
				return false;
			}			
			std::istringstream poiNameStream(line);
			std::string poiName = line;
			if (!getline(infile, line)) {
				std::cerr << "ERROR IN FILE ENCODING";
				return false;
			}
			std::istringstream poiCoordStream(line);
			std::string poiLat, poiLong;
			if (!(poiCoordStream >> poiLat >> poiLong)) {
				std::cerr << "ERROR IN FILE ENCODING";
				return false;
			}
			GeoPoint poiGeoPoint(poiLat, poiLong);
			m_mapPOI[poiName] = poiGeoPoint;
			vectorPOI.push_back(poiGeoPoint);
		}
		return true;

	};

	void makeConnection(std::string coordinate, GeoPoint gp) {
		if (m_mapGeoPointConnections.find(coordinate) == nullptr) {
			std::vector<GeoPoint> vector;
			m_mapGeoPointConnections[coordinate] = vector;
		}
		m_mapGeoPointConnections.find(coordinate)->push_back(gp);
	}
};