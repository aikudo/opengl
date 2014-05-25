// $Id: shape.cpp,v 1.1 2014-05-25 13:00:52-07 - - $

#include <typeinfo>
using namespace std;

#include "shape.h"
#include "util.h"

#include <cmath>

unordered_map<void*,string> text::fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

unordered_map<string,void*> text::fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape(): bordercolor("red"), margin({5,5}){
   DEBUGF ('c', this);
}

void shape::makeborder(const vertex& lcorner,
      const vertex& ucorner) const{
   //glPushAttrib(GL_LINE_WIDTH);
   GLfloat savedlw{};
   glGetFloatv(GL_LINE_WIDTH, &savedlw);

   glLineWidth (4);
   glBegin (GL_LINE_LOOP);
  // glBegin (GL_POLYGON);
   glEnable (GL_LINE_SMOOTH);
   //glLineWidth(borderwidth); //FIXME
   glColor3ubv (bordercolor.ubvec3());
   glVertex2f (lcorner.xpos, lcorner.ypos);
   glVertex2f (lcorner.xpos, ucorner.ypos);
   glVertex2f (ucorner.xpos, ucorner.ypos);
   glVertex2f (ucorner.xpos, lcorner.ypos);
   glEnd();
   glLineWidth (savedlw);

   //glPopAttrib();
}

text::text (const string& fontstr, const string& textdata):
      glut_bitmap_font(fontcode[fontstr]), textdata(textdata) {
   DEBUGF ('c', this);
}

void text::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   int str_width = glutBitmapLength 
      (glut_bitmap_font, (GLubyte*) textdata.c_str());
   int str_height = glutBitmapHeight (glut_bitmap_font);
   vertex cen ( {center.xpos - str_width  / 2 ,
                 center.ypos - str_height / 2 }) ;
   glColor3ubv (color.ubvec3());
   glRasterPos2f (cen.xpos, cen.ypos); //center of bitmap
   glutBitmapString (glut_bitmap_font, 
         (unsigned char *) textdata.c_str());

   if(borderon){
      vertex lcorner, ucorner;
      //lcorner.xpos = cen.xpos - str_width/2  + str_width/2 ;
      //lcorner.ypos = cen.ypos - str_height/2 + str_height/4;
      //ucorner.xpos = cen.xpos + str_width/2  + str_width/2 ;
      //ucorner.ypos = cen.ypos + str_height/2 + str_height/4;
      lcorner.xpos = cen.xpos - margin.xpos;
      lcorner.ypos = cen.ypos - str_height/4 - margin.ypos;
      ucorner.xpos = cen.xpos + str_width + margin.xpos;
      ucorner.ypos = cen.ypos + str_height*3/4 + margin.ypos;
      makeborder(lcorner, ucorner);
   }
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

void ellipse::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");

   const float delta = 2 * M_PI / 32;
   float width = dimension.xpos/2;
   float height = dimension.ypos/2;

   glBegin (GL_LINE_LOOP);//GL_POLYGON, GL_LINE_LOOP,
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec3());
   for (float theta = 0; theta < 2 * M_PI; theta += delta) {
      float xpos = width * cos (theta) + center.xpos ;
      float ypos = height * sin (theta) + center.ypos;
      glVertex2f (xpos, ypos);
   }
   glEnd();

   if(borderon){
      vertex lcorner, ucorner;
      lcorner.xpos = center.xpos - width  - margin.xpos ;
      lcorner.ypos = center.ypos - height - margin.ypos ;
      ucorner.xpos = center.xpos + width  + margin.xpos ;
      ucorner.ypos = center.ypos + height + margin.ypos ;
      makeborder(lcorner, ucorner);
   }
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}

polygon::polygon (const vertex_list& vertices): vertices(vertices) {
   DEBUGF ('c', this);
}

void polygon::draw (const vertex& center, const rgbcolor& color) const {
   DEBUGF ('d', this << "(" << center << "," << color << ")");
   size_t vcnt = 0;
   GLfloat xavg = 0, yavg=0;
   for(const auto& v:vertices){
      xavg += v.xpos;
      yavg += v.ypos;
      ++vcnt;
   }
   xavg /= vcnt;
   yavg /= vcnt;
   vertex_list poly;
   for(const auto& v:vertices){
      poly.push_back({v.xpos - xavg + center.xpos,
                      v.ypos - yavg + center.ypos});
   }

   GLfloat xmin = 1e10f;
   GLfloat xmax = 1e-10f;
   GLfloat ymin = 1e10f;
   GLfloat ymax = 1e-10f;

   glBegin (GL_LINE_LOOP);//GL_POLYGON, GL_LINE_LOOP,
   glEnable (GL_LINE_SMOOTH);
   glColor3ubv (color.ubvec3());
   for(const auto& v:poly){
      glVertex2f (v.xpos, v.ypos);
      xmin = (v.xpos < xmin) ? v.xpos: xmin;
      xmax = (v.xpos > xmax) ? v.xpos: xmax;
      ymin = (v.ypos < ymin) ? v.ypos: ymin;
      ymax = (v.ypos > ymax) ? v.ypos: ymax;
   }
   glEnd();

   if(borderon){
      vertex lcorner, ucorner;
      lcorner.xpos = xmin - margin.xpos  ;
      lcorner.ypos = ymin - margin.ypos  ;
      ucorner.xpos = xmax + margin.xpos  ;
      ucorner.ypos = ymax + margin.ypos  ;
      makeborder(lcorner, ucorner);
   }

}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

rectangle::rectangle (GLfloat w, GLfloat h):
      polygon({ {0,0}, {0,h},  {w,h}, {w,0} }) {
   DEBUGF ('c', this << "(" << w << "," << h << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat w, GLfloat h ):
   polygon({ {0,0}, {w/2,h/2},  {w,0}, {w/2,-h/2} }) {
   DEBUGF ('c', this);
}

triangle::triangle (const vertex_list& vertices): polygon(vertices) {
   DEBUGF ('c', this);
}

right_triangle::right_triangle (GLfloat w, GLfloat h ):
   triangle({ {0,0}, {w,h}, {w,0} }) {
   DEBUGF ('c', this);
}

isosceles::isosceles(GLfloat w, GLfloat h ):
   triangle({ {0,0}, {w/2,h}, {w,0} }) {
   DEBUGF ('c', this);
}

//sqrt(3) = 1.73205080756887729352
equilateral::equilateral(GLfloat w ):
   triangle({ {0,0}, {w/2,1.73205081f*w/2}, {w,0} }) {
   DEBUGF ('c', this);
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}


ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

