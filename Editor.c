/*
 *    Program to edit the color tables for red-green stereoscopy.
 *
 *    This program shows the effects of changing
 *       the color conversion table.
 */

#include "allegro.h"

#include "DLG_adjust_matrix.h"

int main(void)
{
   allegro_init();
   install_keyboard();
   install_mouse();
   install_timer();

   // Set Color depth to 24 bit as norm for bitmaps
   set_color_depth(24);
   
   if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 768, 0, 0) != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Error setting graphics mode\n%s\n", allegro_error);
      return 1;
   }
   
   set_color_conversion(COLORCONV_TOTAL);

   // Clear background to grey
   clear_to_color(screen,makecol(212,208,200));

   // Call the Dialog Box
  	set_dialog_color (get_DLG_adjust_matrix(), makecol(0, 0, 0), makecol(212, 208, 200));
   do_dialog(get_DLG_adjust_matrix(), -1);

   return 0;
}

END_OF_MAIN();
