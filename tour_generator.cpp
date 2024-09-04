#include "tour_generator.h"
#include "geotools.h"
#include <iostream>
#include <sstream>

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : m_geoDatabase(geodb), m_router(router) {}
TourGenerator::~TourGenerator() {}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
	std::vector<TourCommand> result;
	std::string currentPOI;
	std::string currentTalkingPoint;
	TourCommand commentaryCommand;
	if (!stops.get_poi_data(0, currentPOI, currentTalkingPoint)) {
		return result;
	}
	commentaryCommand.init_commentary(currentPOI, currentTalkingPoint);
	result.push_back(commentaryCommand);

	for (int i = 1; i < stops.size(); i++) {
		if (stops.get_poi_data(i, currentPOI, currentTalkingPoint)) {
			GeoPoint prevPosition, currentPosition; 

			//Last POI
			m_geoDatabase.get_poi_location(result[result.size() - 1].get_poi(), prevPosition);
			//Current POI
			commentaryCommand.init_commentary(currentPOI, currentTalkingPoint);
			m_geoDatabase.get_poi_location(currentPOI, currentPosition);

			std::vector<GeoPoint> routeFromLastPOI = m_router.route(prevPosition, currentPosition);
			for (size_t k = 0; k < routeFromLastPOI.size() - 1; k++) {
				TourCommand proceedCommand;
				GeoPoint start = routeFromLastPOI[k];
				GeoPoint end = routeFromLastPOI[k + 1];
				std::string streetName = m_geoDatabase.get_street_name(start, end);

				double angleOfLine = angle_of_line(start, end);
				std::string moveDir;
				if (angleOfLine >= 0 && angleOfLine < 22.5) {
					moveDir = "east";
				} else if (angleOfLine >= 22.5 && angleOfLine < 67.5) {
					moveDir = "northeast";
				} else if (angleOfLine >= 67.5 && angleOfLine < 112.5) {
					moveDir = "north";
				} else if (angleOfLine >= 112.5 && angleOfLine < 157.5) {
					moveDir = "northwest";
				} else if (angleOfLine >= 157.5 && angleOfLine < 202.5) {
					moveDir = "west";
				} else if (angleOfLine >= 202.5 && angleOfLine < 247.5) {
					moveDir = "southwest";
				} else if (angleOfLine >= 247.5 && angleOfLine < 292.5) {
					moveDir = "south";
				} else if (angleOfLine >= 292.5 && angleOfLine < 337.5) {
					moveDir = "southeast";
				} else {
					moveDir = "east";
				}
				
				proceedCommand.init_proceed(moveDir, streetName, distance_earth_miles(start, end), start, end);
				result.push_back(proceedCommand);

				if (k < routeFromLastPOI.size() - 2) {
					GeoPoint turnTo = routeFromLastPOI[k + 2];
					std::string turnToStreetName = m_geoDatabase.get_street_name(end, turnTo);

					double angleOfTurn = angle_of_turn(start, end, turnTo);
					if (angleOfTurn >= 1 && angleOfTurn < 180) {
						TourCommand turnCommand;
						turnCommand.init_turn("left", turnToStreetName);
						result.push_back(turnCommand);
					}
					else if (angleOfTurn >= 180 && angleOfTurn <= 359) {
						TourCommand turnCommand;
						turnCommand.init_turn("right", turnToStreetName);
						result.push_back(turnCommand);
					}
				}
			}

			result.push_back(commentaryCommand);
		}
	}
	return result;
}