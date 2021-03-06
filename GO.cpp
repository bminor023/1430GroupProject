/* Author: Brendon Kofink
 *         Johann Rajadurai
 *         Aaron Sierra
 *         David Day
 *         Lucy Ray
 * Assignment Title: Ball Game
 * Assignment Description: user can launch balls to hit objects.
 * Due Date: 12/08/2021
 * Date Created: 11/02/2021
 * Date Last Modified: 12/04/2021
 */

#include "GO.h"

GO::GO(vec2 center, Shape shape)
{
    this->center = center;
    this->shape = shape;
    physics = DEFAULT_PHYSICS;
    moveMethod = EASE;
}
void GO::Init() {
    shape.setCenter(&center);
}
void GO::setDeltaTime(double deltaTime)
{
    this->deltaTime = deltaTime;
}
void GO::setCenter(vec2 center)
{
    this->center = center;
}
void GO::setPhysics(Physics physics)
{
    this->physics = physics;
}
void GO::setMoveMethod(MoveMethod moveMethod)
{
    this->moveMethod = moveMethod;
}
void GO::setMoving(bool moving)
{
    this->moving = moving;
}
void GO::setVisible(bool visible)
{
    this->visible = visible;
}
void GO::SetDest(vec2 dest)
{
    destination = dest;
    setMoving(true);
}

vec2 GO::getCenter()
{
    return center;
}
Physics GO::getPhysics()
{
    return physics;
}
MoveMethod GO::getMoveMethod()
{
    return moveMethod;
}
bool GO::isMoving()
{
    return moving;
}
bool GO::isColliding()
{
    return colliding;
}
bool GO::isVisible()
{
    return visible;
}

void GO::Translate(double x, double y)
{
    center.x += x;
    center.y += y;
}
void GO::Translate(vec2 delta)
{
    center += delta;
}

void GO::ApplyForce(vec2 force)
{
  physics.velocity += force;
}

void GO::update(SDL_Plotter &g)
{
  // check if the object is moving
  // if it is moving, cover up the old "frame" and draw new one
  // if it has reached its destination, isMoving = false
  // still draw things that aren't moving
  colliding = false;

  if (isMoving() || moveMethod == PHYSICS)
  {
    if(visible) {
      erase(g);
    }
    // Linear movement
    if (moveMethod == LINEAR)
    {
      vec2 linear = destination - getCenter();
      linear = linear.normalized();
      center += (linear*deltaTime*1.2);
    }
    // Ease movement
    else if (moveMethod == EASE)
    {
      setCenter(center.lerp(destination, .01 * deltaTime));
      // setCenter(center.lerp(destination, .01));
    }
    // Physics movement
    else if (moveMethod == PHYSICS)
    {
      physics.velocity += DOWN * deltaTime * 0.01;
      center += physics.velocity * deltaTime;
      // physics.velocity += DOWN *  0.03;
      // center += physics.velocity;
    }
    if ((center - destination).sqrMagnitude() < 0.5)
    {
      setMoving(false);
    }
  }
  if((angle!=shape.getAngle() || scaleChange!=0) && visible)
  {
    if(!moving) {
      erase(g);
    }
    shape.setAngle(angle);
    shape.setRadius(shape.getRadius()+scaleChange*deltaTime);
  }
  scaleChange = 0;
  if (visible)
  {
    shape.draw(g,center);
  }
  Init();
}

void GO::erase(SDL_Plotter &g)
{
  Color color = shape.getColor();
  shape.setColor(BLANK);
  shape.draw(g,center);
  shape.setColor(color);
}

// Use their shapes to check collision and apply force if they are.
// The GameController will call this function for you if it thinks the two are colliding.
void GO::CheckCollision(GO& other)
{
    if (physics.canCollide && other.physics.canCollide)
    {
        if (shape.isColliding(other.shape))
        {
            Collide(other);
            other.Collide(*this);
        }
    }
}

//Called if is colliding.
void GO::Collide(GO& other)
{
    if(moveMethod == PHYSICS) {
        vec2 normal = other.shape.getCollisionAxis(shape).normalized();
        vec2 relativeVelocity = physics.velocity - other.physics.velocity;
        double normalVelocity = normal.dot(relativeVelocity);
        if (normalVelocity > 0)
        {
            normal = normal * -1;
            normalVelocity = normal.dot(relativeVelocity);
        }
        double e = (physics.bounce + other.physics.bounce) / 2;
        double j = -(1 + e) * normalVelocity;
        double impulse = j / (physics.mass + other.physics.mass);
        vec2 impulsePerMass = normal * impulse;
        physics.velocity += impulsePerMass * physics.mass;
    }
    colliding = true;
}

void GO::Rotate(double a)
{
    angle += a * deltaTime;
}

void GO::Scale(double factor)
{
    scaleChange += factor;
}

void GO::SetColor(Color color)
{
    shape.setColor(color);
}

Shape GO::getShape()
{
    return shape;
}
