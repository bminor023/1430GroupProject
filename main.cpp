#include <iostream>
#include "point.h"
#include "color.h"
#include "force.h"
#include "ball.h"
#include "SDL_Plotter.h"

int main(int argc, char** argv)
{
  SDL_Plotter g(500, 500);
  force_t a, b(5.4,PI/2);
  Ball ball;
  ball.setForce(vec2(1, 0));

  cout << a.getMagnitude() << ' ' << a.getDirection() << endl;
  cout << b.getMagnitude() << ' ' << b.getDirection() << endl;
  a.apply(b);
  cout << a.getMagnitude() << ' ' << a.getDirection() << endl;


  while(!g.getQuit()) {
    if(g.kbhit()) {
      g.getKey();
    }
    g.update();
    ball.setColor(color_t(255,255,255));
    ball.draw(g);
    ball.applyForce(DOWN * 0.02);
    ball.move();
    ball.setColor(RED);
    ball.draw(g);
    if(ball.getCenter().y > g.getRow()-50) {
      ball.applyForce(UP);
    }
    if(ball.getCenter().x > g.getCol()-10) {
      ball.applyForce(LEFT);
    }
    else if (ball.getCenter().x < 10)
    {
      ball.applyForce(RIGHT);
    }
  }
  return 0;
}
