// $Id: graphics.h,v 1.1 2014-05-25 13:00:52-07 - - $

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <memory>
#include <vector>
#include <list>
using namespace std;

#include <GL/freeglut.h>

#include "rgbcolor.h"
#include "shape.h"

class object {
   private:
      //shared_ptr<shape> pshape;
      shape_ptr pshape;
      vertex center;
      rgbcolor color;
   public:
      // Default copiers, movers, dtor all OK.
      object(){};
      explicit object(
         const shape_ptr& pshape_, const vertex& center_,
         const rgbcolor& color_):
            pshape(pshape_), center(center_), color(color_){};
      void draw() { pshape->draw (center, color); }
      void setborder(const rgbcolor& color, const vertex& margin){
         pshape->setborder(color, margin);
      }
      void border(bool sw){ pshape->border(sw); }
      void border(){ pshape->border(not pshape->border()); }

      void move (GLfloat delta_x, GLfloat delta_y) {
         center.xpos += delta_x;
         center.ypos += delta_y;
      }
};

class mouse {
      friend class window;
   private:
      int xpos {0};
      int ypos {0};
      int entered {GLUT_LEFT};
      int left_state {GLUT_UP};
      int middle_state {GLUT_UP};
      int right_state {GLUT_UP};
   private:
      void set (int x, int y) { xpos = x; ypos = y; }
      void state (int button, int state);
      void draw();
};


class window {
      friend class mouse;
   private:
      static int width;         // in pixels
      static int height;        // in pixels
      //static list<object> objects;
      static vector<object> objects;
      static size_t selected_obj;
      static mouse mus;
      static int mres; //move resolution
      static int bordersize; //border pixel size 
   private:
      static void close();
      static void entry (int mouse_entered);
      static void display();
      static void border(size_t sel);
      static void reshape (int width, int height);
      static void keyboard (GLubyte key, int, int);
      static void special (int key, int, int);
      static void motion (int x, int y);
      static void passivemotion (int x, int y);
      static void mousefn (int button, int state, int x, int y);
   public:
      static void push_back (const object& obj)
         { objects.push_back (obj); }
      static void setwidth (int width_) { width = width_; }
      static void setheight (int height_) { height = height_; }
      static int getwidth () { return width; }
      static int getheight () { return height; }
      static int  bsize () { return bordersize; }
      static void bsize (int size ) { bordersize = size; }
      static int musres () { return mres; }
      static void musres (int res ) { mres = res; }
      static void main();
};

#endif

