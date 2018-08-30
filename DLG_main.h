#ifndef DLG_main
#define DLG_main
/*
 *   Header file of the main dialog for the Editor 
 *
 */


/* function prototypes */
DIALOG *get_DLG_main(void);                         // getter for the parent module
int button_adj_col2(int msg, DIALOG *d, int c);     // helper for changing one of the color values
int button_adj_col(int msg, DIALOG *d, int c);      // gui object procedure for the color selection sliders

#endif
