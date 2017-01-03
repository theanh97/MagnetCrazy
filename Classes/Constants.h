#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__
#pragma once 
#include "cocos2d.h"

class Constants
{
	Constants();
	static Constants * s_constants;
public:
	static Constants * getInstance();
	~Constants();

	// -------------- ORDER --------------
	const int ORDER_MAGNET						  = 5; 
	const int ORDER_BOX							  = 0;
	const int ORDER_BACKGROUND					  = -1;
	const int ORDER_BACKGROUND_SKY				  = -3;
	const int ORDER_BACKGROUND_LAND				  = -2;
	const int ORDER_BACKGROUND_LOADING			  = 3; 
	const int ORDER_BACKGROUND_LOADING_BACKGROUND = 2;
	const int ORDER_BACKGROUND_LOADING_SN		  = 2;
	const int ORDER_TABLE_SCORE					  = 10;
	const int ORDER_TEXT_SCORE					  = 11;
	const int ORDER_GUIDE						  = 3;

	// -------------- COLLISION ---------
	const int COLLISION_MAGNET		= 1; 
	const int COLLISION_BOX			= 2;
	const int COLLISION_BARRIER		= 3;
	const int COLLISION_LAND		= 4;

	// --------------SPRITE TAG ------------
	const int TAG_TABEL_SCORE			 = 1; 
	const int TAG_BEST_SCORE			 = 2;
	const int TAG_CURRENT_SCORE			 = 3;
	const int TAG_BUTTON_REPLAY			 = 4;
	const int TAG_BUTTON_EXIT			 = 5;
	const int TAG_LOADING_BAR			 = 7;
	const int TAG_LOADING_BAR_BACKGROUND = 8;
	const int TAG_MAGNET				 = 9;
	const int TAG_ARROW_DOWN			 = 10;
	const int TAG_ARROW_UP				 = 11;
	const int TAG_S						 = 12;
	const int TAG_N						 = 13;
	const int TAG_LABEL_SCORE			 = 14;
	const int TAG_TOP_BARRIER			 = 15;
	const int TAG_BOTTOM_BARRIER		 = 16;
	const int TAG_GUIDE_TAP_LEFT		 = 17;
	const int TAG_GUIDE_TAP_RIGHT		 = 18;
	const int TAG_GUIDE_TAP_MOUSE		 = 19;
	const int TAG_GUIDE_ARROW_NEXT		 = 20;
	const int TAG_GUIDE_ARROW_DOWN		 = 21;
	const int TAG_GUIDE_ARROW_UP		 = 22;
	const int TAG_GUIDE_MAGNET			 = 23;
	const int TAG_GUIDE_2_VACH			 = 24;
	const int TAG_GUIDE_TAP_TO_PLAY		 = 25;
};

#endif // __CONSTANTS_H__
