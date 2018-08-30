/*
 *   Source file of the dialog to change the brightness translation Matrix 
 *
 */

#include <allegro.h>
#include <alleggl.h>
#include <GL/glu.h>
#include <iostream>
#include "DLG_adjust_matrix.h"
#include "Trans_3D.h"

//Diagramm-Box
#define DGM_BOX_X         100
#define DGM_BOX_Y         3
#define DGM_BOX_SIZE      360
#define DGM_BOX_X_SCAL    256
#define DGM_BOX_Y_SCAL    256
#define NODE_SIZE   2
#define X_DIST_PXL (DGM_BOX_SIZE / (MAX_NODES-1))

//Dialog-Indizes
#define IDX_COL_SLDR    9
#define IDX_TEXT_FLD    19
#define IDX_RADIO_BT    31

//Enumeratorem für Buttons
#define BTN_REFR 0
#define BTN_APPL 1
#define BTN_LOAD 2
#define BTN_SAVE 3
#define BTN_HELP 4
#define BTN_EXIT 5

//verschiedene Positionsangaben für den Dialog
#define VALUES_Y 480
#define SLIDERS_Y 500
#define BACK_EDIT_X 600
#define BACK_EDIT_Y 400
#define BUTTONS_X 600
#define BUTTONS_W 160
#define BUTTONS_H  20
#define COL_RADIO_X 8
#define SLIDERS_H 258
#define VERLAUF_W 40


bool update_verlauf_1=1;
bool update_verlauf_2=1;
bool update_verlauf_3=1;
bool update_diagramm=1;
bool update_test_img=1;
bool update_background=1;

unsigned char bg_red, bg_green, bg_blue;
char bg_red_str[8], bg_green_str[8], bg_blue_str[8], filename_str[32]="default.ct";
int sliders[MAX_NODES] = {0,1,2,3,4,5,6,7,8};
volatile int close_button_pressed = FALSE;

ENM_LINE Line = COLOR_RED;
ENM_SIDE Side = SIDE_LEFT;
ENM_SIDE Side_prev = Side;
ENM_LINE Line_prev = Line;

fixed curviness;
BITMAP *test_img_left, *test_img_right, *test_img, *viewport, *verlauf_1;
ColorConversion *Col_Con;
DIALOG *dlg_graph;



char *help_str="StereoGL - color table editor\n----------------------------------

StGL_ct_edit.exe is part of the StereoGL engine by Martin Huedepohl.
It's written by Martin Huedepohl and Hannes Pertersen in 2005-2006.
This program creates StereoGL color tables (.ct files). Every monitor needs another color table for
perfect obliteration. A perfect obliteration means that your right eye doesn't see the picture for
the left eye and vice versa.

For more detailed help check out readme.txt.

";

int update_bgnd(void *dp3, int val);
int update_col(void *dp3, int val);
int init_proc( int msg, DIALOG* d, int c);
int diagramm_box_callback(BITMAP* viewport, int msg, int c);
int bgnd_slider(int msg, DIALOG *d, int c);
int col_slider(int msg, DIALOG *d, int c);
int my_radio_proc(int msg, DIALOG *d, int c);
int my_button_proc(int msg, DIALOG *d, int c);
int my_text_proc(int msg, DIALOG *d, int c);
int my_text_bgnd_proc(int msg, DIALOG *d, int c);


void close_button_handler(void)
{
  close_button_pressed = TRUE;
}END_OF_FUNCTION(close_button_handler);

int zeichne_verlauf1_proc(int msg, DIALOG *d, int c)
{
   int    Return = 0;
   if(update_verlauf_1)
   {
      float Scale = 256 / (float)d->w;
      int i;
      GLubyte    Lum1[MAX_POINTS], Lum2[MAX_POINTS], Lum3[MAX_POINTS];
      Col_Con->GetLum( Side, COLOR_RED, Lum1);
      Col_Con->GetLum( Side, COLOR_GREEN, Lum2);
      Col_Con->GetLum( Side, COLOR_BLUE, Lum3);
      if(Side==SIDE_LEFT)
         for(i=0;i < d->w;i++)
         line( verlauf_1, i, 0, i, d->h,
               makecol24( Lum1[   (int)(i * Scale) ],
                          Lum2[ (int)(i * Scale) ],
                          Lum3[  (int)(i * Scale) ]));
      else
         for(i=0;i < d->w;i++)
         line( verlauf_1, i, 0, i, d->h,
               makecol24( bg_red,
                          bg_green,
                          Lum3[  (int)(i * Scale) ]));
   blit(verlauf_1, screen, 0, 0, d->x, d->y, d->w, d->h);
   update_verlauf_1=0;
           blit(viewport, screen, 0, 0, DGM_BOX_X, DGM_BOX_Y, DGM_BOX_SIZE, DGM_BOX_SIZE);

   }
   return D_O_K;
}

int zeichne_verlauf2_proc(int msg, DIALOG *d, int c)
{
   int i;
   if(update_verlauf_2)
   {
      switch(Line)
      {
         case COLOR_RED:
            for(i=0;i<256;i++)line( screen, d->x, d->y+i, d->x+d->w,d->y+i, makecol24(255-i, 0, 0));
         break;
         case COLOR_GREEN:
            for(i=0;i<256;i++)line( screen, d->x, d->y+i, d->x+d->w,d->y+i, makecol24(0, 255-i, 0));
         break;
         case COLOR_BLUE:
            for(i=0;i<256;i++)line( screen, d->x, d->y+i, d->x+d->w,d->y+i, makecol24(0, 0, 255-i));
         break;
      }
   update_verlauf_2=0;
   }
   return D_O_K;
}

int zeichne_verlauf3_proc(int msg, DIALOG *d, int c)
{
   int    Return = 0;
   if(update_verlauf_3)
   {
      float Scale = 256 / (float)d->h;
      int i;
         for(i=0;i < d->h;i++)
         line( screen, d->x , d->y+i, d->x + d->w, d->y + i,
               makecol24(255-i * Scale, 255-i * Scale, 255-i * Scale));
   update_verlauf_3=0;
   }
   return D_O_K;
}


int zeichne_bg_proc(int msg, DIALOG *d, int c)
{
   if(update_background)
   {
      rectfill(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(bg_red, bg_green, bg_blue));
      update_background=0;
   }
   return D_O_K;
}

int draw_diagramm_proc(int msg, DIALOG* d, int c)
{
	static int focus = TRUE;
	static float prevx = 0, prevy = 0;	/* position of mouse */
	static float zoom = 30; 		/* field of view in degrees */
	int ret = 0, i,j,k,Color;
	NODE        N1;


	if (update_diagramm)
   {
        clear_bitmap(viewport);
        int points[8];
        for(i=0;i<MAX_LINES;i++)
        {
              switch(i)
              {
                 case 0:
                    Color = makecol(255,0,0);
                    break;
                 case 1:
                    Color = makecol(0,255,0);
                    break;
                 case 2:
                    Color = makecol(0,0,255);
                    break;
                 default:
                    Color = makecol(0,0,0);
              }

           // Draw splined Line
           for(j=0;j < DGM_BOX_X_SCAL-1;j++)
              line( viewport,
                    (int)(((float)DGM_BOX_SIZE / (float)DGM_BOX_X_SCAL) * j),
                    (int)(DGM_BOX_SIZE - (Col_Con->GetValue( Side, (ENM_LINE)i, j) * ((float)DGM_BOX_SIZE / (float)Col_Con->GetMaxLum( Side, (ENM_LINE)i)))),
                    (int)(((float)DGM_BOX_SIZE / (float)DGM_BOX_X_SCAL) * j+1),
                    (int)(DGM_BOX_SIZE - (Col_Con->GetValue( Side, (ENM_LINE)i, j+1) * ((float)DGM_BOX_SIZE / (float)Col_Con->GetMaxLum( Side, (ENM_LINE)i)))),
                    Color);

           for(j=1;j<MAX_NODES+1;j++)
           {
              Col_Con->GetNode( Side, (ENM_LINE)i, j, &N1);
              circlefill( viewport,
                          (int)(((((float)DGM_BOX_SIZE / (float)DGM_BOX_X_SCAL) * N1.xval) - NODE_SIZE)),
                          (int)(DGM_BOX_SIZE - (N1.yval * ((float)DGM_BOX_SIZE / (float)Col_Con->GetMaxLum( Side, (ENM_LINE)i)))),
                          NODE_SIZE,
                          Color);
           }
        }
        blit(viewport, screen, 0, 0, DGM_BOX_X, DGM_BOX_Y, DGM_BOX_SIZE, DGM_BOX_SIZE);
        update_diagramm=1;
   		ret = D_O_K;
	}
	return ret;
}

int rectangle_proc(int msg, DIALOG *d, int c)
{
   rect(screen, d->x, d->y, d->x+d->w, d->y+d->h, 0);
}
int close_button_proc(int msg, DIALOG *d, int c)
{
   if(close_button_pressed)return D_EXIT;
   return D_USER;
}

int test_image_proc(int msg, DIALOG *d, int c)
{
   if(update_test_img)
   {
      rectfill(screen, d->x, d->y, d->x+d->w, d->y+d->h, makecol(0, 0, 0));
      int x, y, c;
      unsigned char lum_val_left, lum_val_right, index;
      bmp_select(test_img);
      for (y=0; y<(test_img->h); y++)
      {
	      for (x=0; x<test_img->w; x++)
         {
            lum_val_left  = getb24(_getpixel24(test_img_left, x, y));
            lum_val_right = getb24(_getpixel24(test_img_right, x, y));
           _putpixel24(test_img, x, y, makecol24(Col_Con->GetValue( SIDE_LEFT, COLOR_RED, lum_val_left),
                                                 Col_Con->GetValue( SIDE_LEFT, COLOR_GREEN, lum_val_left),
                                                 Col_Con->GetValue( SIDE_RIGHT, COLOR_BLUE, lum_val_right)));
        }
      }


      set_trans_blender(0, 0, 255, 0);
      set_color_blender(0, 0, 255, 0);
      blit(test_img, screen, 0, 0, d->x, d->y+(d->h-test_img->h)/2, test_img->w, test_img->h );


      update_test_img=0;
   }
   return D_O_K;
}

DIALOG Dlg_Graph[] =
{
   /* (dialog proc)          (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp)     (dp2)          (dp3) */
   { init_proc,   0,   0,   0,  0,  0,    0,    0,    0,       0,  0,    NULL,    NULL,  NULL  },
   { rectangle_proc,  1,    1,    558,   766,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { rectangle_proc,  3,    470,    554,   295,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { rectangle_proc,  561,    364,    238,   104,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { rectangle_proc,  561,    470,    238,   104,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },


   { col_slider,             DGM_BOX_X-8,                 SLIDERS_Y,  16,   SLIDERS_H,  1,    0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[0]  },
   { col_slider,             DGM_BOX_X+X_DIST_PXL-8,      SLIDERS_Y,  16,   SLIDERS_H,  2,    0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[1]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*2)-8,  SLIDERS_Y,  16,   SLIDERS_H,  4,    0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[2]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*3)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[3]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*4)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[4]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*5)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[5]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*6)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[6]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*7)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[7]  },
   { col_slider,             DGM_BOX_X+(X_DIST_PXL*8)-8,  SLIDERS_Y,  16,   SLIDERS_H,  255,  0,    0,    0,       160,  0,    NULL,    (void*)update_col,      &sliders[8]  },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },
   { my_text_proc,           DGM_BOX_X-8,                 VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[0]  },
   { my_text_proc,           DGM_BOX_X+X_DIST_PXL-8,      VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[1]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*2)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[2]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*3)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[3]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*4)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[4]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*5)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[5]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*6)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[6]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*7)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[7]  },
   { my_text_proc,           DGM_BOX_X+(X_DIST_PXL*8)-8,  VALUES_Y,  0,    0,    255,  0,    0,    0,       0,    0,    NULL,     NULL,          &sliders[8]  },

   { rectangle_proc,  1,    1,    0,  0,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },
   {d_text_proc, 0, 0, 0, 0,   0,   0,  0,         0, 0, 0, (void*)"", NULL, NULL },


// BE CAREFUL: ACCESS TO RADIO- BUTTONS OVER INDEX OF Dlg_Graph[]
   { my_radio_proc,		     COL_RADIO_X,  SLIDERS_Y-8,	66,	20,	0,	0,	0,	D_SELECTED,		      1,	0,	(void*)"RED",	  NULL,	NULL },
   { my_radio_proc,	        COL_RADIO_X,  SLIDERS_Y+12,	66,	20,	0,	0,	0,	0,	  1,	0,	(void*)"GREEN",  NULL,	NULL },
   { my_radio_proc,		     COL_RADIO_X,  SLIDERS_Y+32,	66,	20,	0,	0,	0,	0,	          1,	0,	(void*)"BLUE",	  NULL,	NULL },
   { my_radio_proc,		     COL_RADIO_X,  23,	90,	20,	0,	0,	0,	0,	  2,	0,	(void*)"RIGHT EYE",  NULL,	NULL },
   { my_radio_proc,		     COL_RADIO_X, 3,	90,	20,	0,	0,	0,	D_SELECTED,	          2,	0,	(void*)"LEFT EYE",   NULL,	NULL },

   { my_button_proc,         BUTTONS_X,  340,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,            BTN_REFR,    0,  (void*)"REFRESH!", NULL, NULL },
   { my_button_proc,         BUTTONS_X,  444,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,            BTN_APPL,    0,  (void*)"APPLY!", NULL, NULL },
   { my_button_proc,         BUTTONS_X,  526,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,            BTN_LOAD,    0,  (void*)"LOAD", NULL, NULL },
   { my_button_proc,         BUTTONS_X,  550,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,            BTN_SAVE,    0,  (void*)"SAVE", NULL, NULL },
   { my_button_proc,         BUTTONS_X,  722,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,            BTN_HELP,    0,  (void*)"HELP", NULL, NULL },
   { my_button_proc,         BUTTONS_X,  746,  BUTTONS_W,    BUTTONS_H,255, 0,  0,  D_EXIT,       BTN_EXIT,    0,  (void*)"CLOSE", NULL, NULL },


   {d_edit_proc,    BACK_EDIT_X+48,      BACK_EDIT_Y,   64,  10,   255, 0,  0,         0,       sizeof(bg_red_str)-1,   0,    bg_red_str,       NULL, NULL  },
   {d_edit_proc,    BACK_EDIT_X+48,      BACK_EDIT_Y+10,   64,  10,   255, 0,  0,         0,       sizeof(bg_green_str)-1,   0,    bg_green_str,       NULL, NULL  },
   {d_edit_proc,    BACK_EDIT_X+48,      BACK_EDIT_Y+20,   64,  10,   255, 0,  0,         0,       sizeof(bg_blue_str)-1,   0,    bg_blue_str,       NULL, NULL  },
   {d_text_proc,              568,             504,  0,  0,   0,   0,  0,         0, 0, 0, (void*)"............................", NULL, NULL },
   {d_edit_proc,    568,      500,   238,  10,   255, 0,  0,         0,       sizeof(filename_str)-1,   0,    filename_str,       NULL, NULL  },
   
   
   { zeichne_bg_proc,        3,    364,    554,    104,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { test_image_proc,        561,    22,    238,    312,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { zeichne_verlauf1_proc,  DGM_BOX_X,    397,    DGM_BOX_SIZE,    VERLAUF_W,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { zeichne_verlauf2_proc,  490,    SLIDERS_Y,    VERLAUF_W,    0,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { zeichne_verlauf3_proc,  490,    DGM_BOX_Y,    VERLAUF_W,    DGM_BOX_SIZE,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   { draw_diagramm_proc,  DGM_BOX_X,    DGM_BOX_Y,    40,    0,    0,    0,    0,    0,       0,    0,    NULL,    NULL,  NULL  },
   {d_ctext_proc,              561,               7,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"TEST IMAGE", NULL, NULL },
   {d_ctext_proc,              561,             371,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"BACKGROUND COLOR", NULL, NULL },
   {d_ctext_proc,              561,             477,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"FILE", NULL, NULL },
   {d_ctext_proc,              561,             620,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"StereoGL color table editor", NULL, NULL },
   {d_ctext_proc,              561,             640,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"version 1.0", NULL, NULL },
   {d_ctext_proc,              561,             660,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"(c) 2006", NULL, NULL },
   {d_ctext_proc,              561,             680,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"by Martin Huedepohl", NULL, NULL },
   {d_ctext_proc,              561,             690,  238,  16,   0,   0,  0,         0, 0, 0, (void*)"&& Hannes Petersen", NULL, NULL },
   {d_text_proc,       BACK_EDIT_X,             BACK_EDIT_Y,  0,  0,   0,   0,  0,         0, 0, 0, (void*)   "RED  :", NULL, NULL },
   {d_text_proc,       BACK_EDIT_X,             BACK_EDIT_Y+10,  0,  0,   0,   0,  0,         0, 0, 0, (void*)"GREEN:", NULL, NULL },
   {d_text_proc,       BACK_EDIT_X,             BACK_EDIT_Y+20,  0,  0,   0,   0,  0,         0, 0, 0, (void*)"BLUE :", NULL, NULL },
   {close_button_proc,                   0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL,    NULL,          NULL          },
   { NULL,                   0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL,    NULL,          NULL          }
};

// getter for the parent module
DIALOG *get_DLG_adjust_matrix(void)
{
    dlg_graph = Dlg_Graph;
    return(dlg_graph);
}


// calculates the distance between two nodes
fixed node_dist(NODE n1, NODE n2)
{
   #define SCALE  64

   fixed dx = itofix(n1.xval - n2.xval) / SCALE;
   fixed dy = itofix(n1.yval - n2.yval) / SCALE;

   return fixsqrt(fixmul(dx, dx) + fixmul(dy, dy)) * SCALE;
}


// constructs nodes to go at the ends of the list, for tangent calculations
NODE dummy_node(NODE node, NODE prev)
{
   NODE n;

   n.xval = node.xval - (prev.xval - node.xval) / 8;
   n.yval = node.yval - (prev.yval - node.yval) / 8;

   return n;
}


// Calculates a set of node tangents
void calc_tangents(ENM_LINE Ln)
{
   register int i;
   NODE N1,N2,N3;

   // Calculate dummy at start
   Col_Con->GetNode( Side, Ln, 1, &N1);
   Col_Con->GetNode( Side, Ln, 2, &N2);
   N3 = dummy_node( N1, N2); 
   Col_Con->SetNode( Side, Ln, &N3, 0);

   // Calculate dummy at end
   Col_Con->GetNode( Side, Ln, MAX_NODES, &N1);
   Col_Con->GetNode( Side, Ln, MAX_NODES-1, &N2);
   N3 = dummy_node( N1, N2); 
   Col_Con->SetNode( Side, Ln, &N3, MAX_NODES+1);

   for (i=1;i < (MAX_NODES+2); i++)
   {
      Col_Con->GetNode( Side, Ln, i-1, &N1);
      Col_Con->GetNode( Side, Ln, i, &N2);
      Col_Con->GetNode( Side, Ln, i+1, &N3);

      N2.tangent = fixatan2( itofix( N3.yval - N1.yval),
                             itofix( N3.xval - N1.xval));

      Col_Con->SetNode( Side, Ln, &N1, i-1);
      Col_Con->SetNode( Side, Ln, &N2, i);
      Col_Con->SetNode( Side, Ln, &N3, i+1);
   }
}


// calculates the control points for a spline segment
void get_control_points(NODE n1, NODE n2, int points[8])
{
   fixed dist = fixmul(node_dist(n1, n2), curviness);

   points[0] = n1.xval;
   points[1] = n1.yval;

   points[2] = n1.xval + fixtoi(fixmul(fixcos(n1.tangent), dist));
   points[3] = n1.yval + fixtoi(fixmul(fixsin(n1.tangent), dist));

   points[4] = n2.xval - fixtoi(fixmul(fixcos(n2.tangent), dist));
   points[5] = n2.yval - fixtoi(fixmul(fixsin(n2.tangent), dist));

   points[6] = n2.xval;
   points[7] = n2.yval;
}



/* draws a spline curve connecting two nodes */
void draw_spline(NODE n1, NODE n2)
{
   int points[8];
   int i;

   get_control_points(n1, n2, points);
   spline(screen, points, makecol(255,0,0));

}


// Do spline calculation for one line
void calc_line(ENM_LINE Ln)
{
   register int i;
   int          Return = 0;
   int          P_Num, Points[8], Dummy[MAX_POINTS], Buf[MAX_POINTS];
   GLubyte      Lum[MAX_POINTS];
   NODE         N1,N2;
   
   
   // Amount of points on the Spline Curve between two nodes
   P_Num = (int)(DGM_BOX_X_SCAL / ((float)MAX_NODES-1));
   
   for (i=1; i < MAX_NODES; i++) 
   {
      Col_Con->GetNode( Side, Ln, i, &N1);
      Col_Con->GetNode( Side, Ln, i+1, &N2);

      get_control_points( N1, 
                          N2, 
                          Points);
      
      calc_spline( Points, 
                   P_Num, 
                   &Dummy[0], 
                   &Buf[(i-1)*P_Num]);      
   }

   // Check limits and convert to GLubyte
   for(i=0;i<MAX_POINTS;i++)
   {
      if(Buf[i] < 0)
         Lum[i] = 0;
      else if(Buf[i] > 255)
         Lum[i] = 255;
      else
         Lum[i] = Buf[i];         
   }
   Col_Con->SetLum( Side, Ln, Lum);
}

// Executed before the standard gl_viewport Procedure
int init_proc( int msg, DIALOG* d, int c)
{
   register int i,j,k;
   NODE         N1;
   GLubyte      Lum1[MAX_POINTS], Lum2[MAX_POINTS], Lum3[MAX_POINTS];
   if (msg == MSG_START)
   {
         LOCK_FUNCTION(close_button_handler);
      set_close_button_callback(close_button_handler);
      Col_Con = new ColorConversion;
      // Set colours of dialog components
	   Col_Con->GetBackgnd( &bg_red, &bg_green, &bg_blue );

      sprintf(bg_red_str, "%d", bg_red);
      sprintf(bg_green_str, "%d", bg_green);
      sprintf(bg_blue_str, "%d", bg_blue);

      // For the Spline
      curviness = ftofix(0.25);

	   viewport=create_bitmap(DGM_BOX_SIZE, DGM_BOX_SIZE);
      test_img_left  = load_bitmap("test_img_left.bmp",  NULL);
      test_img_right = load_bitmap("test_img_right.bmp",  NULL);
      test_img       = create_bitmap(test_img_left->w, test_img_left->h);
      verlauf_1 = create_bitmap(DGM_BOX_SIZE, VERLAUF_W);

      // Init scale for the Color Sliders
      for( i = IDX_COL_SLDR; i < (IDX_COL_SLDR + MAX_NODES);i++)
         Dlg_Graph[i].d1 = Col_Con->GetMaxLum( Side, Line);

      // Get memory for the Color Slider Textfields
      for( i = IDX_TEXT_FLD; i < (IDX_TEXT_FLD + MAX_NODES);i++)
         Dlg_Graph[i].dp = malloc(5);

      // Calculate positions of the Nodes
      for(i=0;i < 2;i++)
      {  // Side
         Side = (ENM_SIDE)i;
         for(j=0;j < MAX_LINES;j++)
         {  // Line
            for(k=1;k < MAX_NODES+1;k++)
            {
               Col_Con->GetNode( (ENM_SIDE)i, (ENM_LINE)j, k, &N1);
               N1.xval = (int)(((float)DGM_BOX_X_SCAL / (float)(MAX_NODES-1)) * (k-1));
               Col_Con->SetNode( (ENM_SIDE)i, (ENM_LINE)j, &N1, k);
            }

            // Initial calculations
            calc_tangents((ENM_LINE)j);
            calc_line((ENM_LINE)j);
            Col_Con->SaveConf(filename_str);

         }
      }
   }
   return D_USER;
}


// handles a new color slider position
int update_col(void *dp3, int val)
{
   int   *type    = (int*)dp3;
   int    Ret = 0;
   NODE          N1;

   Col_Con->GetNode( Side, Line, *type+1, &N1);
   if (N1.yval != val) 
   {   
      N1.yval = val;

      // Calculate the new Y value and the tangents
      calc_tangents(Line);
      calc_line(Line);

      Col_Con->SetNode( Side, Line, &N1, *type+1);
      update_verlauf_1=1;
   }
   return Ret;
}


// gui object procedure for the slider
int my_slider_proc(int msg, DIALOG *d, int c, BOOL Draw)
{
   if(Draw == TRUE)
      msg &= MSG_DRAW;

   return d_slider_proc(msg, d, c);
}


// prepares for the gui object procedure for the color sliders
int col_slider(int msg, DIALOG *d, int c)
{
   BOOL   Draw = FALSE;
   NODE   N1;

   // Identificate Slider
   int   *slider  = (int *)d->dp3;

   Col_Con->GetNode( Side, Line, *slider+1, &N1);
   switch (msg) 
   {
      case MSG_START:
	     // initialise the slider position
         d->d2 = N1.yval;
         Draw = TRUE;

	     break;

      case MSG_IDLE:
	     // has the slider position changed?
         if (d->d2 != N1.yval) 
         {

            d->d2 = N1.yval;
            Draw = TRUE;
//            update_diagramm=1;
         }
         break;
   }
   
   return my_slider_proc(msg, d, c, Draw);
}

/* This function is an ordinary GUI proc (no GL calls)
   It manages the 3 radio buttons and determine which rendering type
   has been selected */
int my_radio_proc(int msg, DIALOG *d, int c)
{
	int ret, i;
	bool redraw=0;
	ret = d_radio_proc(msg, d, c);
	

	/* Determine which one is selected... */
	for (i = IDX_RADIO_BT; i < (IDX_RADIO_BT+3); i++) {
		if (Dlg_Graph[i].flags & D_SELECTED) {
			break;
		}
	}
			
	/* ...and change the mode accordingly */
	switch(i) {
		case IDX_RADIO_BT:
			Line = COLOR_RED; 
            break;
		case IDX_RADIO_BT+1:
            Line = COLOR_GREEN;			
            break;
		case IDX_RADIO_BT+2:
		default:
			Line = COLOR_BLUE;
	}

	/* Determine which one is selected... */
	for (i = IDX_RADIO_BT+3; i < IDX_RADIO_BT+5; i++) {
		if (Dlg_Graph[i].flags & D_SELECTED) {
			break;
		}
	}
			
	/* ...and change the mode accordingly */
	switch(i) {
		case IDX_RADIO_BT+3:
			Side = SIDE_RIGHT;
			Dlg_Graph[IDX_RADIO_BT+2].flags=D_SELECTED;
			Dlg_Graph[IDX_RADIO_BT].flags=0;
			Dlg_Graph[IDX_RADIO_BT+1].flags=0;
            break;
		case IDX_RADIO_BT+4:
		default:
			Side = SIDE_LEFT;
	}
	
    // Update Window
	if(Side_prev != Side){update_background=1; update_verlauf_1=1;  redraw=1;}
   if( Line_prev != Line)update_verlauf_2=1;
   Line_prev=Line;
   Side_prev=Side;
   if(redraw)return D_REDRAW;
	return ret;
}


int my_text_proc(int msg, DIALOG *d, int c)
{
   register int i;
   int          Return;
   NODE         N1;
   // Identificate Textfield
   int   *slider  = (int *)d->dp3;

   Col_Con->GetNode( Side, Line, *slider+1, &N1);
   switch (msg) 
   {
      case MSG_START:
	     // initialise the text
         sprintf((char*)d->dp,"%d",N1.yval);
         strncat((char*)d->dp,"   ",3-strlen((char*)d->dp));
         msg &= MSG_DRAW;
	     break;

      case MSG_IDLE:
	     // has the slider position changed?
         if (atoi((const char*)d->dp) != N1.yval || atoi((const char*)d->dp) == 0) 
         {
            sprintf((char*)d->dp,"%d",N1.yval);
            strncat((char*)d->dp,"   ",3-strlen((char*)d->dp));
            msg &= MSG_DRAW;
         }
         break;
   }

   // call the parent object
   Return = d_text_proc(msg, d, c);
   return Return;
}


int my_button_proc(int msg, DIALOG *d, int c)
{
   int ret;
         
   // call the parent object
   ret = d_button_proc(msg, d, c);
   if (ret == D_CLOSE){

   switch(msg)
   {
      case MSG_DRAW:
         switch(d->d1)
         {
            case BTN_LOAD:
               break;
            
            case BTN_SAVE:
               break;

            case BTN_HELP:
               ;
               break;

            case BTN_EXIT:
               ;
               break;
         }
         break;

      case MSG_CLICK:
         switch(d->d1)
         {
            case BTN_REFR:
               update_test_img=1;
               break;
            case BTN_APPL:
               bg_red   = atoi(bg_red_str);
               bg_green = atoi(bg_green_str);
               bg_blue=atoi(bg_blue_str);
               Col_Con->SetBackgnd(bg_red, bg_green, bg_blue);
               update_background=1;
               update_verlauf_1=1;

               break;
            case BTN_LOAD:
               Col_Con->LoadConf(filename_str);
          	   Col_Con->GetBackgnd( &bg_red, &bg_green, &bg_blue );
          	   sprintf(bg_red_str, "%d", bg_red);
          	   sprintf(bg_green_str, "%d", bg_green);
          	   sprintf(bg_blue_str, "%d", bg_blue);

               update_background=1;
               update_verlauf_1=1;
               ret = D_REDRAW;
               break;
            
            case BTN_SAVE:
               if(!strcmp(filename_str, "default.ct")) allegro_message("default.ct is not proposed for saving.\nPlease choose another filename!");
               else if(!Col_Con->SaveConf(filename_str))
                 allegro_message("saved %s", filename_str);
               break;
            case BTN_HELP:
               allegro_message(help_str);
               break;
             case BTN_EXIT:
               return D_EXIT;
               break;

         }
         break;
   }
   return D_REDRAWME;
}
   // otherwise just return
   return ret;
}
