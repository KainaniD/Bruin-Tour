#include "geodb.h"
#include "router.h"
#include "HashMap.h"
#include <queue>
#include <stack>
#include <algorithm>

Router::Router(const GeoDatabaseBase& geo_db) : m_geoDatabase(geo_db) {}

Router::~Router() {}

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
	HashMap<std::string> m_seenNodes;
	HashMap<GeoPoint> locationOfPreviousWayPoint;

	if (pt1.to_string() == pt2.to_string()) { 
		std::vector<GeoPoint> currentRoute;
		currentRoute.push_back(pt1);
		return currentRoute; 
	}

	std::queue<GeoPoint> currentRoute;
	currentRoute.push(pt1);
	m_seenNodes[pt1.to_string()] = "seen";

	while (!currentRoute.empty()) {
		GeoPoint curr = currentRoute.front();
		currentRoute.pop();
		
		if (curr.to_string() == pt2.to_string()) {
			std::vector<GeoPoint> finalRoute;
			while (locationOfPreviousWayPoint.find(curr.to_string())->to_string() != pt1.to_string()) {
				finalRoute.push_back(curr);
				curr = *locationOfPreviousWayPoint.find(curr.to_string());
			}
			finalRoute.push_back(curr);
			finalRoute.push_back(pt1);
			std::reverse(finalRoute.begin(), finalRoute.end());
			return finalRoute;
		}
		std::vector<GeoPoint> connectedPoints = m_geoDatabase.get_connected_points(curr);
		for (const auto i : connectedPoints) {
			if (m_seenNodes.find(i.to_string()) == nullptr) {
				m_seenNodes[i.to_string()] = "seen";
				locationOfPreviousWayPoint[i.to_string()] = curr;
				currentRoute.push(i);
			}
		}
	}

	std::cerr << "COULD NOT FIND ROUTE";
	std::vector<GeoPoint> empty;
	return empty;
}