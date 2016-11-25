#include "head.h"

Door::Door(const Vector2 &Origin, char* Path)
{
	origin = Origin;
	size = Vector2(32, 64);
	path = Path;
}