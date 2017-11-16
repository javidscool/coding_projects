using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace Space_Invaders
{
    public enum Type
    {
        Bug,
        Saucer,
        Satellite,
        Spaceship,
        Star
    }

    public class Invader
    {
        //how many pixels the invader moves up/down and left/right
        private const int HorizontalInterval = 10;
        private const int VerticalInterval = 30;

        private Bitmap image;
        private Game game;

        private Point location;
        public Point Location { get { return location; } }

        private Type invaderType;
        public Type InvaderType { get { return invaderType; } }

        //to use in collision detection to detect any shots that hit the invader
        public Rectangle Area
        {
            get
            {
                return new Rectangle(location, image.Size);
            }
        }

        public int Score { get; private set; }

        private static Bitmap ResizeImage(Image ImageToResize, int Width, int Height)
        {
            Bitmap bitmap = new Bitmap(Width, Height);
            using (Graphics graphics = Graphics.FromImage(bitmap))
            {
                graphics.DrawImage(ImageToResize, 0, 0, Width, Height);
            }
            return bitmap;
        }

        public Invader(Type invType, Point loc, Game gme, int sc){
            invaderType = invType;
            location = loc;
            Score = sc;
            game = gme;
            image = InvaderImage(0);
        }

        public void Move(Direction dir)
        {
          
            switch (dir)
            {
                case Direction.Down:

                    location.Y += VerticalInterval;
                    break;
                case Direction.Left:
                    location.X -= HorizontalInterval;
                    break;
                case Direction.Right:
                    location.X += HorizontalInterval;
                    break;
            }
          
        }

        public void Draw(Graphics g, int animationCell)
        {
            switch (animationCell)
            {
                case 0:
                    image = InvaderImage(0); g.DrawImageUnscaled(image, location.X, location.Y); break;
                case 1:
                    image = InvaderImage(1); g.DrawImageUnscaled(image, location.X, location.Y); break;
                case 2:
                    image = InvaderImage(2); g.DrawImageUnscaled(image, location.X, location.Y); break;
                case 3:
                    image = InvaderImage(3); g.DrawImageUnscaled(image, location.X, location.Y); break;
            }
        }

        private Bitmap InvaderImage(int animationCell)
        {
            return game.ReturnImage(this, animationCell);
        }

        
              
    }
}
