#include "geodb.h"
#include "geotools.h"


GeoDatabase::GeoDatabase() {
}

GeoDatabase::~GeoDatabase() {
}

bool GeoDatabase::load(const std::string& map_data_file) {
	std::cerr << "START LOADING" << std::endl;
	std::ifstream infile(map_data_file);
	if (!infile) {
		std::cerr << "Error: Cannot open " << map_data_file << "!" << std::endl;
		return false;
	}

	std::string line;
	while (getline(infile, line)) {
		//GET ADDRESS NAME
		std::string addressName;
		if (!processAddressName(addressName, line)) return false;

		//GET COORDINATES (2 PAIRS)
		GeoPoint g1, g2;
		if (!processCoordinates(infile, g1, g2)) return false;

		GeoPoint m1 = midpoint(g1, g2);
		std::string m1Coord = m1.to_string();
		std::string g1Coord = g1.to_string();
		std::string g2Coord = g2.to_string();

		std::string g1tog2 = g1Coord + g2Coord;
		std::string g2tog1 = g2Coord + g1Coord;


		m_mapPathToName[g1tog2] = addressName;
		m_mapPathToName[g2tog1] = addressName;

		makeConnection(g1Coord, g2);
		makeConnection(g2Coord, g1);

		//GET POI #
		int amtOfPOIs;
		if (!processPoiAmt(infile, amtOfPOIs)) return false;
		if (amtOfPOIs > 0) {

			std::string g1tom1 = g1Coord + m1Coord;
			std::string m1tog1 = m1Coord + g1Coord;
			std::string g2tom1 = g2Coord + m1Coord;
			std::string m1tog2 = m1Coord + g2Coord;
			m_mapPathToName[g1tom1] = addressName;
			m_mapPathToName[m1tog1] = addressName;
			m_mapPathToName[g2tom1] = addressName;
			m_mapPathToName[m1tog2] = addressName;


			makeConnection(g1Coord, m1);
			makeConnection(m1Coord, g1);
			makeConnection(g2Coord, m1);
			makeConnection(m1Coord, g2);
		}

		std::vector<GeoPoint> vectorPOI;
		if (!processPoi(infile, amtOfPOIs, vectorPOI)) return false;

		for (const auto p1 : vectorPOI){
			std::string p1Coord = p1.to_string();

			std::string p1tom1 = p1Coord + m1Coord;
			std::string m1top1 = m1Coord + p1Coord;

			m_mapPathToName[p1tom1] = "a path";
			m_mapPathToName[m1top1] = "a path";

			makeConnection(p1Coord, m1);
			makeConnection(m1Coord, p1);
		}
	}
	std::cerr << "FINISHED PROCESSING!" << std::endl;
	return true;
}


bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const {
	if (m_mapPOI.find(poi) == nullptr)	return false;
	point = *m_mapPOI.find(poi);
	return true;
}

std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
	std::string pointCoordinates = pt.to_string();
	std::vector<GeoPoint> geoPointConnectionsVector = *(m_mapGeoPointConnections.find(pointCoordinates));

	return geoPointConnectionsVector;
}

std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
	std::string coordinatesForward = pt1.to_string() + pt2.to_string();
	std::string coordinatesBackward = pt2.to_string() + pt1.to_string();

	if (m_mapPathToName.find(coordinatesForward) != nullptr) return *m_mapPathToName.find(coordinatesForward);
	if (m_mapPathToName.find(coordinatesBackward) != nullptr) return *m_mapPathToName.find(coordinatesBackward);
	return "";
}
