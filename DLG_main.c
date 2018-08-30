/*
 *   Source file of the main dialog for the Editor 
 *
 */

#include "allegro.h"

#include "DLG_main.h"
#include "DLG_adjust_matrix.h"

BITMAP *image_1[2];

DIALOG *the_dialog;

/* dialog initialisation matrix */
DIALOG The_Dialog[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)                    (d2)  (dp)              (dp2) (dp3) */
//  { d_button_proc,      840,  132,  161,  49,   255,  0,    't',  0,       0,                      0,   (void*)"&Toggle Me" ,      NULL, NULL  },
//   { d_list_proc,       360,  100,  207,  207,  255,  0,    0,    0,       0,                      0,    listbox_getter,   NULL, NULL  },
//   { change_font_proc,  80,   232,  161,  49,   255,  0,    'f',  D_EXIT,  0,                      0,    "Change &Font",   NULL, NULL  },
/*   { load_left_bitmap,  840,  232,  150,  32,   255,  0,    'b',  D_EXIT,  0,                      0,    "LOAD LEFT BITMAP",   NULL, NULL  },
   { load_right_bitmap, 840,  272,  150,  32,   255,  0,    'b',  D_EXIT,  0,                      0,    "LOAD RIGHT BITMAP",   NULL, NULL  },
   { stereo_view,       840,  340,  161,  49,   255,  0,    't',  D_EXIT,       0,                      0,   "Stereo View" ,     NULL, NULL  },
*/
   { button_adj_col2,    840,  272,  161,  49,   255,  0,    0,    D_EXIT,       0,                      0,    (void*)"test",   NULL, NULL  },
   { button_adj_col,    840,  340,  161,  49,   255,  0,    0,    D_EXIT,       0,                      0,    (void*)"Adjust Colour",   NULL, NULL  },
   { d_button_proc,     840,  400,  161,  49,   255,  0,    0,    D_EXIT,  0,                      0,    (void*)"EXIT",            NULL, NULL  },
   { NULL,              0,    0,    0,    0,    0,    0,    0,    0,       0,                      0,    NULL,              NULL, NULL  }
};

/* getter for the parent module */
DIALOG *get_DLG_main(void)
{
    the_dialog = The_Dialog;
    return(the_dialog);
}


int button_adj_col2(int msg, DIALOG *d, int c)
{
   int ret;
   
   //image[1]=load_bitmap("button_rechts.bmp",  NULL);
   //The_Dialog[1].dp2 = image[1];
   
   /* call the parent object */
   ret = d_button_proc(msg, d, c);

   /* trap the close return value and change the font */
   if( ret == D_CLOSE ) 
   {
         //set_palette(desktop_palette);
         clear_to_color(screen,makecol(40,40,40));
         return D_REDRAW; 
   }

   /* otherwise just return */
   return ret;
}

int button_adj_col(int msg, DIALOG *d, int c)
{
   int ret;

   /* call the parent object */
   ret = d_button_proc(msg, d, c);
/*
   RGB rgb;
      rgb.r = 80;
      rgb.g = 60;
      rgb.b = 60;

      vsync();
      set_color(0, &rgb);
*/

   /* trap the close return value and change the font */
   if( ret == D_CLOSE ) 
   {
      clear_to_color(screen,makecol(40,40,40));
      if(do_dialog(get_DLG_adjust_matrix(), -1))
      {
         //set_palette(desktop_palette);
         clear_to_color(screen,makecol(40,40,40));
         return D_REDRAW; 
      }
   }

   /* otherwise just return */
   return ret;
}

