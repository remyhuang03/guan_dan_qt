#include"card.h"
bool operator==(const Card& card1, const Card& card2)
{
	if (card1.suit_ == card2.suit_ &&
		card1.point_ == card2.point_)
		return true;
	return false;
}
