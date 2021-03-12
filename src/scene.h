#include <SFML/Graphics.hpp>
#include "../lib_ecm/ecm.h"

class Scene {
public:
	Scene() = default;

	virtual ~Scene() = default;
	virtual void update(double dt);
	virtual void render(sf::RenderWindow& window);
	virtual void load() = 0;
	std::vector<std::shared_ptr<Entity>>& getEnts();

protected:
	EntityManager _ents;
};