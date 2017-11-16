using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace The_Quest
{
    //direction enum
    public enum Direction
    {
        Up,
        Down,
        Left,
        Right
    }

    public abstract class Mover
    {
        private const int MoveInterval = 10;
        
        protected Point location;
        public Point Location { get { return location; } }

        protected Game game;

        public Mover(Game gme, Point loc)
        {
            game = gme;
            location = loc;
        }

        //checks to see if a point is near this object's current location
        public bool Nearby(Point locationToCheck, int distance)
        {
            if ((Math.Abs(location.X - locationToCheck.X) < distance) &&
                (Math.Abs(location.Y - locationToCheck.Y) < distance))
                return true;

            else return false;
        }

        //moving this object...unless the end location is out of bounds, then we stay where we are
        public Point Move(Direction dir, Rectangle boundaries)
        {
            Point newLoc = location;

            switch (dir)
            {
                case Direction.Up:
                    if (newLoc.Y - MoveInterval >= boundaries.Top)
                        newLoc.Y -= MoveInterval;
                    break;

                case Direction.Down:
                    if (newLoc.Y + MoveInterval <= boundaries.Bottom)
                        newLoc.Y += MoveInterval;
                    break;

                case Direction.Left:
                    if (newLoc.X - MoveInterval >= boundaries.Left)
                        newLoc.X -= MoveInterval;
                    break;

                case Direction.Right:
                    if (newLoc.X + MoveInterval <= boundaries.Right)
                        newLoc.X += MoveInterval;
                    break;

                default: break;
            }

            return newLoc;
        }

    }
}
