using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Stars_Test
{
    public class Stars
    {
        private struct Star
        {
            public Point point;
            public Pen pen;

            public Star(Point pt, Pen p)
            {
                point = pt;
                pen = p;
            }
        }

        //we'll need to store 300 stars
        List<Star> stars;

        private Rectangle boundaries;
        private Random random;

        public Stars(Rectangle bound, Random ran)
        {
            stars = new List<Star>();
            boundaries = bound;
            random = ran;
            for (int i = 0; i < 300; i++)
            {
                stars.Add(new Star(new Point(random.Next(boundaries.Left, boundaries.Right), random.Next(boundaries.Top, boundaries.Bottom)), RandomPen(random)));
            }
        }

        public void Draw(Graphics g)
        {
            foreach (Star star in stars)
                //drawing a rectangle that has a width and height of 1
                g.DrawRectangle(star.pen, star.point.X,
                                          star.point.Y, 1, 1);
        }

        public void Twinkle(Random random)
        {
            for (int i = 0; i < 5; i++)
            {
                stars.Remove(stars[random.Next(0, 300)]);
                stars.Add(new Star(new Point(random.Next(boundaries.Left, boundaries.Right), random.Next(boundaries.Top, boundaries.Bottom)), RandomPen(random)));
            }
        }

        private Pen RandomPen(Random random)
        {
            int choice = random.Next(0, 5);
            switch (choice)
            {
                case 0: return new Pen(Color.Red);
                case 1: return new Pen(Color.Yellow);
                case 2: return new Pen(Color.White);
                case 3: return new Pen(Color.SkyBlue);
                default: return new Pen(Color.Green);
            }
        }

    }
}
