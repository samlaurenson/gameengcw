#include "scene.h"

extern std::shared_ptr<Entity> player;
extern std::vector<std::shared_ptr<Entity>> enemies;
extern std::shared_ptr<Entity> boss;

extern std::shared_ptr<Scene> activeScene;
extern std::shared_ptr<Scene> dungeonScene;
extern std::shared_ptr<Scene> bossScene;
extern std::shared_ptr<Scene> menuScene;

extern sf::Clock timer;

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

class BossScene : public Scene {
private:
	sf::Text text;

public:
	BossScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
	void restart();
};

class MenuScene : public Scene {
private:
	sf::Font font;
	sf::Text title;
	sf::Text playText;
public:
	MenuScene() = default;
	void update(double dt) override;
	void render(sf::RenderWindow& window) override;
	void load() override;
};