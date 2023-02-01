#ifndef RRT_TEST_H
#define RRT_TEST_H

class RrtCamera;

class RrtTest
{
public:
	static void main();

private:
	static void draw(const RrtCamera &camera, int width, int height, int channel, uint8_t* data);

	RrtTest() {};
	~RrtTest() = default;
};

#endif
