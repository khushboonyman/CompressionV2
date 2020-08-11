#pragma once
class IndexLength
{
private:
	int index;
	int length;
	int cumulativeIndex;
public :
	IndexLength(int index, int length, int cumulativeIndex) {
		this->index = index;
		this->length = length;
		this->cumulativeIndex = cumulativeIndex;
	}
	int getIndex() {
		return this->index;
	}
	int getLength() {
		return this->length;
	}
	int getCumulativeLength() {
		return this->cumulativeIndex;
	}
};

