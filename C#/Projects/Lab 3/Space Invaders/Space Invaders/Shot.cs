using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Space_Invaders
{
    public class Shot
    {
        //adjust these constants to make the game easier or harder...
        //...smaller shots are easier to dodge, faster shots are harder to avoid
        private int moveInterval;
        private const int width = 5;
        private const int height = 15;

        private Point location;
        public Point Location
        {
            get
            {
                return location;
            }
        }

        private Direction direction;
        private Rectangle boundaries;

        public Shot(Point loc, Direction dir, Rectangle bound, int moveInt)
        {
            location = loc;
            direction = dir;
            boundaries = bound;
            moveInterval = moveInt;
        }

        public void Draw(Graphics g)
        {
            g.FillRectangle(Brushes.Yellow, location.X, location.Y, width, height);
        }

        public bool Move()
        {
            if (direction == Direction.Up)
            {
                location.Y -= moveInterval;

                //if distance is greater than 10 pixels, shot is still within the boundaries of the game
                if ( (location.Y - boundaries.Top) >= 10) return true;
                else return false;
            }

            //else the shot's going down
            else
            {
                location.Y += moveInterval;

                //if distance is greater than 10 pixels, shot is still within the boundaries of the game
                if ( (boundaries.Bottom - location.Y) >= 10) return true;
                else return false;
            }
        }

    }
}
