//
// Created by Dave Riedel on 05-Sep-21.
//

#ifndef BITSANDDROIDSGUI_COORDINATES_H
#define BITSANDDROIDSGUI_COORDINATES_H

class coordinates {
 private:
  float x;
  float y;

 public:
  coordinates();
  coordinates(float x, float y);
  float getX() const;
  float getY() const;

  void setX(float x);
  void setY(float y);
};

#endif  // BITSANDDROIDSGUI_COORDINATES_H
