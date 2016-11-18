#include "head.h"

//Axis Aligned Bounding Box
//Frequently used to represent static platforms.
class AABB
{
protected:
	Vector2 origin;
	Vector2 size;
	SDL_Texture* texture = NULL;

public:
	AABB();
	AABB(const Vector2 &Origin, const Vector2 &Size);

	//Getters
	Vector2 getOrigin();
	Vector2 getSize();

	//Setters
	void setOrigin(Vector2 newOrigin);
	void setSize(Vector2 newSize);

	bool overlaps(AABB other);

	void draw(SDL_Renderer* r, bool debug, Camera camera);
};