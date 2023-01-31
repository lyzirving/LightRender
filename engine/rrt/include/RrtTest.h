#ifndef RRT_TEST_H
#define RRT_TEST_H

class RrtTest
{
public:
	static void main();

private:
	static void gradientImg(int width, int height, int channel, uint8_t *data);
	static void draw(int width, int height, int channel, uint8_t* data);

	RrtTest() {};
	~RrtTest() = default;
};

#endif
