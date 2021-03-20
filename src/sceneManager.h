#include "scene.h"

extern std::shared_ptr<Entity> player;
extern std::vector<std::shared_ptr<Entity>> enemies;

extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> dungeonScene;

class DungeonScene : public Scene {
private:
	sf::Text text;

public:
	DungeonScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
	void restart();
};