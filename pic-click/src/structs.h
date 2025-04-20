/*
 * Copyright (C) 2015-2018,2022 Parallel Realities. All rights reserved.
 */

typedef struct
{
	void (*logic)(void);
	void (*draw)(void);
} Delegate;

typedef struct
{
	int x;
	int y;
	int button[MAX_MOUSE_BUTTONS];
	int wheel;
} Mouse;

typedef struct
{
	SDL_Renderer *renderer;
	SDL_Window   *window;
	Delegate	 delegate;
	Mouse		 mouse;
} App;
