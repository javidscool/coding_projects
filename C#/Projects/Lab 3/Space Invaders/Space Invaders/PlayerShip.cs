using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Space_Invaders
{
    public class PlayerShip
    {
        //defines the way the ship moves
        private const int HorizontalInterval = 10;

        private Point location;
        public Point Location { get { return location; } }

        private DateTime time;

        private bool alive;
        public bool Alive
        {
            get { return alive; }
            set
            {
                time = DateTime.Now;
                alive = value;
            }
        }

        private Bitmap image;
        private int deadShipHeight;

        //so the ships knows when it goes out of bounds
        private Rectangle boundaries;

        //to use in collision detection to detect any shots that hit the invader
        public Rectangle Area
        {
            get
            {
                return new Rectangle(location, image.Size);
            }
        }

        private static Bitmap ResizeImage(Image ImageToResize, int Width, int Height)
        {
            Bitmap bitmap = new Bitmap(Width, Height);
            using (Graphics graphics = Graphics.FromImage(bitmap))
            {
                graphics.DrawImage(ImageToResize, 0, 0, Width, Height);
            }
            return bitmap;
        }

        public PlayerShip(Rectangle bound, Point loc)
        {
            boundaries = bound;
            location = loc;
            alive = true;
            deadShipHeight = 100;
            image = ResizeImage(Properties.Resources.player, 50, 50);
        }

        TimeSpan duration;
        public void Draw(Graphics g)
        {
            if (alive)
            {
                deadShipHeight = 0;
                g.DrawImageUnscaled(image, location.X, location.Y);
            }
            else
            {
                DateTime now = DateTime.Now;
                duration = now - time;
                if (duration.Seconds >= 4) alive = true;

                if (deadShipHeight < 50)
                {
                    deadShipHeight++;
                    Point[] pts = new Point[3];
                    pts[0] = new Point(location.X, location.Y + deadShipHeight);
                    pts[1] = new Point(location.X + Area.Width, location.Y + deadShipHeight);
                    pts[2] = new Point(location.X, location.Y + Area.Height);
                    g.DrawImage(image, pts);
                }
            }
        }

        public void Move(Direction dir)
        {
            switch (dir)
            {
                case Direction.Left:
                    if( !( (Area.Left - boundaries.Left) <= 10) ) 
                        location.X -= HorizontalInterval;
                    break;
                case Direction.Right:
                    if (!( (boundaries.Right - Area.Right) <= 10))
                        location.X += HorizontalInterval;
                    break;
            }
        }

    }
}
