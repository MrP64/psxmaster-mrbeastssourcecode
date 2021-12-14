/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "week4.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	IO_Data arc_water, arc_water_ptr[1];
	IO_Data arc_watert, arc_watert_ptr[1];
	IO_Data arc_clouds, arc_clouds_ptr[2];

	Gfx_Tex tex_back0; //sunset
	Gfx_Tex tex_back1; //beach
	Gfx_Tex tex_back2; //gf trash
	Gfx_Tex tex_back5; //text
	Gfx_Tex tex_trash; //trash
	
	//water state
	Gfx_Tex tex_water;
	u8 water_frame, water_tex_id;

	Gfx_Tex tex_water2;
	u8 water2_frame, water2_tex_id;

	Gfx_Tex tex_water3;
	u8 water3_frame, water3_tex_id;

	//watert state
	Gfx_Tex tex_watert;
	u8 watert_frame, watert_tex_id;

	Gfx_Tex tex_watert2;
	u8 watert2_frame, watert2_tex_id;

	Gfx_Tex tex_watert3;
	u8 watert3_frame, watert3_tex_id;

    //cloud state
	Gfx_Tex tex_clouds;
	u8 clouds_frame, clouds_tex_id;

	Gfx_Tex tex_clouds2;
	u8 clouds2_frame, clouds2_tex_id;

	Gfx_Tex tex_clouds3;
	u8 clouds3_frame, clouds3_tex_id;

	Animatable water_animatable;
	Animatable water2_animatable;
	Animatable water3_animatable;
	
	Animatable watert_animatable;
	Animatable watert2_animatable;
	Animatable watert3_animatable;

	Animatable clouds_animatable;
	Animatable clouds2_animatable;
	Animatable clouds3_animatable;
} Back_Week4;

//water animation and rects
static const CharFrame water_frame[5] = {
	//water 1
	{0, {  0,   0,   256,  57}, { 256,  -37}}, //0
	{0, {  0,   0,   256,  57}, { 256,  -36}}, //1
	{0, {  0,   0,   256,  57}, { 256,  -35}}, //2
	{0, {  0,   0,   256,  57}, { 256,  -34}}, //3
	{0, {  0,   0,   256,  57}, { 256,  -33}}, //4
};

static const Animation water_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 1
};

//water animation and rects
static const CharFrame watert_frame[5] = {
	//water 1
	{0, {  0,   0,   256,  57}, { 256,  -37}}, //0
	{0, {  0,   0,   256,  57}, { 256,  -36}}, //1
	{0, {  0,   0,   256,  57}, { 256,  -35}}, //2
	{0, {  0,   0,   256,  57}, { 256,  -34}}, //3
	{0, {  0,   0,   256,  57}, { 256,  -33}}, //4
};

static const Animation watert_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 1
};


//clouds animation and rects
static const CharFrame clouds_frame[5] = {
	//clouds
	{0, {  0,   0,   256,  165}, { 256,  165}}, //0
	{0, {  0,   0,   256,  165}, { 256,  164}}, //1
	{0, {  0,   0,   256,  165}, { 256,  163}}, //2
	{0, {  0,   0,   256,  165}, { 256,  162}}, //3
	{0, {  0,   0,   256,  165}, { 256,  161}}, //4
};

static const Animation clouds_anim[2] = {
	{2, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //clouds 1
};

//clouds 2 animation and rects
static const CharFrame clouds2_frame[5] = {
	//clouds 2
	{1, {  0,   0,   145,  169}, { 145,  169}}, //0
	{1, {  0,   0,   145,  169}, { 145,  168}}, //1
	{1, {  0,   0,   145,  169}, { 145,  167}}, //2
	{1, {  0,   0,   145,  169}, { 145,  166}}, //3
	{1, {  0,   0,   145,  169}, { 145,  165}}, //4
};

static const Animation clouds2_anim[2] = {
	{2, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //clouds 2
};

//clouds 3 animation and rects
static const CharFrame clouds3_frame[5] = {
	//clouds 3
	{1, {  145, 0,   111,  53}, { 111,  51}}, //0
	{1, {  145, 0,   111,  53}, { 111,  50}}, //1
	{1, {  145, 0,   111,  53}, { 111,  49}}, //2
	{1, {  145, 0,   111,  53}, { 111,  48}}, //3
	{1, {  145, 0,   111,  53}, { 111,  47}}, //4
};

static const Animation clouds3_anim[2] = {
	{2, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1,  ASCR_BACK, 0}}, //clouds 3
};


//water animation and rects
static const CharFrame water2_frame[5] = {
	//water 2
	{0, {  0,   57,  256,  57}, { 256,  -37}}, //5 
	{0, {  0,   57,  256,  57}, { 256,  -36}}, //6 
	{0, {  0,   57,  256,  57}, { 256,  -35}}, //7 
	{0, {  0,   57,  256,  57}, { 256,  -34}}, //8 
	{0, {  0,   57,  256,  57}, { 256,  -33}}, //9
	
};
    static const Animation water2_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 2
};

static const CharFrame water3_frame[5] = {
	//water 3
	{0, {  0,   114, 128,  57}, { 256,  -37}}, //10
	{0, {  0,   114, 128,  57}, { 256,  -36}}, //11 
	{0, {  0,   114, 128,  57}, { 256,  -35}}, //12 
	{0, {  0,   114, 128,  57}, { 256,  -34}}, //13 
	{0, {  0,   114, 128,  57}, { 256,  -33}}, //14 
};
   static const Animation water3_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 3
};

//water animation and rects
static const CharFrame watert2_frame[5] = {
	//water 2
	{0, {  0,   57,  256,  57}, { 256,  -37}}, //5 
	{0, {  0,   57,  256,  57}, { 256,  -36}}, //6 
	{0, {  0,   57,  256,  57}, { 256,  -35}}, //7 
	{0, {  0,   57,  256,  57}, { 256,  -34}}, //8 
	{0, {  0,   57,  256,  57}, { 256,  -33}}, //9
	
};
    static const Animation watert2_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 2
};

static const CharFrame watert3_frame[5] = {
	//water 3
	{0, {  0,   114, 128,  57}, { 256,  -37}}, //10
	{0, {  0,   114, 128,  57}, { 256,  -36}}, //11 
	{0, {  0,   114, 128,  57}, { 256,  -35}}, //12 
	{0, {  0,   114, 128,  57}, { 256,  -34}}, //13 
	{0, {  0,   114, 128,  57}, { 256,  -33}}, //14 
};
   static const Animation watert3_anim[2] = {
	{3, (const u8[]){0, 1, 2, 3, 4, 3, 2, 1, ASCR_BACK, 0}}, //water 3
};


//water functions
void Week4_water_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->water_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &water_frame[this->water_frame = frame];
		if (cframe->tex != this->water_tex_id)
			Gfx_LoadTex(&this->tex_water, this->arc_water_ptr[this->water_tex_id = cframe->tex], 0);
	}
}
void Week4_water2_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->water2_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &water2_frame[this->water2_frame = frame];
		if (cframe->tex != this->water2_tex_id)
			Gfx_LoadTex(&this->tex_water2, this->arc_water_ptr[this->water2_tex_id = cframe->tex], 0);
	}
}
void Week4_water3_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->water3_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &water3_frame[this->water3_frame = frame];
		if (cframe->tex != this->water3_tex_id)
			Gfx_LoadTex(&this->tex_water3, this->arc_water_ptr[this->water3_tex_id = cframe->tex], 0);
	}
}

//watert functions
void Week4_watert_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->watert_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &watert_frame[this->watert_frame = frame];
		if (cframe->tex != this->watert_tex_id)
			Gfx_LoadTex(&this->tex_watert, this->arc_watert_ptr[this->watert_tex_id = cframe->tex], 0);
	}
}
void Week4_watert2_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->watert2_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &watert2_frame[this->watert2_frame = frame];
		if (cframe->tex != this->watert2_tex_id)
			Gfx_LoadTex(&this->tex_watert2, this->arc_watert_ptr[this->watert2_tex_id = cframe->tex], 0);
	}
}
void Week4_watert3_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->watert3_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &watert3_frame[this->watert3_frame = frame];
		if (cframe->tex != this->watert3_tex_id)
			Gfx_LoadTex(&this->tex_watert3, this->arc_watert_ptr[this->watert3_tex_id = cframe->tex], 0);
	}
}

void Week4_clouds_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->clouds_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &clouds_frame[this->clouds_frame = frame];
		if (cframe->tex != this->clouds_tex_id)
			Gfx_LoadTex(&this->tex_clouds, this->arc_clouds_ptr[this->clouds_tex_id = cframe->tex], 0);
	}
}
void Week4_clouds2_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->clouds2_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &clouds2_frame[this->clouds2_frame = frame];
		if (cframe->tex != this->clouds2_tex_id)
			Gfx_LoadTex(&this->tex_clouds2, this->arc_clouds_ptr[this->clouds2_tex_id = cframe->tex], 0);
	}
}
void Week4_clouds3_SetFrame(void *user, u8 frame)
{
	Back_Week4 *this = (Back_Week4*)user;
	
	//Check if this is a new frame
	if (frame != this->clouds3_frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &clouds3_frame[this->clouds3_frame = frame];
		if (cframe->tex != this->clouds3_tex_id)
			Gfx_LoadTex(&this->tex_clouds3, this->arc_clouds_ptr[this->clouds3_tex_id = cframe->tex], 0);
	}
}

void Week4_water_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &water_frame[this->water_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_water, &src, &dst, stage.camera.bzoom);
}
void Week4_water2_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &water2_frame[this->water2_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_water2, &src, &dst, stage.camera.bzoom);
}
void Week4_water3_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &water3_frame[this->water3_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_water3, &src, &dst, stage.camera.bzoom);
}
void Week4_watert_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &watert_frame[this->watert_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_watert, &src, &dst, stage.camera.bzoom);
}
void Week4_watert2_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &watert2_frame[this->watert2_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_watert2, &src, &dst, stage.camera.bzoom);
}
void Week4_watert3_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &watert3_frame[this->watert3_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_watert3, &src, &dst, stage.camera.bzoom);
}

void Week4_clouds_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &clouds_frame[this->clouds_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_clouds, &src, &dst, stage.camera.bzoom);
}
void Week4_clouds2_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &clouds2_frame[this->clouds2_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_clouds2, &src, &dst, stage.camera.bzoom);
}
void Week4_clouds3_Draw(Back_Week4 *this, fixed_t x, fixed_t y)
{
	//Draw character
	const CharFrame *cframe = &clouds3_frame[this->clouds3_frame];
	
	fixed_t ox = x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t oy = y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {ox, oy, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(&this->tex_clouds3, &src, &dst, stage.camera.bzoom);
}
//Week 4 background functions

void Back_Week4_DrawFG(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;

	//Draw text
	RECT text_src = {0, 0, 256, 256};
	RECT_FIXED text_dst = {
		FIXED_DEC(-130 - SCREEN_WIDEOADD2,1),
		FIXED_DEC(-120,1),
		FIXED_DEC(256 + SCREEN_WIDEOADD,1),
		FIXED_DEC(256,1)
	};

if (stage.stage_id == StageId_1_1 && stage.song_step <= 135)
{
	Stage_DrawTex(&this->tex_back5, &text_src, &text_dst, stage.camera.bzoom);
}
}

void Back_Week4_DrawMD(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;
	
	//Draw gf trash
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT gftrash_src = {0, 0, 256, 256};
	RECT_FIXED gftrash_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(640 + SCREEN_WIDEOADD,1),
		FIXED_DEC(360,1)
	};
	if (stage.song_step <= 1984) {
	Stage_DrawTex(&this->tex_back2, &gftrash_src, &gftrash_dst, stage.camera.bzoom);
	}
}

void Back_Week4_DrawBG(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	fixed_t fx, fy;


	//Draw trash
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT trash_src = {0, 0, 256, 256};
	RECT_FIXED trash_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(640 + SCREEN_WIDEOADD,1),
		FIXED_DEC(360,1)
	};

	if (stage.song_step <= 1984) {
	Stage_DrawTex(&this->tex_trash, &trash_src, &trash_dst, stage.camera.bzoom);
	}

	//Draw beach
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT beach_src = {0, 0, 256, 256};
	RECT_FIXED beach_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(640 + SCREEN_WIDEOADD,1),
		FIXED_DEC(360,1)
	};
	
	Stage_DrawTex(&this->tex_back1, &beach_src, &beach_dst, stage.camera.bzoom);

	//Animate and draw clouds
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->clouds_animatable, 0);
				Animatable_SetAnim(&this->clouds2_animatable, 0);
				Animatable_SetAnim(&this->clouds3_animatable, 0);

				break;
		}
	}

	Animatable_Animate(&this->clouds_animatable, (void*)this, Week4_clouds_SetFrame);
	Week4_clouds_Draw(this, FIXED_DEC(91,1) - fx, FIXED_DEC(30,1) - fy);

	Animatable_Animate(&this->clouds2_animatable, (void*)this, Week4_clouds2_SetFrame);
	Week4_clouds2_Draw(this, FIXED_DEC(234,1) - fx, FIXED_DEC(30,1) - fy);

	Animatable_Animate(&this->clouds3_animatable, (void*)this, Week4_clouds3_SetFrame);
	Week4_clouds3_Draw(this, FIXED_DEC(480,1) - fx, FIXED_DEC(50,1) - fy);
	
	//Animate and draw water
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->water_animatable, 0);
				Animatable_SetAnim(&this->water2_animatable, 0);
				Animatable_SetAnim(&this->water3_animatable, 0);

				break;
		}
	}

	if (stage.song_step >= 1984) {
	Animatable_Animate(&this->water_animatable, (void*)this, Week4_water_SetFrame);
	Week4_water_Draw(this, FIXED_DEC(91,1) - fx, FIXED_DEC(30,1) - fy);

	Animatable_Animate(&this->water2_animatable, (void*)this, Week4_water2_SetFrame);
	Week4_water2_Draw(this, FIXED_DEC(345,1) - fx, FIXED_DEC(27,1) - fy);

	Animatable_Animate(&this->water3_animatable, (void*)this, Week4_water3_SetFrame);
	Week4_water3_Draw(this, FIXED_DEC(600,1) - fx, FIXED_DEC(27,1) - fy);
	}

	//Animate and draw watert
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		switch (stage.song_step & 7)
		{
			case 0:
				Animatable_SetAnim(&this->watert_animatable, 0);
				Animatable_SetAnim(&this->watert2_animatable, 0);
				Animatable_SetAnim(&this->watert3_animatable, 0);

				break;
		}
	}

	Animatable_Animate(&this->watert_animatable, (void*)this, Week4_watert_SetFrame);
	Week4_watert_Draw(this, FIXED_DEC(91,1) - fx, FIXED_DEC(30,1) - fy);

	Animatable_Animate(&this->watert2_animatable, (void*)this, Week4_watert2_SetFrame);
	Week4_watert2_Draw(this, FIXED_DEC(345,1) - fx, FIXED_DEC(27,1) - fy);

	Animatable_Animate(&this->watert3_animatable, (void*)this, Week4_watert3_SetFrame);
	Week4_watert3_Draw(this, FIXED_DEC(600,1) - fx, FIXED_DEC(27,1) - fy);

	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT sunset_src = {0, 0, 256, 256};
	RECT_FIXED sunset_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(640 + SCREEN_WIDEOADD,1),
		FIXED_DEC(360,1)
	};
	
	Stage_DrawTex(&this->tex_back0, &sunset_src, &sunset_dst, stage.camera.bzoom);
}

void Back_Week4_Free(StageBack *back)
{
	Back_Week4 *this = (Back_Week4*)back;
	
	//Free water archive
	Mem_Free(this->arc_water);
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Week4_New(void)
{
	//Allocate background structure
	Back_Week4 *this = (Back_Week4*)Mem_Alloc(sizeof(Back_Week4));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_Week4_DrawFG;
	this->back.draw_md = Back_Week4_DrawMD;
	this->back.draw_bg = Back_Week4_DrawBG;
	this->back.free = Back_Week4_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\WEEK4\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "back0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "back1.tim"), 0);
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);
	Gfx_LoadTex(&this->tex_back5, Archive_Find(arc_back, "back5.tim"), 0);
	Gfx_LoadTex(&this->tex_trash, Archive_Find(arc_back, "trash.tim"), 0);
	Mem_Free(arc_back);
	
	//Load water textures
	this->arc_water = IO_Read("\\WEEK4\\WATER.ARC;1");
	this->arc_water_ptr[0] = Archive_Find(this->arc_water, "water.tim");

	//Load water textures
	this->arc_watert = IO_Read("\\WEEK4\\WATERT.ARC;1");
	this->arc_watert_ptr[0] = Archive_Find(this->arc_watert, "watert.tim");
	
	//Load clouds textures
	this->arc_clouds = IO_Read("\\WEEK4\\CLOUDS.ARC;1");
	this->arc_clouds_ptr[0] = Archive_Find(this->arc_clouds, "back3.tim");
	this->arc_clouds_ptr[1] = Archive_Find(this->arc_clouds, "back4.tim");
	
	//Initialize water state
	Animatable_Init(&this->water_animatable, water_anim);
	Animatable_Init(&this->water2_animatable, water2_anim);
	Animatable_Init(&this->water3_animatable, water3_anim);

	Animatable_SetAnim(&this->water_animatable, 0);
	Animatable_SetAnim(&this->water2_animatable, 0);
	Animatable_SetAnim(&this->water3_animatable, 0);

	Animatable_Init(&this->watert_animatable, watert_anim);
	Animatable_Init(&this->watert2_animatable, watert2_anim);
	Animatable_Init(&this->watert3_animatable, watert3_anim);

	Animatable_SetAnim(&this->watert_animatable, 0);
	Animatable_SetAnim(&this->watert2_animatable, 0);
	Animatable_SetAnim(&this->watert3_animatable, 0);

	Animatable_Init(&this->clouds_animatable, clouds_anim);
	Animatable_Init(&this->clouds2_animatable, clouds2_anim);
	Animatable_Init(&this->clouds3_animatable, clouds3_anim);

	Animatable_SetAnim(&this->clouds_animatable, 0);
	Animatable_SetAnim(&this->clouds2_animatable, 0);
	Animatable_SetAnim(&this->clouds3_animatable, 0);

	this->water_frame = this->water_tex_id = 0xFF; //Force art load
	this->water2_frame = this->water2_tex_id = 0xFF; //Force art load
	this->water3_frame = this->water3_tex_id = 0xFF; //Force art load

	this->watert_frame = this->watert_tex_id = 0xFF; //Force art load
	this->watert2_frame = this->watert2_tex_id = 0xFF; //Force art load
	this->watert3_frame = this->watert3_tex_id = 0xFF; //Force art load

	this->clouds_frame = this->clouds_tex_id = 0xFF; //Force art load
	this->clouds2_frame = this->clouds2_tex_id = 0xFF; //Force art load
	this->clouds3_frame = this->clouds3_tex_id = 0xFF; //Force art load
	
	return (StageBack*)this;
}
