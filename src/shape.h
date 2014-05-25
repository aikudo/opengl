// $Id: shape.h,v 1.1 2014-05-25 13:00:52-07 - - $

#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <iomanip>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>
using namespace std;

#include "rgbcolor.h"

//
// Shapes constitute a single-inheritance hierarchy, summarized
// here, with the superclass listed first, and subclasses indented
// under their immediate superclass.
//
// shape
//    text
//    ellipse
//       circle
//    polygon
//       rectangle
//          square
//       diamond
//       triangle
//          right_triangle
//          isosceles
//          equilateral
//

class shape;
typedef struct {GLfloat xpos; GLfloat ypos; } vertex;
typedef vector<vertex> vertex_list;
typedef shared_ptr<shape> shape_ptr; 

//
// Abstract base class for all shapes in this system.
//

class shape {
   friend ostream& operator<< (ostream& out, const shape&);
   private:
      shape (const shape&) = delete; // Prevent copying.
      shape& operator= (const shape&) = delete; // Prevent copying.

   protected:
      rgbcolor bordercolor;// ("red");
      vertex margin;
      bool borderon=false;
      GLfloat borderwidth=4.0f;
      inline shape();
      void makeborder(const vertex& lcorner,
            const vertex& ucorner) const;
   public:
      //void border
      virtual ~shape() {}
      //what is = zero mean?
      virtual void draw (const vertex&, const rgbcolor&) const = 0;
      virtual void show (ostream&) const;
      void setborder(const rgbcolor& color_, const vertex& margin_){
         bordercolor = color_;
         margin = margin_;
      };
      void border(bool sw) { borderon = sw; };
      bool border() { return borderon; };
      //void border(GLfloat width = 4, const rgbcolor& color );
};


//
// Class for printing text.
//

class text: public shape {
   protected:
      void* glut_bitmap_font = nullptr;
      static unordered_map<void*,string> fontname ;
      static unordered_map<string,void*> fontcode;
      string textdata;
   public:
      //text (void* glut_bitmap_font, const string& textdata);
      text (const string& glut_bitmap_font, const string& textdata);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

//
// Classes for ellipse and circle.
//

class ellipse: public shape {
   protected:
      vertex dimension;
   public:
      ellipse (GLfloat width, GLfloat height);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};

class circle: public ellipse {
   public:
//inherited for both draw() and show() from ellipse
      circle (GLfloat diameter);
};

//
// Class polygon.
//

class polygon: public shape {
   protected:
      const vertex_list vertices;
   public:
      polygon (const vertex_list& vertices);
      virtual void draw (const vertex&, const rgbcolor&) const override;
      virtual void show (ostream&) const override;
};


//
// Classes rectangle, square, etc.
//

class rectangle: public polygon {
   public:
      rectangle (GLfloat width, GLfloat height);
};

class square: public rectangle {
   public:
      square (GLfloat width);
};

class diamond: public polygon {
   public:
      diamond (const GLfloat width, const GLfloat height);
};

class triangle: public polygon {
   public:
      triangle (const vertex_list& vertices);
};

class right_triangle: public triangle {
   public:
      right_triangle (const GLfloat width, const GLfloat height);
};

class isosceles: public triangle {
   public:
      isosceles (const GLfloat width, const GLfloat height);
};

class equilateral: public triangle {
   public:
      equilateral (const GLfloat width);
};

ostream& operator<< (ostream& out, const shape&);

#endif

