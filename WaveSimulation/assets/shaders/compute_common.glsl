
vec2 cubepos = vec2(11, 15);
float cubescale = 8.0f;
bool insideCube(vec2 position)
{
	return (position.x > cubepos.x - cubescale &&
			position.x < cubepos.x + cubescale && 
			position.y > cubepos.y - cubescale && 
			position.y < cubepos.y + cubescale);
}
