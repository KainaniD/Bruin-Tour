#include "base_classes.h"

class TourGenerator : public TourGeneratorBase
{
public:
	TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
	virtual ~TourGenerator();
	virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;

	const GeoDatabaseBase& m_geoDatabase;
	const RouterBase& m_router;
};