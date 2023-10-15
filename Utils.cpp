#include "Utils.h"

int matModUtils::reverseInt(int x) {
	int val = (x << 16) | ((x >> 16) & 0xFFFF);
	return ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
}
