#pragma once
class IndexLength
{
private:
	int index;
	int length;
public :
	IndexLength(int index, int length) {
		this->index = index;
		this->length = length;
	}
	int getIndex() {
		return this->index;
	}
	int getLength() {
		return this->length;
	}
};

