#include <SFML/Graphics.hpp>
#include "../lib_ecm/ecm.h"

class ShapeComponent : public Component {
protected:
	std::shared_ptr<sf::Shape> _shape;
public:
	ShapeComponent() = delete;
	explicit ShapeComponent(Entity* p);

	void update(double dt) override;
	void render(sf::RenderWindow& window) override;

	sf::Shape& getShape() const;

	template<typename T, typename... Targs>
	void setShape(Targs... params) {
		_shape.reset(new T(params...));
	}
};